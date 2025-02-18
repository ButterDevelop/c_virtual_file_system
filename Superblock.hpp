#ifndef SEMESTRALNIPRACE_SUPERBLOCK_HPP
#define SEMESTRALNIPRACE_SUPERBLOCK_HPP


#include <string>
#include <cmath>
#include "Constants.hpp"

using std::string;


class Superblock {
public:

    /**
     * Constructor for superblock
     */
    Superblock();

    /**
     * Constructor for superblock
     *
     * @param diskSize - size of disk in bytes
     */
    Superblock(int32_t diskSize);

    /**
     * Copy constructor
     *
     * @param other - other superblock
     */
    Superblock(const Superblock& other);

    /**
     * Assignment operator
     *
     * @param other - other superblock
     * @return this superblock
     */
    Superblock& operator=(const Superblock& other);

    /**
     * Destructor
     */
    ~Superblock();

    /**
     * Gets signature
     *
     * @return signature
     */
    const char* getSignature() const;

    /**
     * Sets signature
     *
     * @param signature - new signature
     */
    void setSignature(const char* signature);

    /**
     * Gets disk size
     *
     * @return disk size
     */
    int32_t getDiskSize() const;

    /**
     * Sets disk size
     *
     * @param diskSize - new disk size
     */
    void setDiskSize(int32_t diskSize);

    /**
     * Gets cluster size
     *
     * @return cluster size
     */
    int32_t getClusterSize() const;

    /**
     * Sets cluster size
     *
     * @param clusterSize - new cluster size
     */
    void setClusterSize(int32_t clusterSize);

    /**
     * Gets cluster count
     *
     * @return cluster count
     */
    int32_t getClusterCount() const;

    /**
     * Sets cluster count
     *
     * @param clusterCount - new cluster count
     */
    void setClusterCount(int32_t clusterCount);

    /**
     * Gets inode count
     *
     * @return inode count
     */
    int32_t getInodeCount() const;

    /**
     * Sets inode count
     *
     * @param inodeCount - new inode count
     */
    void setInodeCount(int32_t inodeCount);

    /**
     * Gets bitmap cluster count
     *
     * @return bitmap cluster count
     */
    int32_t getBitmapClusterCount() const;

    /**
     * Sets bitmap cluster count
     *
     * @param bitmapClusterCount - new bitmap cluster count
     */
    void setBitmapClusterCount(int32_t bitmapClusterCount);

    /**
     * Gets inode cluster count
     *
     * @return inode cluster count
     */
    int32_t getInodeClusterCount() const;

    /**
     * Sets inode cluster count
     *
     * @param inodeClusterCount - new inode cluster count
     */
    void setInodeClusterCount(int32_t inodeClusterCount);

    /**
     * Gets data cluster count
     *
     * @return data cluster count
     */
    int32_t getDataClusterCount() const;

    /**
     * Sets data cluster count
     *
     * @param dataClusterCount - new data cluster count
     */
    void setDataClusterCount(int32_t dataClusterCount);

    /**
     * Gets bitmap start address
     *
     * @return bitmap start address
     */
    int32_t getBitmapStartAddress() const;

    /**
     * Sets bitmap start address
     *
     * @param bitmapStartAddress - new bitmap start address
     */
    void setBitmapStartAddress(int32_t bitmapStartAddress);

    /**
     * Gets inode start address
     *
     * @return inode start address
     */
    int32_t getInodeStartAddress() const;

    /**
     * Sets inode start address
     *
     * @param inodeStartAddress - new inode start address
     */
    void setInodeStartAddress(int32_t inodeStartAddress);

    /**
     * Gets data start address
     *
     * @return data start address
     */
    int32_t getDataStartAddress() const;

    /**
     * Sets data start address
     *
     * @param dataStartAddress - new data start address
     */
    void setDataStartAddress(int32_t dataStartAddress);

private:
    char* signature;
    int32_t diskSize;
    int32_t clusterSize;
    int32_t clusterCount;
    int32_t inodeCount;
    int32_t bitmapClusterCount;
    int32_t inodeClusterCount;
    int32_t dataClusterCount;
    int32_t bitmapStartAddress;
    int32_t inodeStartAddress;
    int32_t dataStartAddress;
};

/**
 * Initializes superblock
 *
 * @param disk_size - size of disk in bytes
 * @return pointer to superblock
 */
Superblock* superblockInit(int32_t disk_size);

#endif //SEMESTRALNIPRACE_SUPERBLOCK_HPP
