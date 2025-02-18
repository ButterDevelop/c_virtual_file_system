#include "VirtualFileSystem.hpp"
#include "Utils.hpp"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <sstream>

using std::streamsize;
using std::unordered_map;
using std::string;
using std::vector;
using std::ios;
using std::runtime_error;
using std::ofstream;
using std::stringstream;

VirtualFileSystem::VirtualFileSystem()
        : superblock(nullptr), inodes(nullptr), dataBitmap(nullptr),
          isFormatted(false), currentDir(nullptr), name(""), vfsFile(nullptr) {}

VirtualFileSystem::VirtualFileSystem(Superblock* superblock, Inode* inodes, int8_t* dataBitmap,
                                     bool isFormatted, Directory* currentDir,
                                     const string& name, fstream* vfsFile)
        : superblock(superblock), inodes(inodes), dataBitmap(dataBitmap),
          isFormatted(isFormatted), currentDir(currentDir), name(name), vfsFile(vfsFile) {}

VirtualFileSystem::VirtualFileSystem(const string& vfsName)
        : superblock(nullptr), inodes(nullptr), dataBitmap(nullptr),
          isFormatted(false), currentDir(nullptr), name(vfsName), vfsFile(nullptr) {

    vfsFile = new fstream(vfsName, ios::in | ios::out | ios::binary);

    if (vfsFile->is_open()) {
        vfsFile->seekg(0, ios::end);

        if (vfsFile->tellg() == 0) {
            // File is empty
            isFormatted = false;
        } else {
            // File is not empty
            isFormatted = true;
            vfsFile->seekg(0, ios::beg); // Rewind file pointer to the beginning
            loadVfs();
        }
    } else {
        isFormatted = false;
    }
}

VirtualFileSystem::~VirtualFileSystem() {
    delete superblock;
    delete[] inodes;
    delete[] dataBitmap;

    // Delete all directories
    for (auto& pair : allDirs) {
        delete pair.second;
    }
    // Clear the map
    allDirs.clear();

    delete vfsFile;
}

Directory* VirtualFileSystem::getDirectory(int32_t id) {
    if (id >= 0 && id <= allDirs.size()) {
        return allDirs[id];
    }
    return nullptr; // return nullptr if id is invalid
}

void VirtualFileSystem::addDirectory(Directory* dir, int32_t index) {
    allDirs[index] = dir;
}

unordered_map<int, Directory*>& VirtualFileSystem::getAllDirectories() {
    return allDirs;
}


void VirtualFileSystem::loadDirectoryFromVfs(Directory* dir, int id) {
    int blockCount;
    const int inodeCount = 64;
    char filename[12];

    vector<int32_t> dataBlocks = getDataBlocks(dir->getCurrent()->getInode(), &blockCount, nullptr);

    for (int i = 0; i < blockCount; i++) {
        seekDataCluster(dataBlocks[i]);
        for (int j = 0; j < inodeCount; j++) {
            int32_t nodeId;
            readFromFile(&nodeId);
            if (nodeId > 0) {
                readFromFile(reinterpret_cast<int8_t*>(filename), sizeof(filename));
                DirectoryItem* item = createDirectoryItem(nodeId, filename);
                if (inodes[nodeId].getIsDirectory()) {
                    dir->addSubdirectory(item);
                } else {
                    dir->addFile(item);
                }
            } else {
                seekCur(sizeof(filename));
            }
        }
    }

    // Recursively load subdirectories
    for (auto tempSubdir = dir->getSubdir(); tempSubdir != nullptr; tempSubdir = tempSubdir->getNext()) {
        auto newDirectory = new Directory();
        newDirectory->setParent(dir);
        newDirectory->setCurrent(tempSubdir);

        delete allDirs[tempSubdir->getInode()];
        allDirs[tempSubdir->getInode()] = newDirectory;
        loadDirectoryFromVfs(newDirectory, tempSubdir->getInode());
    }
}

void VirtualFileSystem::loadVfs() {
    if (!vfsFile->is_open()) {
        log(PLEASE_FORMAT_VFS_TEXT);
        isFormatted = false;
        return;  // File is not open
    }

    // Superblock initialization
    superblock = new Superblock();
    rewindVfs();

    char signatureBuffer[SIGNATURE_LENGTH + 1];
    vfsFile->read(signatureBuffer, SIGNATURE_LENGTH);
    signatureBuffer[SIGNATURE_LENGTH] = '\0';
    superblock->setSignature(signatureBuffer);
    readAndSet(*vfsFile, *superblock, &Superblock::setDiskSize);
    readAndSet(*vfsFile, *superblock, &Superblock::setClusterSize);
    readAndSet(*vfsFile, *superblock, &Superblock::setClusterCount);
    readAndSet(*vfsFile, *superblock, &Superblock::setInodeCount);
    readAndSet(*vfsFile, *superblock, &Superblock::setBitmapClusterCount);
    readAndSet(*vfsFile, *superblock, &Superblock::setInodeClusterCount);
    readAndSet(*vfsFile, *superblock, &Superblock::setDataClusterCount);
    readAndSet(*vfsFile, *superblock, &Superblock::setBitmapStartAddress);
    readAndSet(*vfsFile, *superblock, &Superblock::setInodeStartAddress);
    readAndSet(*vfsFile, *superblock, &Superblock::setDataStartAddress);

    dataBitmap = new int8_t[superblock->getClusterCount()];
    seekSet(superblock->getBitmapStartAddress());
    vfsFile->read(reinterpret_cast<char*>(dataBitmap), sizeof(int8_t) * superblock->getDataClusterCount());

    inodes = new Inode[superblock->getInodeCount()];
    for (int i = 0; i < superblock->getInodeCount(); i++) {
        seekSet(superblock->getInodeStartAddress() + i * INODE_SIZE);
        auto* inode = new Inode();
        readInodeFromFile(inode);

        inodes[i] = *inode;
        delete inode;
    }

    auto* rootItem = new DirectoryItem(0, "/");
    auto* rootDirectory = new Directory();
    rootDirectory->setCurrent(rootItem);
    rootDirectory->setParent(rootDirectory); // rootDirectory refers to itself as parent

    allDirs[0] = rootDirectory;

    // Set current directory to root
    currentDir = allDirs[0];

    // Load directory from vfs
    loadDirectoryFromVfs(currentDir, 0);
}

string VirtualFileSystem::getCurrentPath() {
    Directory* temp_dir = getCurrentDir();
    string result;

    while (temp_dir->getCurrent()->getInode() != allDirs[0]->getCurrent()->getInode()) {
        result = "/" + string(temp_dir->getCurrent()->getItemName()) + result;
        temp_dir = temp_dir->getParent();
    }

    if (result.empty()) {
        result = "/";
    }

    return result;
}

void VirtualFileSystem::freeInode(int id) {
    if (id < 0 || id >= superblock->getInodeCount()) {
        throw runtime_error("Invalid inode id.");
    }

    inodes[id].setNodeId(ID_ITEM_FREE);
    inodes[id].setIsDirectory(false);
    inodes[id].setReferences(1);
    inodes[id].setFileSize(0);
    inodes[id].setDirect(0, ID_ITEM_FREE);
    inodes[id].setDirect(1, ID_ITEM_FREE);
    inodes[id].setDirect(2, ID_ITEM_FREE);
    inodes[id].setDirect(3, ID_ITEM_FREE);
    inodes[id].setDirect(4, ID_ITEM_FREE);
    inodes[id].setIndirect(0, ID_ITEM_FREE);
    inodes[id].setIndirect(1, ID_ITEM_FREE);

}

Directory* VirtualFileSystem::findDirectory(const string& path) {
    Directory* dir = (path[0] == '/') ? allDirs[0] : currentDir;

    size_t start = 0;
    size_t end = path.find(PATH_DELIMETER);

    while (start != string::npos) {
        string part = (end == string::npos) ? path.substr(start) : path.substr(start, end - start);

        if (part.empty() || part == ".") {
            // Skip empty parts and current directory
        } else if (part == "..") {
            // Drop to parent directory
            dir = dir->getParent();
        } else {
            // Search for subdirectory or file
            bool found = false;
            DirectoryItem* item = dir->getSubdir();

            while (item != nullptr) {
                Directory* subdir = allDirs[item->getInode()];
                if (subdir && subdir->getCurrent()->getItemName() == part) {
                    dir = subdir;
                    found = true;
                    break;
                }
                item = item->getNext();
            }

            if (!found) {
                return nullptr;  // Subdirectory or file not found
            }
        }

        start = (end == string::npos) ? string::npos : end + 1;
        end = path.find(PATH_DELIMETER, start);
    }

    return dir;
}

Superblock * VirtualFileSystem::getSuperblock() const {
    return superblock;
}

void VirtualFileSystem::setSuperblock(Superblock* newSuperblock) {
    superblock = newSuperblock;
}

Inode* VirtualFileSystem::getInodes() const {
    return inodes;
}


void VirtualFileSystem::setInodes(Inode* newInodes) {
    inodes = newInodes;
}

int8_t* VirtualFileSystem::getDataBitmap() const {
    return dataBitmap;
}

void VirtualFileSystem::setDataBitmap(int8_t* newDataBitmap) {
    dataBitmap = newDataBitmap;
}

bool VirtualFileSystem::getIsFormatted() const {
    return isFormatted;
}

void VirtualFileSystem::setIsFormatted(bool newIsFormatted) {
    isFormatted = newIsFormatted;
}

Directory* VirtualFileSystem::getCurrentDir() const {
    return currentDir;
}

void VirtualFileSystem::setCurrentDir(Directory* newCurrentDir) {
    currentDir = newCurrentDir;
}

string VirtualFileSystem::getName() const {
    return name;
}

void VirtualFileSystem::setName(const string& newName) {
    name = newName;
}

fstream* VirtualFileSystem::getVfsFile() const {
    return vfsFile;
}

void VirtualFileSystem::setVfsFile(fstream* newVfsFile) {
    vfsFile = newVfsFile;
}

vector<int32_t> VirtualFileSystem::findFreeDataBlocks(int count) {
    vector<int32_t> blocks;
    blocks.reserve(count);

    for (int32_t i = 1; i < this->getSuperblock()->getDataClusterCount(); ++i) {
        if (this->getDataBitmap()[i] == 0) {
            blocks.push_back(i);
            if (blocks.size() == static_cast<size_t>(count)) {
                return blocks;
            }
        }
    }

    return {};
}

int32_t VirtualFileSystem::initializeInode(int32_t inodeId, int32_t size, int blockCount, vector<int32_t>& blocks) {
    int32_t lastDataBlock = 0;
    Inode& node = inodes[inodeId];

    node.setNodeId(inodeId);
    node.setIsDirectory(false);
    node.setReferences(1);
    node.setFileSize(size);
    node.setDirect(0, blocks[0]);

    if (blockCount > 1) {
        node.setDirect(1, blocks[1]);
        lastDataBlock = 1;
    }

    if (blockCount > 2) {
        node.setDirect(2, blocks[2]);
        lastDataBlock = 2;
    }

    if (blockCount > 3) {
        node.setDirect(3, blocks[3]);
        lastDataBlock = 3;
    }

    if (blockCount > 4) {
        node.setDirect(4, blocks[4]);
        lastDataBlock = 4;
    }

    // Work with indirect blocks
    if (blockCount > 5) {
        int blockCountWithIndirect = getBlockCountWithIndirect(blockCount);
        node.setIndirect(0, blocks[blockCountWithIndirect - 1]); // Add first indirect block

        if (blockCount > (CLUSTER_SIZE / sizeof(int32_t)) + 5) {
            node.setIndirect(1, blocks[blockCountWithIndirect - 2]); // Add second indirect block
            seekDataCluster(node.getIndirect(0));
            writeToFile(&blocks[5], sizeof(int32_t) * (CLUSTER_SIZE / sizeof(int32_t)));
            int tmpBlockCount = blockCount - (CLUSTER_SIZE / sizeof(int32_t) + 5);
            seekDataCluster(node.getIndirect(1));
            writeToFile(&blocks[(CLUSTER_SIZE / sizeof(int32_t) + 5)], sizeof(int32_t) * tmpBlockCount);
        }
        else {
            int tmpBlockCount = blockCount - 5;
            seekDataCluster(node.getIndirect(0));
            writeToFile(&blocks[5], sizeof(int32_t) * tmpBlockCount);
        }

        lastDataBlock = blockCount - 1;
    }

    return lastDataBlock;
}

int32_t VirtualFileSystem::findFreeInode() {
    for (int32_t i = 1; i < this->getSuperblock()->getInodeCount(); ++i) {
        if (this->getInodes()[i].getNodeId() == ID_ITEM_FREE) {
            return i;
        }
    }
    return ERROR_CODE;
}

vector<int32_t> VirtualFileSystem::getDataBlocks(int32_t nodeid, int* block_count, int* rest) {
    vector<int32_t> data_blocks;
    int32_t number;
    int max_numbers = 2 * (CLUSTER_SIZE / 4) + 5; // Max number of blocks in i-node
    Inode& node = inodes[nodeid];

    if (node.getIsDirectory()) {
        data_blocks.reserve(max_numbers);
        addDirectBlocks(node, data_blocks);
        addIndirectBlocks(node.getIndirect(0), data_blocks, CLUSTER_SIZE / sizeof(int32_t));
        addIndirectBlocks(node.getIndirect(1), data_blocks, CLUSTER_SIZE / sizeof(int32_t));
        *block_count = static_cast<int>(data_blocks.size());
    } else {
        *block_count = node.getFileSize() / CLUSTER_SIZE;
        if (rest != nullptr) {
            *rest = node.getFileSize() % CLUSTER_SIZE;
        }
        if (node.getFileSize() % CLUSTER_SIZE != 0) {
            (*block_count)++;
        }

        if (*block_count == 0) {
            data_blocks.resize(1);
        } else {
            data_blocks.resize(*block_count);
        }

        fillDirectBlocks(node, data_blocks);
        fillIndirectBlocks(node, data_blocks, *block_count);
    }

    return data_blocks;
}

void VirtualFileSystem::addDirectBlocks(const Inode& node, vector<int32_t>& blocks) {
    if (node.getDirect(0) != ID_ITEM_FREE) blocks.push_back(node.getDirect(0));
    if (node.getDirect(1) != ID_ITEM_FREE) blocks.push_back(node.getDirect(1));
    if (node.getDirect(2) != ID_ITEM_FREE) blocks.push_back(node.getDirect(2));
    if (node.getDirect(3) != ID_ITEM_FREE) blocks.push_back(node.getDirect(3));
    if (node.getDirect(4) != ID_ITEM_FREE) blocks.push_back(node.getDirect(4));
}

void VirtualFileSystem::addIndirectBlocks(int32_t indirect_block_address, vector<int32_t>& blocks, int max_blocks) {
    if (indirect_block_address != ID_ITEM_FREE) {
        seekDataCluster(indirect_block_address);
        int32_t number;
        for (int i = 0; i < max_blocks; ++i) {
            readFromFile(&number, 1);
            if (number > 0) blocks.push_back(number);
        }
    }
}

void VirtualFileSystem::fillDirectBlocks(const Inode& node, vector<int32_t>& blocks) {
    blocks[0] = node.getDirect(0);
    if (blocks.size() > 1) blocks[1] = node.getDirect(1);
    if (blocks.size() > 2) blocks[2] = node.getDirect(2);
    if (blocks.size() > 3) blocks[3] = node.getDirect(3);
    if (blocks.size() > 4) blocks[4] = node.getDirect(4);
}

void VirtualFileSystem::fillIndirectBlocks(const Inode& node, vector<int32_t>& blocks, int block_count) {
    if (block_count > 5) {
        int tmp = block_count - 5;
        seekDataCluster(node.getIndirect(0));
        readFromFile(&blocks[5], tmp);
        if (block_count > (CLUSTER_SIZE / sizeof(int32_t)) + 5) {
            tmp = block_count - (CLUSTER_SIZE / sizeof(int32_t)) - 5;
            seekDataCluster(node.getIndirect(1));
            readFromFile(&blocks[(CLUSTER_SIZE / sizeof(int32_t)) + 5], tmp);
        }
    }
}

void VirtualFileSystem::writeInodeToVfs(int id) {
    if (id < 0 || id >= superblock->getInodeCount()) {
        throw runtime_error("Invalid inode id.");
    }

    // Positioning to the beginning of the i-node
    seekSet(superblock->getInodeStartAddress() + id * INODE_SIZE);

    // Data writing to the file of the i-node
    writeInodeToFile(&inodes[id]);

    // Flushing the file ( saving changes )
    flushVfs();
}

void VirtualFileSystem::updateSizesInFile(Directory* dir, int32_t size) {
    Directory* d = dir;
    while (d != allDirs[0]) { // While not root
        inodes[d->getCurrent()->getInode()].setFileSize(inodes[d->getCurrent()->getInode()].getFileSize() + size);
        writeInodeToVfs(d->getCurrent()->getInode());
        d = d->getParent(); // Drop to parent directory
    }

    // Update size of root directory
    inodes[d->getCurrent()->getInode()].setFileSize(inodes[d->getCurrent()->getInode()].getFileSize() + size);
    writeInodeToVfs(d->getCurrent()->getInode());
}

void VirtualFileSystem::printDirItemInfo(const DirectoryItem* item) {
    Inode& node = inodes[item->getInode()];
    stringstream ss;
    ss << "Name: " << item->getItemName() << "\n"
       << "Size: " << node.getFileSize() << "B\n"
       << "i-node: " << node.getNodeId() << "\n"
       << "Direct blocks:\n";
    for (int i = 0; i < 5; ++i) {
        if (node.getDirect(i) != ID_ITEM_FREE) {
            ss << "  [" << i << "]: " << node.getDirect(i) << "\n";
        }
    }
    ss << "Indirect 1 blocks: ";
    printIndirectBlocks(node.getIndirect(0), ss);
    ss << "\nIndirect 2 blocks:";
    printIndirectBlocks(node.getIndirect(1), ss);
    log(ss.str());
}

void VirtualFileSystem::printIndirectBlocks(int32_t indirectBlockAddress, stringstream& ss) {
    if (indirectBlockAddress == ID_ITEM_FREE) {
        ss << "  FREE";
        return;
    }

    int32_t blockNumber;
    seekDataCluster(indirectBlockAddress);
    for (int i = 0; i < INT32_COUNT_IN_BLOCK; ++i) {
        readFromFile(&blockNumber, 1);
        if (blockNumber > 0) {
            ss << blockNumber << " ";
        } else {
            break;
        }
    }
}

int VirtualFileSystem::getBlockCountWithIndirect(int blockCount) {
    if (blockCount <= 5) { // Only direct blocks
        return blockCount;
    }

    if (blockCount <= CLUSTER_SIZE / sizeof(int32_t) + 5) { // One indirect block
        return blockCount + 1;
    } else { // Two indirect blocks
        return blockCount + 2;
    }

}

void VirtualFileSystem::updateBitmapInFile(DirectoryItem* item, int8_t value, vector<int32_t> const& dataBlocks) {
    int block_count;
    vector<int32_t> blocks = dataBlocks.empty() ? getDataBlocks(item->getInode(), &block_count, nullptr) : dataBlocks;

    // Update values in bitmap and write them to the file
    for (int32_t block : blocks) {
        dataBitmap[block] = value;
        seekSet(superblock->getBitmapStartAddress() + block);
        writeToFile(&value, 1);
    }

    // Indirect blocks
    updateIndirectBlocksInBitmap(inodes[item->getInode()].getIndirect(0), value);
    updateIndirectBlocksInBitmap(inodes[item->getInode()].getIndirect(1), value);

    // flush the file ( saving changes )
    flushVfs();
}

void VirtualFileSystem::updateIndirectBlocksInBitmap(int32_t indirectBlock, int8_t value) {
    if (indirectBlock != ID_ITEM_FREE) {
        dataBitmap[indirectBlock] = value;
        seekSet(superblock->getBitmapStartAddress() + indirectBlock);
        writeToFile(&value, 1);
    }
}

void VirtualFileSystem::cleanup() {
    delete superblock;
    superblock = nullptr;

    delete[] inodes;
    inodes = nullptr;

    delete[] dataBitmap;
    dataBitmap = nullptr;

    vector<unordered_map<int, Directory*>::iterator> deletionOrder;

    for (auto it = allDirs.begin(); it != allDirs.end(); ++it) {
        deletionOrder.push_back(it);
    }

    for (auto it = deletionOrder.rbegin(); it != deletionOrder.rend(); ++it) {
        delete (*it)->second; // remove directory
        allDirs.erase(*it); // remove directory from map
    }

    currentDir = nullptr;

    isFormatted = false;
}

bool VirtualFileSystem::removeDirectory(Directory* parentDir, const string& name) {
    if (!parentDir) {
        return false;
    }

    DirectoryItem** itemAddress = &(parentDir->getSubdir());
    DirectoryItem* item = parentDir->getSubdir();
    while (item != nullptr) {
        if (name == item->getItemName()) {
            Directory* dirToDelete = allDirs[item->getInode()];

            if (dirToDelete->getFile() != nullptr || dirToDelete->getSubdir() != nullptr) {
                return false;
            }

            *itemAddress = item->getNext();

            if (currentDir == dirToDelete) {
                currentDir = dirToDelete->getParent();
            }

            updateBitmapInFile(item, 0, {});
            freeInode(item->getInode());
            writeInodeToVfs(item->getInode());

            updateDirectoryInFile(parentDir, item, false);

            return true;
        }

        itemAddress = &(item->getNextRef());
        item = item->getNext();
    }

    return false;
}

bool VirtualFileSystem::format(int32_t filesystemSize) {
    cleanup();
    if (!vfsFile || !vfsFile->is_open()) {
        ofstream fileCreator(name, ios::out | ios::binary);
        if (!fileCreator.is_open()) {
            return false;
        }

        delete vfsFile;
        vfsFile = new fstream(name, ios::in | ios::out | ios::binary);
        if (!vfsFile->is_open()) {
            return false;
        }
    }

    flushVfs();
    delete superblock;
    superblock = ::superblockInit(filesystemSize);

    delete dataBitmap;
    delete inodes;

    dataBitmap = new int8_t[superblock->getClusterCount()]();
    inodes = new Inode[superblock->getInodeCount()]();

    // Create root directory
    auto* rootItem = new DirectoryItem(0, "/");
    auto* rootDirectory = new Directory();
    rootDirectory->setCurrent(rootItem);
    rootDirectory->setParent(rootDirectory); // Root directory refers to itself as parent

    delete allDirs[0];
    delete currentDir;

    // Add root directory to the map
    allDirs[0] = rootDirectory;
    currentDir = rootDirectory;

    // Free all i-nodes
    for (int i = 0; i < superblock->getInodeCount(); i++) {
        freeInode(i);
    }

    // Set root i-node to the first i-node
    inodes[0].setNodeId(0);
    inodes[0].setIsDirectory(true);
    inodes[0].setReferences(1);
    inodes[0].setDirect(0, 0); // First direct block is the first data block

    // Fill the file with empty data clusters
    char buffer[CLUSTER_SIZE];
    memset(buffer, 0, CLUSTER_SIZE);
    for (int i = 0; i < superblock->getClusterCount(); i++) {
        vfsFile->write(buffer, sizeof(buffer));
    }

    for (int i = 0; i < superblock->getDataClusterCount(); i++) {
        dataBitmap[i] = 0;
    }
    dataBitmap[0] = 1;

    // Save superblock
    rewindVfs();
    writeSuperblock();

    int8_t temp = 1;
    seekSet(superblock->getBitmapStartAddress());
    writeToFile(&temp);

    // Update bitmap in file
    updateBitmapInFile(rootItem, 1, {0});
    for (int i = 0; i < superblock->getInodeCount(); i++) {
        writeInodeToVfs(i);
    }

    isFormatted = true;

    return true;
}

void VirtualFileSystem::writeSuperblock() {
    if (!vfsFile->is_open()) {
        throw runtime_error("File stream is not open");
    }

    // Pozitioning to the beginning of the file
    vfsFile->seekp(0, ios::beg);

    // Writing signature
    vfsFile->write(superblock->getSignature(), SIGNATURE_LENGTH);

    int32_t temp;
    temp = superblock->getDiskSize();             writeToFile(&temp);
    temp = superblock->getClusterSize();          writeToFile(&temp);
    temp = superblock->getClusterCount();         writeToFile(&temp);
    temp = superblock->getInodeCount();           writeToFile(&temp);
    temp = superblock->getBitmapClusterCount();   writeToFile(&temp);
    temp = superblock->getInodeClusterCount();    writeToFile(&temp);
    temp = superblock->getDataClusterCount();     writeToFile(&temp);
    temp = superblock->getBitmapStartAddress();   writeToFile(&temp);
    temp = superblock->getInodeStartAddress();    writeToFile(&temp);
    temp = superblock->getDataStartAddress();     writeToFile(&temp);
}

void VirtualFileSystem::flushVfs() {
    vfsFile->flush();
}

int VirtualFileSystem::seekDataCluster(int blockNumber) const {
    return seekSet(superblock->getDataStartAddress() + blockNumber * CLUSTER_SIZE);
}

int VirtualFileSystem::seekSet(long int offset) const {
    vfsFile->seekg(offset, ios::beg);
    return vfsFile->good() ? 0 : -1;
}

int VirtualFileSystem::seekCur(long int offset) {
    vfsFile->seekg(offset, ios::cur);
    return vfsFile->good() ? 0 : -1;
}

void VirtualFileSystem::rewindVfs() const {
    vfsFile->clear();
    vfsFile->seekg(0, ios::beg);
}

template<typename T>
void VirtualFileSystem::readAndSet(fstream& file, Superblock& superblock, void(Superblock::*setter)(T)) {
    T temp;
    file.read(reinterpret_cast<char*>(&temp), sizeof(T));
    (superblock.*setter)(temp);
}

template<typename T>
streamsize VirtualFileSystem::writeToFile(const T* ptr, size_t count) {
    vfsFile->write(reinterpret_cast<const char*>(ptr), sizeof(T) * count);
    return vfsFile->good() ? count : 0;
}

template<typename T>
streamsize VirtualFileSystem::readFromFile(T* ptr, size_t count) {
    vfsFile->read(reinterpret_cast<char*>(ptr), sizeof(T) * count);
    return vfsFile->good() ? count : 0;
}

int VirtualFileSystem::createDirectoryInFile(Directory* dir, DirectoryItem* item) {
    const int maxItemsInBlock = 64;
    int blockCount = 0, rest = 0;
    vector<int32_t> blocks = getDataBlocks(dir->getCurrent()->getInode(), &blockCount, &rest);

    for (int block_number = 0; block_number < blockCount; block_number++) {
        this->seekDataCluster(blocks[block_number]);
        for (int j = 0; j < maxItemsInBlock; j++) {
            int32_t nodeId;
            readFromFile(&nodeId, 1);
            if (nodeId == 0) {
                seekCur(-4); // go back -4 bytes
                int32_t temp = item->getInode();
                writeToFile(&temp, 1); // Write address of i-node
                writeToFile(item->getItemName(), FILENAME_LENGTH); // Write filename to file
                flushVfs();
                return NO_ERROR_CODE;
            } else {
                seekCur(FILENAME_LENGTH); // skip filename length
            }
        }
    }

    // If there is no empty space
    vector<int32_t> freeBlock = findFreeDataBlocks(1);
    if (freeBlock.empty()) {
        return ERROR_CODE;
    }

    Inode& dirNode = inodes[dir->getCurrent()->getInode()];


    if (dirNode.getDirect(0) == ID_ITEM_FREE) dirNode.setDirect(0, freeBlock[0]);
    else if (dirNode.getDirect(1) == ID_ITEM_FREE) dirNode.setDirect(1, freeBlock[0]);
    else if (dirNode.getDirect(2) == ID_ITEM_FREE) dirNode.setDirect(2, freeBlock[0]);
    else if (dirNode.getDirect(3) == ID_ITEM_FREE) dirNode.setDirect(3, freeBlock[0]);
    else if (dirNode.getDirect(4) == ID_ITEM_FREE) dirNode.setDirect(4, freeBlock[0]);
    else {
        freeBlock = findFreeDataBlocks(2);
        if (freeBlock.empty()) return ERROR_CODE;

        if (dirNode.getIndirect(0) == ID_ITEM_FREE) {
            dirNode.setIndirect(0, freeBlock[1]);
        }
        else if (dirNode.getIndirect(1) == ID_ITEM_FREE) {
            dirNode.setIndirect(1, freeBlock[1]);
        }

        seekDataCluster(freeBlock[1]);
        writeToFile(&(freeBlock[0]));
    }

    seekDataCluster(freeBlock[0]);
    int32_t temp = item->getInode();
    writeToFile(&temp, 1);
    writeToFile(item->getItemName(), FILENAME_LENGTH);

    flushVfs();
    updateBitmapInFile(item, 1, freeBlock);
    writeInodeToVfs(dir->getCurrent()->getInode());

    return NO_ERROR_CODE;
}

bool VirtualFileSystem::removeFile(Directory* parentDir, DirectoryItem* item) {
    if (!parentDir || !item) {
        return false;
    }

    Inode& inode = inodes[item->getInode()];
    inode.setReferences(inode.getReferences() - 1);

    if (inode.getReferences() == 0) {
        int block_count, rest;
        vector<int32_t> blocks = getDataBlocks(item->getInode(), &block_count, &rest);

        // Clear data blocks
        char buffer[CLUSTER_SIZE];
        memset(buffer, 0, CLUSTER_SIZE);
        for (int32_t block : blocks) {
            seekDataCluster(block);
            writeToFile(buffer, sizeof(buffer));
        }

        // Clear indirect blocks
        clearIndirectBlocks(item->getInode());

        flushVfs();

        // Clear bitmap
        updateBitmapInFile(item, 0, blocks);

        // Update sizes in file
        updateSizesInFile(parentDir, -inode.getFileSize());

        freeInode(item->getInode());
    }

    updateDirectoryInFile(parentDir, item, false);
    writeInodeToVfs(item->getInode());

    return true;
}

Directory* VirtualFileSystem::getParentDirectory(DirectoryItem* item) {
    for (auto dir: allDirs) {
        if (findItemByInodeId(dir.second->getFile(), item->getInode()) != nullptr) {
            return dir.second;
        }
    }
    return nullptr;
}

void VirtualFileSystem::clearIndirectBlocks(int32_t inodeId) {
    Inode& inode = inodes[inodeId]; // Find inode by id
    char buffer[CLUSTER_SIZE];
    memset(buffer, 0, CLUSTER_SIZE); // Fill buffer with zeros

    // Clear indirect blocks if they are not empty
    if (inode.getIndirect(0) != ID_ITEM_FREE) {
        seekDataCluster(inode.getIndirect(0));
        writeToFile(buffer, sizeof(buffer));
        inode.setIndirect(0, ID_ITEM_FREE);
    }
    if (inode.getIndirect(1) != ID_ITEM_FREE) {
        seekDataCluster(inode.getIndirect(1));
        writeToFile(buffer, sizeof(buffer));
        inode.setIndirect(1, ID_ITEM_FREE);
    }
}


int VirtualFileSystem::removeDirectoryFromFile(Directory* dir, DirectoryItem* item) {
    const int maxItemsInBlock = 64;
    int empty[4];
    int32_t itemCount, block_count, rest;
    int32_t nodeId;
    vector<int32_t> blocks = this->getDataBlocks(dir->getCurrent()->getInode(), &block_count, &rest);

    memset(empty, 0, sizeof(empty));

    for (int block_number = 0; block_number < block_count; block_number++) {
        this->seekDataCluster(blocks[block_number]);
        itemCount = 0;
        bool found = false;

        for (int j = 0; j < maxItemsInBlock; j++) {
            this->readFromFile(&nodeId, 1);
            if (nodeId > 0) {
                itemCount++;
            }

            if (!found) {
                if (nodeId == item->getInode()) {
                    this->seekCur(-4);
                    this->writeToFile(&empty, 1);
                    this->flushVfs();
                    found = true;
                    if (itemCount > 1) break;
                }
            }

        }

        if (found) {
            if (itemCount == 1) {
                Inode* node = &(inodes[item->getInode()]);

                if (node->getDirect(0) != block_number) {	/* Don't remove first direct */
                    if (node->getDirect(1) == block_number) {
                        node->setDirect(1, ID_ITEM_FREE);
                    }
                    else if (node->getDirect(2) == block_number) {
                        node->setDirect(2, ID_ITEM_FREE);
                    }
                    else if (node->getDirect(3) == block_number) {
                        node->setDirect(3, ID_ITEM_FREE);
                    }
                    else if (node->getDirect(4) == block_number) {
                        node->setDirect(4, ID_ITEM_FREE);
                    }
                    else {
                        for (int i = 0; i < 2; i++) {
                            if (i == 0) {    // Go through indirect1
                                seekDataCluster(node->getIndirect(0));
                            } else {        // Go through indirect2
                                seekDataCluster(node->getIndirect(1));
                            }

                            int32_t count = 0, number;
                            found = false;
                            for (int j = 0; j < INT32_COUNT_IN_BLOCK; j++) {
                                readFromFile(&number);
                                if (number > 0)
                                    count++;
                                if (!found) {
                                    if (number == block_number) {
                                        found = true;
                                        seekCur(NEGATIVE_SIZE_OF_INT32);
                                        writeToFile(&empty, 1);
                                        flushVfs();
                                        if (count > 1)
                                            break;
                                    }
                                }
                            }

                            if (found) {
                                /* Remove indirect references if they are empty */
                                if (count == 1) {
                                    if (i == 0) {
                                        node->setIndirect(0, ID_ITEM_FREE);
                                    }
                                    else {
                                        node->setIndirect(1, ID_ITEM_FREE);
                                    }
                                }
                                break;
                            }
                        }
                    }

                    updateBitmapInFile(item, 0, {});
                    writeInodeToVfs(item->getInode());
                }
            }

            return NO_ERROR_CODE;
        }
    }

    return ERROR_CODE;
}

int VirtualFileSystem::updateDirectoryInFile(Directory* dir, DirectoryItem* item, bool create) {
    if (create) {	// Store item (find free space)
        return createDirectoryInFile(dir, item);
    }
    else {	// Remove item (find the item with the specific id)
        return removeDirectoryFromFile(dir, item);
    }
}

void VirtualFileSystem::writeInodeToFile(Inode* ptr) {
    int32_t int32;
    int8_t  int8;
    int32 = ptr->getNodeId();            writeToFile(&int32);
    int8  = ptr->getIsDirectory();       writeToFile(&int8);
    int8  = ptr->getReferences();        writeToFile(&int8);
    int32 = ptr->getFileSize();          writeToFile(&int32);
    int32 = ptr->getDirect(0);     writeToFile(&int32);
    int32 = ptr->getDirect(1);     writeToFile(&int32);
    int32 = ptr->getDirect(2);     writeToFile(&int32);
    int32 = ptr->getDirect(3);     writeToFile(&int32);
    int32 = ptr->getDirect(4);     writeToFile(&int32);
    int32 = ptr->getIndirect(0);   writeToFile(&int32);
    int32 = ptr->getIndirect(1);   writeToFile(&int32);
}

void VirtualFileSystem::readInodeFromFile(Inode* ptr, size_t count) {
    int32_t  int32;
    int8_t   int8;
    readFromFile(&int32); ptr->setNodeId(int32);
    readFromFile(&int8);  ptr->setIsDirectory(int8);
    readFromFile(&int8);  ptr->setReferences(int8);
    readFromFile(&int32); ptr->setFileSize(int32);
    readFromFile(&int32); ptr->setDirect(0, int32);
    readFromFile(&int32); ptr->setDirect(1, int32);
    readFromFile(&int32); ptr->setDirect(2, int32);
    readFromFile(&int32); ptr->setDirect(3, int32);
    readFromFile(&int32); ptr->setDirect(4, int32);
    readFromFile(&int32); ptr->setIndirect(0, int32);
    readFromFile(&int32); ptr->setIndirect(1, int32);
}

template streamsize VirtualFileSystem::readFromFile<char>(char*, size_t);