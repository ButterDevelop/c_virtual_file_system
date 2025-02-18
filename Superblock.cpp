#include "Superblock.hpp"
#include "Inode.hpp"
#include "Constants.hpp"
#include <cstring>
#include <cmath>

using std::strncpy;


Superblock::Superblock()
        : signature(nullptr), diskSize(0), clusterSize(CLUSTER_SIZE),
          clusterCount(0), inodeCount(0), bitmapClusterCount(0),
          inodeClusterCount(0), dataClusterCount(0), bitmapStartAddress(0),
          inodeStartAddress(0), dataStartAddress(0) {
    signature = new char[SIGNATURE_LENGTH + 1];
    strncpy(signature, SIGNATURE, SIGNATURE_LENGTH);
    signature[SIGNATURE_LENGTH] = '\0';
}

Superblock::Superblock(int32_t diskSize)
        : Superblock() {
    this->diskSize = diskSize;
    clusterCount = diskSize / CLUSTER_SIZE;
    inodeClusterCount = clusterCount / 20;
    inodeCount = (inodeClusterCount * CLUSTER_SIZE) / INODE_SIZE;
    bitmapClusterCount = static_cast<int32_t>(ceil((clusterCount - inodeClusterCount - 1) / static_cast<float>(CLUSTER_SIZE)));
    dataClusterCount = clusterCount - 1 - bitmapClusterCount - inodeClusterCount;
    bitmapStartAddress = CLUSTER_SIZE;
    inodeStartAddress = bitmapStartAddress + CLUSTER_SIZE * bitmapClusterCount;
    dataStartAddress = inodeStartAddress + CLUSTER_SIZE * inodeClusterCount;
}

Superblock::Superblock(const Superblock& other)
        : diskSize(other.diskSize), clusterSize(other.clusterSize),
          clusterCount(other.clusterCount), inodeCount(other.inodeCount),
          bitmapClusterCount(other.bitmapClusterCount), inodeClusterCount(other.inodeClusterCount),
          dataClusterCount(other.dataClusterCount), bitmapStartAddress(other.bitmapStartAddress),
          inodeStartAddress(other.inodeStartAddress), dataStartAddress(other.dataStartAddress),
          signature(new char[SIGNATURE_LENGTH + 1]) {
    strcpy(signature, other.signature);
}

Superblock& Superblock::operator=(const Superblock& other) {
    if (this != &other) {
        delete[] signature;
        signature = new char[SIGNATURE_LENGTH + 1];
        strcpy(signature, other.signature);
        diskSize = other.diskSize;
        clusterSize = other.clusterSize;
        clusterCount = other.clusterCount;
        inodeCount = other.inodeCount;
        bitmapClusterCount = other.bitmapClusterCount;
        inodeClusterCount = other.inodeClusterCount;
        dataClusterCount = other.dataClusterCount;
        bitmapStartAddress = other.bitmapStartAddress;
        inodeStartAddress = other.inodeStartAddress;
        dataStartAddress = other.dataStartAddress;
    }
    return *this;
}

Superblock::~Superblock() {
    delete[] signature;
}

const char* Superblock::getSignature() const {
    return signature;
}

/**
 * Sets signature
 *
 * @param newSignature - new signature
 */
void Superblock::setSignature(const char* newSignature) {
    if (newSignature) {
        strncpy(signature, newSignature, SIGNATURE_LENGTH);
        signature[SIGNATURE_LENGTH] = '\0';
    }
}

/**
 * Gets disk size
 *
 * @return disk size
 */
int32_t Superblock::getDiskSize() const { return diskSize; }

/**
 * Sets disk size
 *
 * @param newDiskSize - new disk size
 */
void Superblock::setDiskSize(int32_t newDiskSize) { diskSize = newDiskSize; }

/**
 * Gets cluster size
 *
 * @return cluster size
 */
int32_t Superblock::getClusterSize() const { return clusterSize; }

/**
 * Sets cluster size
 *
 * @param newClusterSize - new cluster size
 */
void Superblock::setClusterSize(int32_t newClusterSize) { clusterSize = newClusterSize; }

/**
 * Gets cluster count
 *
 * @return cluster count
 */
int32_t Superblock::getClusterCount() const { return clusterCount; }

/**
 * Sets cluster count
 *
 * @param newClusterCount - new cluster count
 */
void Superblock::setClusterCount(int32_t newClusterCount) { clusterCount = newClusterCount; }

/**
 * Gets inode count
 *
 * @return inode count
 */
int32_t Superblock::getInodeCount() const { return inodeCount; }

/**
 * Sets inode count
 *
 * @param newInodeCount - new inode count
 */
void Superblock::setInodeCount(int32_t newInodeCount) { inodeCount = newInodeCount; }

/**
 * Gets bitmap cluster count
 *
 * @return bitmap cluster count
 */
int32_t Superblock::getBitmapClusterCount() const { return bitmapClusterCount; }

/**
 * Sets bitmap cluster count
 *
 * @param newBitmapClusterCount - new bitmap cluster count
 */
void Superblock::setBitmapClusterCount(int32_t newBitmapClusterCount) { bitmapClusterCount = newBitmapClusterCount; }

/**
 * Gets inode cluster count
 *
 * @return inode cluster count
 */
int32_t Superblock::getInodeClusterCount() const { return inodeClusterCount; }

/**
 * Sets inode cluster count
 *
 * @param newInodeClusterCount - new inode cluster count
 */
void Superblock::setInodeClusterCount(int32_t newInodeClusterCount) { inodeClusterCount = newInodeClusterCount; }

/**
 * Gets data cluster count
 *
 * @return data cluster count
 */
int32_t Superblock::getDataClusterCount() const { return dataClusterCount; }

/**
 * Sets data cluster count
 *
 * @param newDataClusterCount - new data cluster count
 */
void Superblock::setDataClusterCount(int32_t newDataClusterCount) { dataClusterCount = newDataClusterCount; }

/**
 * Gets bitmap start address
 *
 * @return bitmap start address
 */
int32_t Superblock::getBitmapStartAddress() const { return bitmapStartAddress; }

/**
 * Sets bitmap start address
 *
 * @param newBitmapStartAddress - new bitmap start address
 */
void Superblock::setBitmapStartAddress(int32_t newBitmapStartAddress) { bitmapStartAddress = newBitmapStartAddress; }

/**
 * Gets inode start address
 *
 * @return inode start address
 */
int32_t Superblock::getInodeStartAddress() const { return inodeStartAddress; }

/**
 * Sets inode start address
 *
 * @param newInodeStartAddress - new inode start address
 */
void Superblock::setInodeStartAddress(int32_t newInodeStartAddress) { inodeStartAddress = newInodeStartAddress; }

/**
 * Gets data start address
 *
 * @return data start address
 */
int32_t Superblock::getDataStartAddress() const { return dataStartAddress; }

/**
 * Sets data start address
 *
 * @param newDataStartAddress - new data start address
 */
void Superblock::setDataStartAddress(int32_t newDataStartAddress) { dataStartAddress = newDataStartAddress; }


Superblock* superblockInit(int32_t disk_size) {
    return new Superblock(disk_size);
}
