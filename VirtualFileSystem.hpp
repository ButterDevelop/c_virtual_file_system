#ifndef SEMESTRALNIPRACE_VIRTUALFILESYSTEM_HPP
#define SEMESTRALNIPRACE_VIRTUALFILESYSTEM_HPP

#include <string>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <fstream>
#include <unordered_map>
#include "Constants.hpp"
#include "Inode.hpp"
#include "Directory.hpp"
#include "Superblock.hpp"

using std::streamsize;
using std::unordered_map;
using std::fstream;
using std::string;
using std::vector;
using std::stringstream;

class VirtualFileSystem {
public:

    /**
     * Default constructor for virtual file system
     */
    VirtualFileSystem();

    /**
     * Constructor for virtual file system
     * @param superblock reference to superblock
     * @param inodes reference to inodes
     * @param dataBitmap reference to data bitmap
     * @param isFormatted flag indicating whether the file system is formatted
     * @param currentDir reference to current directory
     * @param name name of the file system file
     * @param vfsFile reference to file stream
     */
    VirtualFileSystem(Superblock* superblock,
                      Inode* inodes,
                      int8_t* dataBitmap,
                      bool isFormatted,
                      Directory* currentDir,
                      const string& name,
                      fstream* vfsFile);

    /**
     * Constructor for virtual file system
     * @param vfsName name of the file system file
     */
    VirtualFileSystem(const string& vfsName);

    /**
     * Destructor for virtual file system
     */
    ~VirtualFileSystem();

    /**
     * Gets current path in the virtual file system (e.g. /home/user)
     * @return current path in the virtual file system
     */
    string getCurrentPath();

    /**
     * Finds directory with the given path in the virtual file system or nullptr if the directory is not found
     * @param path path to the directory
     * @return directory with the given path in the virtual file system or nullptr if the directory is not found
     */
    Directory* findDirectory(const string& path);

    /**
     * Gets superblock of the virtual file system
     * @return superblock of the virtual file system
     */
    Superblock* getSuperblock() const;

    /**
     * Sets superblock of the virtual file system
     * @param newSuperblock new superblock
     */
    void setSuperblock(Superblock* newSuperblock);

    /**
     * Gets i-nodes of the virtual file system
     * @return i-nodes of the virtual file system
     */
    Inode* getInodes() const;

    /**
     * Sets i-nodes of the virtual file system
     * @param newInodes new i-nodes
     */
    void setInodes(Inode* newInodes);

    /**
     * Gets the number of blocks with indirect blocks for the given number of blocks if its needed
     * @param blockCount number of blocks
     * @return number of blocks with indirect blocks for the given number of blocks if its needed
     */
    int getBlockCountWithIndirect(int blockCount);

    /**
     * Gets data bitmap of the virtual file system
     * @return data bitmap of the virtual file system
     */
    int8_t* getDataBitmap() const;

    /**
     * Sets data bitmap of the virtual file system
     * @param newDataBitmap new data bitmap
     */
    void setDataBitmap(int8_t* newDataBitmap);

    /**
     * Gets flag indicating whether the virtual file system is formatted
     * @return flag indicating whether the virtual file system is formatted
     */
    bool getIsFormatted() const;

    /**
     * Sets flag indicating whether the virtual file system is formatted
     * @param newIsFormatted new flag indicating whether the virtual file system is formatted
     */
    void setIsFormatted(bool newIsFormatted);

    /**
     * Gets current directory in the virtual file system
     * @return current directory in the virtual file system
     */
    Directory* getCurrentDir() const;

    /**
     * Sets current directory in the virtual file system
     * @param newCurrentDir new current directory
     */
    void setCurrentDir(Directory* newCurrentDir);

    /**
     * Gets name of the virtual file system file
     * @return name of the virtual file system file
     */
    string getName() const;

    /**
     * Sets name of the virtual file system file
     * @param newName new name of the virtual file system file
     */
    void setName(const string& newName);

    /**
     * Gets file stream of the virtual file system
     * @return file stream of the virtual file system
     */
    fstream* getVfsFile() const;

    /**
     * Sets file stream of the virtual file system
     * @param newVfsFile new file stream of the virtual file system
     */
    void setVfsFile(fstream* newVfsFile);

    /**
     * Prints information about the given directory item to the console in special format
     * @param item directory item to print information about
     */
    void printDirItemInfo(const DirectoryItem* item);

    /**
     * Prints information about the given indirect block to the console in special format
     * @param indirectBlockAddress address of the indirect block to print information about
     */
    void printIndirectBlocks(int32_t indirectBlockAddress, stringstream& ss);

    /**
     * Cleanup the virtual file system ( delete all directories, i-nodes, data bitmap, etc. ) and set isFormatted flag to false
     */
    void cleanup();

    /**
     * Findes free data blocks in the virtual file system and returns them
     * @param count number of blocks to find
     * @return vector of free data blocks
     */
    vector<int32_t> findFreeDataBlocks(int count);

    /**
     * Finds free i-node in the virtual file system and returns its id or -1 if there is no free i-node
     * @return free i-node id or -1 if there is no free i-node
     */
    int32_t findFreeInode();

    /**
     * Gets vector of data blocks of the given i-node in the virtual file system and returns it
     * @param nodeid id of the i-node
     * @param block_count number of blocks
     * @param rest rest of the blocks
     * @return vector of data blocks of the given i-node in the virtual file system
     */
    vector<int32_t> getDataBlocks(int32_t nodeid, int* block_count, int* rest);

    /**
     * Adds direct blocks of the given i-node in the virtual file system to the given vector
     * @param node i-node
     * @param blocks vector of blocks
     */
    void addDirectBlocks(const Inode& node, vector<int32_t>& blocks);

    /**
     * Adds indirect blocks of the given i-node in the virtual file system to the given vector
     * @param indirect_block_address address of the indirect block
     * @param blocks vector of blocks
     * @param max_blocks max number of blocks
     */
    void addIndirectBlocks(int32_t indirect_block_address, vector<int32_t>& blocks, int max_blocks);

    /**
     * Fills direct blocks of the given i-node in the virtual file system to the given vector
     * @param node i-node
     * @param blocks vector of blocks
     */
    void fillDirectBlocks(const Inode& node, vector<int32_t>& blocks);

    /**
     * Fills indirect blocks of the given i-node in the virtual file system to the given vector
     * @param node i-node
     * @param blocks vector of blocks
     * @param block_count number of blocks
     */
    void fillIndirectBlocks(const Inode& node, vector<int32_t>& blocks, int block_count);

    /**
     * Writes the given i-node to the virtual file system file
     * @param id id of the i-node
     */
    void writeInodeToVfs(int id);

    /**
     * Updates bitmap in the virtual file system file for the given directory item with the given value and data blocks
     * @param item directory item to update bitmap for
     * @param value value to update bitmap with
     * @param dataBlocks data blocks to update bitmap with
     */
    void updateBitmapInFile(DirectoryItem* item, int8_t value, vector<int32_t> const& dataBlocks);

    /**
     * Updates indirect blocks in bitmap in the virtual file system file for the given indirect block with the given value
     * @param indirectBlock indirect block to update in bitmap
     * @param value value to update indirect block in bitmap with
     */
    void updateIndirectBlocksInBitmap(int32_t indirectBlock, int8_t value);

    /**
     * Formats the virtual file system with the given size and returns true if the virtual file system was formatted successfully, false otherwise
     * @param filesystemSize size of the virtual file system
     * @return true if the virtual file system was formatted successfully, false otherwise
     */
    bool format(int32_t filesystemSize);

    /**
     * Writes superblock to the virtual file system file or throws an exception if the file is not open
     */
    void writeSuperblock();

    /**
     * Flushes the virtual file system file ( saves changes )
     */
    void flushVfs();

    /**
     * Frees the i-node with the given id in the virtual file system or throws an exception if the id is invalid ( initialized with ID_ITEM_FREE )
     * @param id id of the i-node to free
     */
    void freeInode(int id);

    /**
     * Seeks the data cluster for a given block number.
     * @param blockNumber The block number to seek.
     * @return 0 if successful, -1 on error.
     */
    int seekDataCluster(int blockNumber) const;

    /**
     * Seeks to a specific offset in the virtual file system file.
     * @param offset The offset to seek to.
     * @return 0 if successful, -1 on error.
     */
    int seekSet(long int offset) const;

    /**
     * Seeks to a specific offset relative to the current position in the file.
     * @param offset The offset to seek to.
     * @return 0 if successful, -1 on error.
     */
    int seekCur(long int offset);

    /**
     * Rewinds the virtual file system file to the beginning.
     */
    void rewindVfs() const;

    /**
     * Reads a value from a file and sets it in the superblock.
     * @tparam T The type of the value to read.
     * @param file The file to read from.
     * @param superblock The superblock to set the value in.
     * @param setter A pointer to a setter function in the superblock.
     */
    template<typename T> void readAndSet(fstream& file, Superblock& superblock, void(Superblock::*setter)(T));

    /**
     * Writes data to the virtual file system file.
     * @tparam T The type of data to write.
     * @param ptr A pointer to the data to write.
     * @param count The number of elements to write.
     * @return The number of elements written.
     */
    template<typename T> streamsize writeToFile(const T* ptr, size_t count = 1);

    /**
     * Reads data from the virtual file system file.
     * @tparam T The type of data to read.
     * @param ptr A pointer to the buffer to read data into.
     * @param count The number of elements to read.
     * @return The number of elements read.
     */
    template<typename T> streamsize readFromFile(T* ptr, size_t count = 1);

    /**
     * Creates directory in the virtual file system file with the given name in the given parent directory
     * @param dir directory to create
     * @param item directory item to create
     * @return true if the directory was created successfully, false otherwise
     */
    int createDirectoryInFile(Directory* dir, DirectoryItem* item);

    /**
     * Deletes a directory item from the virtual file system file, including updating the associated data blocks
     * and the directory's inode
     * @param dir The directory containing the item to remove.
     * @param item The directory item to be removed.
     * @return 0 if the item was successfully removed, -1 otherwise.
     */
    int removeDirectoryFromFile(Directory* dir, DirectoryItem* item);

    /**
     * Create or remove directory in file
     * @param dir pointer to directory
     * @param item pointer to directory item
     * @param create true if create, false if remove
     * @return 0 if success, -1 if error
     */
    int updateDirectoryInFile(Directory* dir, DirectoryItem* item, bool create);

    /**
     * Clear indirect blocks of inode by id
     * @param inodeId id of inode to clear indirect blocks
     */
    void clearIndirectBlocks(int32_t inodeId);

    /**
     * Remove file from directory and from file system ( including linking links )
     * @param parentDir directory where file is located
     * @param item file to remove
     * @return true if file was removed successfully, false otherwise
     */
    bool removeFile(Directory* parentDir, DirectoryItem* item);

    /**
     * Get parent directory of item (file or directory)
     * @param item item to find parent directory
     * @return parent directory of item
     */
    Directory* getParentDirectory(DirectoryItem* item);

    /**
     * Initializes i-node in the virtual file system with the given parameters
     * @param inodeId id of the i-node
     * @param size size of file
     * @param blockCount number of blocks
     * @param blocks vector of blocks
     * @return last data block
     */
    int32_t initializeInode(int32_t inode_id, int32_t size, int block_count, vector<int32_t>& blocks);

    /**
     * Updates the directory in the virtual file system file
     * @param dir directory to update in the virtual file system file
     * @param size size of the directory to update in the virtual file system file
     */
    void updateSizesInFile(Directory* dir, int32_t size);

    /**
     * Write inode to file
     * @param ptr pointer to inode with data to write
     */
    void writeInodeToFile(Inode* ptr);

    /**
     * Read inode from file
     * @param ptr pointer to inode the read data to store
     * @param count number of inodes to read
     */
    void readInodeFromFile(Inode* ptr, size_t count = 1);

    /**
     * Loads the directory from the virtual file system
     * @param dir directory to load
     * @param id id of the directory
     */
    void loadDirectoryFromVfs(Directory* dir, int id);

    /**
     * Loads the virtual file system from the file
     */
    void loadVfs();

    /**
     * Gets all directories in the virtual file system ( map<int, Directory*> )
     * @return all directories in the virtual file system ( map<int, Directory*> )
     */
    unordered_map<int, Directory*>& getAllDirectories();

    /**
     * Gets the directory with the given id in the virtual file system or nullptr if the id is invalid
     * @param id id of the directory
     * @return directory with the given id or nullptr if the id is invalid
     */
    Directory* getDirectory(int32_t id);

    /**
     * Adds the directory to the virtual file system
     * @param dir directory to add
     * @param index index of the directory
     */
    void addDirectory(Directory* dir, int32_t index);

    /**
     * Removees directory item from the virtual file system file by name
     * @param parentDir parent directory of the directory item to remove
     * @param name name of the directory item to remove
     * @return true if the directory item was removed successfully, false otherwise
     */
    bool removeDirectory(Directory* parentDir, const string& name);

private:
    Superblock* superblock;
    Inode* inodes;
    int8_t* dataBitmap;

    bool isFormatted;
    Directory* currentDir;
    unordered_map<int, Directory*> allDirs;

    string name;
    fstream* vfsFile;
};

#endif //SEMESTRALNIPRACE_VIRTUALFILESYSTEM_HPP