#include "Inode.hpp"
#include "Constants.hpp"

using std::invalid_argument;

int32_t Inode::getNodeId() const {
    return nodeId;
}

void Inode::setNodeId(int32_t nodeId) {
    this->nodeId = nodeId;
}

bool Inode::getIsDirectory() const {
    return isDirectory;
}

void Inode::setIsDirectory(bool isDirectory) {
    this->isDirectory = isDirectory;
}

int8_t Inode::getReferences() const {
    return references;
}

void Inode::setReferences(int8_t references) {
    this->references = references;
}

int32_t Inode::getFileSize() const {
    return fileSize;
}

void Inode::setFileSize(int32_t fileSize) {
    this->fileSize = fileSize;
}

int32_t Inode::getDirect(int index) const {
    if (index >= 0 && index <= 4) {
        return direct[index];
    }

    throw invalid_argument(THE_INDEX_VALUE_HAS_TO_BE_BETWEEN_0_AND_4_TEXT);
}

void Inode::setDirect(int index, int32_t value) {
    if (index >= 0 && index <= 4) {
        direct[index] = value;
    } else {
        throw invalid_argument(THE_INDEX_VALUE_HAS_TO_BE_BETWEEN_0_AND_4_TEXT);
    }
}

int32_t Inode::getIndirect(int index) const {
    if (index >= 0 && index <= 1) {
        return indirect[index];
    }

    throw invalid_argument(THE_INDEX_VALUE_HAS_TO_BE_BETWEEN_0_AND_1_TEXT);
}

void Inode::setIndirect(int index, int32_t value) {
    if (index >= 0 && index <= 1) {
        indirect[index] = value;
    } else {
        throw invalid_argument(THE_INDEX_VALUE_HAS_TO_BE_BETWEEN_0_AND_1_TEXT);
    }

}