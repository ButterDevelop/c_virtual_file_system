#include <iostream>
#include <vector>
#include <cstring>
#include "Utils.hpp"
#include "CommandProcessor.hpp"
#include "VirtualFileSystem.hpp"

using std::string;
using std::vector;
using std::ios;
using std::memset;
using std::ifstream;
using std::ofstream;
using std::getline;


CommandProcessor::CommandProcessor(VirtualFileSystem* vfs) : vfs(vfs) {
    commandMap[HELP_COMMAND]        = [this](const string& args)    { this->processHelp(splitString(args));     }; // help         --    Display this helpful text
    commandMap[CP_COMMAND]          = [this](const string& args)    { this->processCp(splitString(args));       }; // cp s1 s2     --    Copy file from path s1 to path s2. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND
    commandMap[MV_COMMAND]          = [this](const string& args)    { this->processMv(splitString(args));       }; // mv s1 s2     --    Move or rename file from path s1 to path s2. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND
    commandMap[RM_COMMAND]          = [this](const string& args)    { this->processRm(splitString(args));       }; // rm s1        --    Delete file s1. Possible results: OK, FILE NOT FOUND
    commandMap[MKDIR_COMMAND]       = [this](const string& args)    { this->processMkdir(splitString(args));    }; // mkdir a1     --    Create directory a1. Possible results: OK, PATH NOT FOUND, EXIST
    commandMap[RMDIR_COMMAND]       = [this](const string& args)    { this->processRmdir(splitString(args));    }; // rmdir a1     --    Delete empty directory a1. Possible results: OK, FILE NOT FOUND, NOT EMPTY
    commandMap[LS_COMMAND]          = [this](const string& args)    { this->processLs(splitString(args));       }; // ls a1        --    List contents of directory a1 or current directory if a1 is omitted. Possible results: -FILE, +DIRECTORY, PATH NOT FOUND
    commandMap[CAT_COMMAND]         = [this](const string& args)    { this->processCat(splitString(args));      }; // cat s1       --    Display contents of file s1. Possible results: CONTENT, FILE NOT FOUND
    commandMap[CD_COMMAND]          = [this](const string& args)    { this->processCd(splitString(args));       }; // cd a1        --    Change current path to directory a1. Possible results: OK, PATH NOT FOUND
    commandMap[PWD_COMMAND]         = [this](const string& args)    { this->processPwd(splitString(args));      }; // pwd          --    Display current path. Possible results: PATH
    commandMap[INFO_COMMAND]        = [this](const string& args)    { this->processInfo(splitString(args));     }; // info s1/a1   --    Display information about file/directory s1/a1 (i-node number, direct and indirect links). Possible results: NAME – SIZE – i-node NUMBER, FILE NOT FOUND
    commandMap[INCP_COMMAND]        = [this](const string& args)    { this->processIncp(splitString(args));     }; // incp s1 s2   --    Upload file s1 from hard disk to path s2 in your FS. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND
    commandMap[OUTCP_COMMAND]       = [this](const string& args)    { this->processOutcp(splitString(args));    }; // outcp s1 s2  --    Upload file s1 from your FS to path s2 on hard disk. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND
    commandMap[LOAD_COMMAND]        = [this](const string& args)    { this->processLoad(splitString(args));     }; // load s1      --    Execute commands from file s1 on hard disk, one command per line. Possible results: OK, FILE NOT FOUND
    commandMap[FORMAT_COMMAND]      = [this](const string& args)    { this->processFormat(splitString(args));   }; // format size  --    Format the file system to the specified size (1K, 1M, 1G). If the file already contains data, it will be overwritten. Possible results: OK, CANNOT CREATE FILE
    commandMap[HARDLINK_COMMAND]    = [this](const string& args)    { this->processLn(splitString(args));       }; // ln s1 s2     --    Create a hard link to the file s1 named s2. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND

    // Limited functionality commands
    registerLimitedFunctionalityCommand(HELP_COMMAND);
    registerLimitedFunctionalityCommand(PWD_COMMAND);
    registerLimitedFunctionalityCommand(LOAD_COMMAND);
    registerLimitedFunctionalityCommand(FORMAT_COMMAND);

    if (!vfs->getIsFormatted()) {
        log(PLEASE_FORMAT_VFS_TEXT);
    }
}

void CommandProcessor::registerLimitedFunctionalityCommand(const string& command) {
    limitedFunctionalityCommands.insert(command);
}


bool CommandProcessor::isCommandAvailableInLimitedMode(const string& command) const {
    return limitedFunctionalityCommands.find(command) != limitedFunctionalityCommands.end();
}

void CommandProcessor::processHelp(const vector<string>& args) {
    if (vfs->getIsFormatted()) {
        log("");
        log("<===========================================================================================================================================================================>");
        log("                                                                             Available commands");
        log("<===========================================================================================================================================================================>");
        log("help          --    Display this helpful text");
        log("exit/quit     --    Well, goodbye");
        log("cp s1 s2      --    Copy file from path s1 to path s2. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND");
        log("mv s1 s2      --    Move or rename file from path s1 to path s2. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND");
        log("rm s1         --    Delete file s1. Possible results: OK, FILE NOT FOUND");
        log("mkdir a1      --    Create directory a1. Possible results: OK, PATH NOT FOUND, EXIST");
        log("rmdir a1      --    Delete empty directory a1. Possible results: OK, FILE NOT FOUND, NOT EMPTY");
        log("ls a1         --    List contents of directory a1 or current directory if a1 is omitted. Possible results: -FILE, +DIRECTORY, PATH NOT FOUND");
        log("cat s1        --    Display contents of file s1. Possible results: CONTENT, FILE NOT FOUND");
        log("cd a1         --    Change current path to directory a1. Possible results: OK, PATH NOT FOUND");
        log("pwd           --    Display current path. Possible results: PATH");
        log("info s1/a1    --    Display information about file/directory s1/a1 (i-node number, direct and indirect links). Possible results: NAME – SIZE – i-node NUMBER, FILE NOT FOUND");
        log("incp s1 s2    --    Upload file s1 from hard disk to path s2 in your FS. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND");
        log("outcp s1 s2   --    Upload file s1 from your FS to path s2 on hard disk. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND");
        log("load s1       --    Execute commands from file s1 on hard disk, one command per line. Possible results: OK, FILE NOT FOUND");
        log("format size   --    Format the file system to the specified size (1K, 1M, 1G). If the file already contains data, it will be overwritten. Possible results: OK, CANNOT CREATE FILE");
        log("ln s1 s2      --    Create a hard link to the file s1 named s2. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND");
        log("<===========================================================================================================================================================================>");
        log("");
    } else {
        log("");
        log("<===========================================================================================================================================================================>");
        log("                                                                     Limited Mode Available commands");
        log("<===========================================================================================================================================================================>");
        log("help          --    Display this helpful text");
        log("exit/quit     --    Well, goodbye");
        log("pwd           --    Display current path. Possible results: PATH");
        log("load s1       --    Execute commands from file s1 on hard disk, one command per line. Possible results: OK, FILE NOT FOUND");
        log("format size   --    Format the file system to the specified size (1K, 1M, 1G). If the file already contains data, it will be overwritten. Possible results: OK, CANNOT CREATE FILE");
        log("<===========================================================================================================================================================================>");
        log("Use 'format' command to create VFS necessaries and leave limited mode.");
        log("");
    }
}


void CommandProcessor::processCp(const vector<string>& args) {
    if (args.size() != 2) {
        log(WRONG_NUMBER_OF_ARGS_TEXT);
        return;
    }

    const string& srcPath = args[0];
    const string& destPath = args[1];

    string srcDirPath = getDirPath(srcPath);
    string srcFileName = getFileName(srcPath);

    string destDirPath = getDirPath(destPath);
    string destFileName = getFileName(destPath);

    if (destFileName.empty()) {
        destFileName = srcFileName;
    }

    Directory* srcDir = vfs->findDirectory(srcDirPath);
    if (!srcDir) {
        log(SOURCE_DIR_NOT_FOUND_TEXT);
        return;
    }

    DirectoryItem* srcItem = findItem(srcDir->getFile(), srcFileName.c_str());
    if (!srcItem) {
        log(SOURCE_FILE_NOT_FOUND_TEXT);
        return;
    }

    Directory* destDir = vfs->findDirectory(destDirPath);
    if (!destDir) {
        log(DESTINATION_DIR_NOT_FOUND_TEXT);
        return;
    }

    if (findItem(destDir->getFile(), destFileName.c_str())) {
        log(FILE_ALREADY_EXISTS_IN_DESTINATION_DIR_TEXT);
        return;
    }

    int blockCount, rest;
    vector<int32_t> sourceBlocks = vfs->getDataBlocks(srcItem->getInode(), &blockCount, &rest);

    int realBlockCount = vfs->getBlockCountWithIndirect(blockCount);

    vector<int32_t> freeBlocks = vfs->findFreeDataBlocks(realBlockCount);
    if (freeBlocks.empty()) {
        log(NOT_ENOUGH_SPACE_BLOCKS_TEXT);
        return;
    }

    int32_t freeInode = vfs->findFreeInode();
    if (freeInode == ERROR_CODE) {
        log(NO_FREE_INODES_TEXT);
        return;
    }

    int lastBlockIndex = vfs->initializeInode(freeInode, vfs->getInodes()[srcItem->getInode()].getFileSize(), blockCount, freeBlocks);
    DirectoryItem* newItem = new DirectoryItem(freeInode, destFileName.c_str());
    destDir->addFile(newItem);

    vfs->updateBitmapInFile(newItem, 1, freeBlocks);
    vfs->writeInodeToVfs(freeInode);
    vfs->updateSizesInFile(destDir, vfs->getInodes()[newItem->getInode()].getFileSize());
    vfs->updateDirectoryInFile(destDir, newItem, true);

    char buffer[CLUSTER_SIZE];
    for (int i = 0; i < blockCount - 1; i++) {
        vfs->seekDataCluster(sourceBlocks[i]);
        vfs->readFromFile<char>(buffer, CLUSTER_SIZE);
        vfs->seekDataCluster(freeBlocks[i]);
        vfs->writeToFile<char>(buffer, CLUSTER_SIZE);
        vfs->flushVfs();
    }

    memset(buffer, 0, CLUSTER_SIZE);
    int lastBlockSize = (rest == 0) ? CLUSTER_SIZE : rest;
    vfs->seekDataCluster(sourceBlocks.back());
    vfs->readFromFile<char>(buffer, lastBlockSize);
    vfs->seekDataCluster(freeBlocks[lastBlockIndex]);
    vfs->writeToFile<char>(buffer, lastBlockSize);

    log(FILE_COPIED_SECCESSFULLY_TEXT);
}


void CommandProcessor::processMv(const vector<string>& args) {
    if (args.size() != 2) {
        log(WRONG_NUMBER_OF_ARGS_TEXT);
        return;
    }

    const string& srcPath = args[0];
    const string& destPath = args[1];

    string srcDirPath = getDirPath(srcPath);
    string srcFileName = getFileName(srcPath);

    string destDirPath = getDirPath(destPath);
    string destFileName = getFileName(destPath);

    if (destFileName.empty()) {
        destFileName = srcFileName;
    }

    Directory* srcDir = vfs->findDirectory(srcDirPath);
    if (!srcDir) {
        log(SOURCE_DIR_NOT_FOUND_TEXT);
        return;
    }

    Directory* destDir = vfs->findDirectory(destDirPath);
    if (!destDir) {
        log(DESTINATION_DIR_NOT_FOUND_TEXT);
        return;
    }

    DirectoryItem* srcItem = findItem(srcDir->getFile(), srcFileName.c_str());
    if (!srcItem) {
        log(SOURCE_FILE_NOT_FOUND_TEXT);
        return;
    }

    if (findItem(destDir->getFile(), destFileName.c_str())) {
        log(FILE_ALREADY_EXISTS_IN_DESTINATION_DIR_TEXT);
        return;
    }

    DirectoryItem* item = srcDir->getFile();
    DirectoryItem** prevItem = &srcDir->getFile();
    bool removed = false;
    while (item != nullptr) {
        if (item->getItemName() == srcFileName) {
            *prevItem = item->getNext();
            vfs->updateSizesInFile(srcDir, -vfs->getInodes()[item->getInode()].getFileSize());
            vfs->updateDirectoryInFile(srcDir, item, false);
            removed = true;
            break;
        }
        prevItem =  &(item->getNextRef());
        item = item->getNext();
    }

    if (!removed) {
        log(SOURCE_FILE_NOT_FOUND_TEXT);
        return;
    }

    DirectoryItem** newItem = &(destDir->getFile());
    while (*newItem != nullptr) {
        newItem = &(*newItem)->getNextRef();
    }

    *newItem = item;

    vfs->updateSizesInFile(destDir, vfs->getInodes()[item->getInode()].getFileSize());
    vfs->updateDirectoryInFile(destDir, item, true);

    log(OK_TEXT);
}

void CommandProcessor::processRm(const vector<string>& args) {
    if (args.size() != 1) {
        log(WRONG_NUMBER_OF_ARGS_TEXT);
        return;
    }

    const string& filePath = args[0];

    string dirPath = getDirPath(filePath);
    string fileName = getFileName(filePath);

    Directory* dir = vfs->findDirectory(dirPath);
    if (dir == nullptr) {
        log(DIRECTORY_NOT_FOUND_TEXT);
        return;
    }

    DirectoryItem* item = dir->deleteFileFromDirectory(fileName);
    if (item == nullptr) {
        log(FILE_NOT_FOUND_TEXT);
        return;
    }

    vfs->removeFile(dir, item);

    log(OK_TEXT);
}


void CommandProcessor::processMkdir(const vector<string>& args) {
    if (args.size() != 1) {
        log(WRONG_NUMBER_OF_ARGS_TEXT);
        return;
    }

    string parentPath;
    const string& name = args[0];

    Directory* parentDir = vfs->findDirectory(parentPath);
    if (parentDir == nullptr) {
        log(PARENT_DIR_NOT_EXISTS_TEXT);
        return;
    }

    if (name.length() >= FILENAME_LENGTH) {
        log(FIlENAME_IS_TOO_LONG_TEXT);
        return;
    }

    if (findItem(parentDir->getSubdir(), name.c_str()) != nullptr) {
        log(DIRECTORY_ALREADY_EXISTS_TEXT);
        return;
    }

    // Getting free inode
    int32_t inode_id = vfs->findFreeInode();
    if (inode_id == ERROR_CODE) {
        log(NOT_ENOUGH_SPACE_BLOCKS_TEXT);
        return;
    }

    // Getting free data block
    vector<int32_t> data_blocks = vfs->findFreeDataBlocks(1);
    if (data_blocks.empty()) {
        log(NOT_ENOUGH_SPACE_BLOCKS_TEXT);
        return;
    }

    // Creating new directory and directory item
    auto newDir = new Directory();
    auto newDirItem = new DirectoryItem(inode_id, name.c_str());
    newDir->setParent(parentDir);
    newDir->setCurrent(newDirItem);

    vfs->addDirectory(newDir, inode_id);

    // Updating bitmap and inode
    vfs->getDataBitmap()[data_blocks[0]] = 1;

    Inode& newInode = vfs->getInodes()[inode_id];
    newInode.setNodeId(inode_id);
    newInode.setIsDirectory(true);
    newInode.setReferences(1);
    newInode.setFileSize(0);
    newInode.setDirect(0, data_blocks[0]);

    parentDir->addSubdirectory(vfs->getDirectory(inode_id)->getCurrent());

    vfs->updateDirectoryInFile(parentDir, newDir->getCurrent(), true);

    // Saving new directory to VFS
    vfs->writeInodeToVfs(inode_id);
    vfs->updateBitmapInFile(newDir->getCurrent(), true, data_blocks);

    log(OK_TEXT);
}

void CommandProcessor::processRmdir(const vector<string>& args) {
    if (args.size() != 1) {
        log(WRONG_NUMBER_OF_ARGS_TEXT);
        return;
    }

    string parentPath;
    const string& name = args[0];

    Directory* parentDir = vfs->findDirectory(parentPath);
    if (parentDir == nullptr) {
        log(PARENT_DIR_NOT_EXISTS_TEXT);
        return;
    }

    if (vfs->removeDirectory(parentDir, name)) {
        log(OK_TEXT);
    } else {
        log(DIR_NOT_FOUND_OR_NOT_EMPTY_TEXT);
    }

}


void CommandProcessor::processLs(const vector<string>& args) {
    string currentPath = vfs->getCurrentPath();

    if (args.size() > 1) {
        log(WRONG_NUMBER_OF_ARGS_TEXT);
        return;
    }

    if (args.size() == 1) {
        currentPath = args[0];
    }

    Directory* dir = vfs->findDirectory(currentPath);
    if (dir == nullptr) {
        log(PATH_NOT_FOUND_TEXT);
        return;
    }

    // Output all subdirectories
    DirectoryItem* subdirItem = dir->getSubdir();
    while (subdirItem != nullptr) {
        log("+" + string(subdirItem->getItemName()));
        subdirItem = subdirItem->getNext();
    }

    // Output all files
    DirectoryItem* fileItem = dir->getFile();
    while (fileItem != nullptr) {
        log("-" + string(fileItem->getItemName()));
        fileItem = fileItem->getNext();
    }
}

void CommandProcessor::processCat(const vector<string>& args) {
    if (args.size() != 1) {
        log(WRONG_NUMBER_OF_ARGS_TEXT);
        return;
    }

    const string& filepath = args[0];

    // Parse the file path to get directory path and file name
    string dirPath = getDirPath(filepath);
    string fileName = getFileName(filepath);

    // Find the directory
    Directory* dir = vfs->findDirectory(dirPath);
    if (dir == nullptr) {
        log(DIRECTORY_NOT_FOUND_TEXT);
        return;
    }

    // Find the file item within the directory
    DirectoryItem* item = findItem(dir->getFile(), fileName.c_str());
    if (item == nullptr) {
        log(FILE_NOT_FOUND_TEXT);
        return;
    }

    int blockCount, rest;
    vector<int32_t> blocks = vfs->getDataBlocks(item->getInode(), &blockCount, &rest);
    char buffer[CLUSTER_SIZE];
    memset(buffer, 0, CLUSTER_SIZE);

    for (int i = 0; i < blockCount - 1; i++) {
        memset(buffer, 0, CLUSTER_SIZE);
        vfs->seekDataCluster(blocks[i]);
        vfs->readFromFile<char>(buffer, CLUSTER_SIZE);
        log(buffer, false);
    }

    // Handle the last block
    int lastBlockSize = (rest == 0) ? CLUSTER_SIZE : rest;
    memset(buffer, 0, CLUSTER_SIZE);
    vfs->seekDataCluster(blocks.back());
    vfs->readFromFile<char>(buffer, CLUSTER_SIZE);
    log(buffer);
}


void CommandProcessor::processCd(const vector<string>& args) {
    if (args.size() != 1) {
        log(WRONG_NUMBER_OF_ARGS_TEXT);
        return;
    }

    Directory* dir = vfs->findDirectory(args[0]);
    if (dir == nullptr) {
        log(PATH_NOT_FOUND_TEXT);
        return;
    }

    vfs->setCurrentDir(dir);
    processPwd(args);
}

void CommandProcessor::processPwd(const vector<string>& args) {
    log(vfs->getCurrentPath());
}

void CommandProcessor::processInfo(const vector<string>& args) {
    if (args.size() > 1) {
        log(WRONG_NUMBER_OF_ARGS_TEXT);
        return;
    }

    const string& path = args[0];

    if (args.empty()) {
        vfs->printDirItemInfo(vfs->getCurrentDir()->getCurrent());
        return;
    }

    string dirPath = getDirPath(path);
    string itemName = getFileName(path);

    // Find directory
    Directory* dir = vfs->findDirectory(dirPath);
    if (!dir) {
        log(DIRECTORY_NOT_FOUND_WITH_NAME_TEXT + dirPath);
        return;
    }

    // Find directory item
    DirectoryItem* item = nullptr;
    for (DirectoryItem* tempItem = dir->getFile(); tempItem != nullptr; tempItem = tempItem->getNext()) {
        if (tempItem->getItemName() == itemName) {
            item = tempItem;
            break;
        }
    }

    if (!item) {
        for (DirectoryItem* tempItem = dir->getSubdir(); tempItem != nullptr; tempItem = tempItem->getNext()) {
            if (tempItem->getItemName() == itemName) {
                item = tempItem;
                break;
            }
        }
    }

    if (!item) {
        log(ITEM_NOT_FOUND_TEXT + itemName);
        return;
    }

    vfs->printDirItemInfo(item);
}

void CommandProcessor::processIncp(const vector<string>& args) {
    if (args.size() != 2) {
        log(WRONG_NUMBER_OF_ARGS_TEXT);
        return;
    }

    const string& filepath_src = args[0];
    const string& filepath_dest = args[1];

    string dirPath = getDirPath(filepath_dest);
    string fileName = getFileName(filepath_dest);

    if (fileName.length() >= FILENAME_LENGTH) {
        log(FIlENAME_IS_TOO_LONG_TEXT);
        return;
    }

    if (fileName.length() >= FILENAME_LENGTH) {
        log(FIlENAME_IS_TOO_LONG_TEXT);
        return;
    }

    // Find destination directory
    Directory* dir = vfs->findDirectory(dirPath);
    if (dir == nullptr) {
        log(DESTINATION_PATH_NOT_FOUND_TEXT);
        return;
    }
    if (findItem(dir->getFile(), fileName.c_str()) != nullptr) {
        log(FILE_ALREADY_EXISTS_TEXT);
        return;
    }

    ifstream src_file(filepath_src, ios::binary);
    if (!src_file) {
        log(SOURCE_FILE_NOT_FOUND_TEXT);
        return;
    }

    src_file.seekg(0, ios::end);
    int32_t fileSize = static_cast<int32_t>(src_file.tellg());
    src_file.seekg(0, ios::beg);

    int blockCount = fileSize / CLUSTER_SIZE;
    if (fileSize % CLUSTER_SIZE != 0) blockCount++;

    int realBlockCount = vfs->getBlockCountWithIndirect(blockCount);

    vector<int32_t> blocks = vfs->findFreeDataBlocks(realBlockCount);
    if (blocks.empty()) {
        log(NOT_ENOUGH_SPACE_BLOCKS_TEXT);
        return;
    }

    int32_t inodeId = vfs->findFreeInode();
    if (inodeId == ERROR_CODE) {
        log(NO_FREE_INODES_TEXT);
        return;
    }

    auto* newItem = new DirectoryItem(inodeId, fileName.c_str());
    dir->addFile(newItem);

    int lastBlockIndex = vfs->initializeInode(inodeId, fileSize, blockCount, blocks);

    vfs->updateBitmapInFile(newItem, true, blocks);
    vfs->writeInodeToVfs(inodeId);
    vfs->updateDirectoryInFile(dir, newItem, true);
    vfs->updateSizesInFile(dir, fileSize);

    char buffer[CLUSTER_SIZE];
    memset(buffer, 0, CLUSTER_SIZE);

    for (int i = 0; i < blockCount - 1; i++) {
        src_file.read(buffer, CLUSTER_SIZE);
        vfs->seekDataCluster(blocks[i]);
        vfs->writeToFile(buffer, CLUSTER_SIZE);
    }

    int lastBlockSize = fileSize % CLUSTER_SIZE;
    if (lastBlockSize == 0) lastBlockSize = CLUSTER_SIZE;
    char partBuffer[lastBlockSize];

    src_file.read(partBuffer, lastBlockSize);
    vfs->seekDataCluster(blocks[lastBlockIndex]);
    vfs->writeToFile(partBuffer, lastBlockSize);

    vfs->flushVfs();

    src_file.close();

    log(FILE_COPIED_SECCESSFULLY_TEXT);
}

void CommandProcessor::processOutcp(const vector<string>& args) {
    if (args.size() != 2) {
        log(WRONG_NUMBER_OF_ARGS_TEXT);
        return;
    }

    const string& vfsFilePath = args[0];
    const string& externalFilePath = args[1];

    string dirPath = getDirPath(vfsFilePath);
    string fileName = getFileName(vfsFilePath);

    Directory* dir = vfs->findDirectory(dirPath);
    if (!dir) {
        log(DIRECTORY_NOT_FOUND_IN_VFS_TEXT + dirPath);
        return;
    }

    DirectoryItem* item = findItem(dir->getFile(), fileName.c_str());
    if (!item) {
        log(FILE_NOT_FOUND_IN_VFS_TEXT + fileName);
        return;
    }

    // open file on hard drive for writing
    ofstream outputFile(externalFilePath, ios::binary | ios::out);
    if (!outputFile) {
        log(COULD_NOT_OPEN_FILE_ON_HARD_DISK_FOR_WRITING + externalFilePath);
        return;
    }

    // get data blocks
    int blockCount, rest;
    vector<int32_t> blocks = vfs->getDataBlocks(item->getInode(), &blockCount, &rest);
    char buffer[CLUSTER_SIZE];

    // Copying all blocks except the last one
    for (int i = 0; i < blockCount - 1; i++) {
        vfs->seekDataCluster(blocks[i]);
        vfs->readFromFile<char>(buffer, CLUSTER_SIZE);
        outputFile.write(buffer, CLUSTER_SIZE);
    }

    // Copying the last block
    int lastBlockSize = (rest == 0) ? CLUSTER_SIZE : rest;
    vfs->seekDataCluster(blocks.back());
    vfs->readFromFile<char>(buffer, lastBlockSize);
    outputFile.write(buffer, lastBlockSize);

    outputFile.close();
    log(FILE_SUCESSFULLY_COPIED_FROM_VFS_TO_TEXT + externalFilePath);
}

void CommandProcessor::processLoad(const vector<string>& args) {
    if (args.size() != 1) {
        log(WRONG_NUMBER_OF_ARGS_TEXT);
        return;
    }

    ifstream command_file(args[0]);
    if (!command_file.is_open()) {
        log(FILE_NOT_FOUND_TEXT);
        return;
    }

    string line;
    while (getline(command_file, line)) {
        string input = removeEndOfLine(line);
        if (input.empty()) {
            continue;
        }

        log("> " + input);
        processCommandLine(input);
    }

    log(FILE_COMPLETE_TEXT);
}

void CommandProcessor::processFormat(const vector<string>& args) {
    if (args.size() != 1) {
        log(WRONG_NUMBER_OF_ARGS_TEXT);
        return;
    }

    int32_t vfsSize = getSizeFromString(args[0]);
    if (vfsSize <= 0) {
        log(NUMBER_PROBABLY_IS_WRONG);
        return;
    }

    if (vfs->format(vfsSize)) {
        log(FORMAT_SUCCESSFUL_TEXT);
    } else {
        log(FORMAT_ERROR_TEXT);
    }
}

void CommandProcessor::processLn(const vector<string>& args) {
    if (args.size() != 2) {
        log(WRONG_NUMBER_OF_ARGS_TEXT);
        return;
    }

    const string& sourcePath = args[0];
    string linkDir = getDirPath(args[1]), linkName = getFileName(args[1]);

    // Find source directory
    Directory* sourceDir = vfs->findDirectory(getDirPath(sourcePath));
    if (!sourceDir) {
        log(SOURCE_DIR_NOT_FOUND_TEXT);
        return;
    }
    DirectoryItem* sourceItem = findItem(sourceDir->getFile(), getFileName(sourcePath).c_str());
    if (!sourceItem) {
        log(SOURCE_FILE_NOT_FOUND_TEXT);
        return;
    }
    int32_t sourceInodeId = sourceItem->getInode();

    Directory* targetDir = vfs->findDirectory(linkDir);
    if (!targetDir) {
        log(TARGET_DIR_NOT_FOUND_TEXT);
        return;
    }

    // Check if file or directory with the same name already exists
    if (findItem(targetDir->getFile(), linkName.c_str()) || findItem(targetDir->getSubdir(), linkName.c_str())) {
        log("A file or directory with the name '" + linkName + "' already exists."); // I'v decided not to put it in consts
        return;
    }

    // Initialize directory item
    DirectoryItem* newLinkItem = createDirectoryItem(sourceInodeId, linkName.c_str());

    targetDir->addFile(newLinkItem);

    Inode& inode = vfs->getInodes()[sourceItem->getInode()];
    inode.setReferences(inode.getReferences() + 1);

    // Update inode in VFS
    vfs->writeInodeToVfs(sourceInodeId);

    vfs->updateDirectoryInFile(targetDir, newLinkItem, true);

    vfs->flushVfs();

    log(OK_TEXT);
}

void CommandProcessor::processCommandLine(const string& input) {
    size_t pos = input.find(' ');
    pos = pos == string::npos ? input.length() : pos;
    string command = input.substr(0, pos);
    string args = pos + 1 >= input.length() ? "" : input.substr(pos + 1);

    if (command == EXIT_COMMAND || command == QUIT_COMMAND) {
        log(END_OF_PROGRAM_TEXT);
        exit(0);
    }

    auto it = commandMap.find(command);
    if (it != commandMap.end()) {
        if (!vfs->getIsFormatted() && !isCommandAvailableInLimitedMode(command)) {
            log(COMMAND_IS_NOT_AVAILABLE_TEXT);
            log(PLEASE_FORMAT_VFS_TEXT);
            return;
        }
        it->second(args);
    } else {
        log(UNKNOWN_COMMAND_TEXT + command);
    }
}

