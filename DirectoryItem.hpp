#ifndef SEMESTRALNIPRACE_DIRECTORYITEM_HPP
#define SEMESTRALNIPRACE_DIRECTORYITEM_HPP

#include <cstdint>
#include "Constants.hpp"

/**
 * Class representing a directory item
 */
class DirectoryItem {
public:
    /**
     * Default constructor for directory item
     */
    DirectoryItem(int32_t inodeId, const char* itemName);

    /**
     * Copy constructor
     * @param other - other directory item
     */
    DirectoryItem(const DirectoryItem& other); // Copy constructor

    /**
     * Copy assignment operator
     * @param other - other directory item
     * @return reference to this directory item
     */
    DirectoryItem& operator=(const DirectoryItem& other); // Copy assignment operator

    /**
     * Destructor for directory item
     */
    ~DirectoryItem(); // Destructor

    /**
     * Gets the inode id of the item
     * @return inode id of the item
     */
    int32_t getInode() const;

    /**
     * Sets the inode id of the item
     * @param inodeId - inode id of the item
     */
    void setInode(int32_t inodeId);

    /**
     * Gets the name of the item
     * @return name of the item
     */
    const char* getItemName() const;

    /**
     * Sets the name of the item
     * @param itemName - name of the item
     */
    void setItemName(const char* itemName);

    /**
     * Gets the next item
     * @return next item
     */
    DirectoryItem* getNext() const;

    /**
     * Sets the next item
     * @param nextItem - next item
     */
    void setNext(DirectoryItem* nextItem);

    /**
     * Gets the reference to the next item
     * @return reference to the next item
     */
    DirectoryItem*& getNextRef();

private:
    int32_t inode;
    char itemName[12];
    DirectoryItem* next;
};

/**
 * Creates a new directory item
 * @param inodeId - inode id of the item
 * @param name - name of the item
 * @return new directory item
 */
DirectoryItem* createDirectoryItem(int32_t inodeId, const char* name);

/**
 * Finds an item in a linked list of directory items by name
 * @param firstItem - first item in the linked list
 * @param name - name of the item to find
 * @return found item or nullptr if the item was not found
 */
DirectoryItem* findItem(DirectoryItem* firstItem, const char* name);

/**
 * Finds an item in a linked list of directory items by inode id
 * @param firstItem - first item in the linked list
 * @param inodeId - inode id of the item to find
 * @return found item or nullptr if the item was not found
 */
DirectoryItem* findItemByInodeId(DirectoryItem* firstItem, int32_t inodeId);

#endif //SEMESTRALNIPRACE_DIRECTORYITEM_HPP
