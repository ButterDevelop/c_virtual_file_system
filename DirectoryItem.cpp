#include "DirectoryItem.hpp"
#include <cstring>

using std::strncpy;
using std::strcmp;

DirectoryItem::DirectoryItem(int32_t inodeId, const char* itemName)
        : inode(inodeId), next(nullptr) {
    strncpy(this->itemName, itemName, 11);
    this->itemName[11] = '\0'; // Ensure null termination
}

DirectoryItem::DirectoryItem(const DirectoryItem& other)
        : inode(other.inode), next(other.next) {
    strcpy(this->itemName, other.itemName);
}

DirectoryItem& DirectoryItem::operator=(const DirectoryItem& other) {
    if (this != &other) {
        inode = other.inode;
        strcpy(this->itemName, other.itemName);
        next = other.next;
    }
    return *this;
}

DirectoryItem::~DirectoryItem() {
    // No dynamic memory to delete
}

int32_t DirectoryItem::getInode() const {
    return inode;
}

void DirectoryItem::setInode(int32_t inodeId) {
    inode = inodeId;
}

const char* DirectoryItem::getItemName() const {
    return itemName;
}

void DirectoryItem::setItemName(const char* itemName) {
    strncpy(this->itemName, itemName, 11);
    this->itemName[11] = '\0';
}

DirectoryItem* DirectoryItem::getNext() const {
    return next;
}

DirectoryItem*& DirectoryItem::getNextRef() {
    return next;
}

void DirectoryItem::setNext(DirectoryItem* nextItem) {
    next = nextItem;
}

DirectoryItem* createDirectoryItem(int32_t inodeId, const char* name) {
    return new DirectoryItem(inodeId, name);
}

DirectoryItem* findItem(DirectoryItem* firstItem, const char* name) {
    DirectoryItem* currentItem = firstItem;
    while (currentItem != nullptr) {
        if (strcmp(name, currentItem->getItemName()) == 0) {
            return currentItem;
        }
        currentItem = currentItem->getNext();
    }
    return nullptr;
}

DirectoryItem* findItemByInodeId(DirectoryItem* firstItem, int32_t inodeId) {
    DirectoryItem* currentItem = firstItem;
    while (currentItem != nullptr) {
        if (inodeId == currentItem->getInode()) {
            return currentItem;
        }
        currentItem = currentItem->getNext();
    }
    return nullptr;
}