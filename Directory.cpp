#include "Directory.hpp"

using std::string;

Directory::Directory(Directory* parent, DirectoryItem* current, DirectoryItem* subdir, DirectoryItem* file)
        : parent(parent), current(current), subdir(subdir), file(file) {}

Directory* Directory::getParent() const {
    return parent;
}


Directory::~Directory() {
    // Helper lambda to delete all items in a linked list
    auto deleteAllItems = [](DirectoryItem* item) {
        while (item != nullptr) {
            DirectoryItem* nextItem = item->getNext();
            delete item; // Delete the current item
            item = nextItem;
        }
    };

    // Delete all files
    deleteAllItems(file);

    // Delete all subdirectories
    deleteAllItems(subdir);
}

void Directory::setParent(Directory* newParent) {
    parent = newParent;
}

DirectoryItem* Directory::getCurrent() const {
    return current;
}

void Directory::setCurrent(DirectoryItem* newCurrent) {
    current = newCurrent;
}

DirectoryItem*& Directory::getSubdir() {
    return subdir;
}

void Directory::setSubdir(DirectoryItem* newSubdir) {
    subdir = newSubdir;
}

DirectoryItem*& Directory::getFile() {
    return file;
}

void Directory::setFile(DirectoryItem* newFile) {
    file = newFile;
}

void Directory::addSubdirectory(DirectoryItem* item) {
    if (subdir == nullptr) {
        subdir = item;
    } else {
        DirectoryItem* current = subdir;
        while (current->getNext() != nullptr) {
            current = current->getNext(); // Go to the last item
        }
        current->setNext(item); // Add the new item to the end of the list
    }
}

void Directory::addFile(DirectoryItem* item) {
    if (file == nullptr) {
        file = item;
    } else {
        DirectoryItem* current = file;
        while (current->getNext() != nullptr) {
            current = current->getNext(); // Go to the last item
        }
        current->setNext(item); // Add the new item to the end of the list
    }
}

DirectoryItem* Directory::deleteFileFromDirectory(const string& fileName) {
    // Search for the file in the directory
    DirectoryItem* item = file;
    DirectoryItem** prevItem = &file;
    bool fileFound = false;
    while (item != nullptr) {
        if (item->getItemName() == fileName) {
            fileFound = true;
            break;
        }
        prevItem = &(item->getNextRef()); // Get the reference to the next item
        item = item->getNext(); // Go to the next item
    }

    if (!fileFound) {
        return nullptr;
    }

    // Remove the item from the list
    *prevItem = item->getNext();

    return item;
}