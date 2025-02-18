#ifndef SEMESTRALNIPRACE_INODE_HPP
#define SEMESTRALNIPRACE_INODE_HPP

#include <cstdint>
#include <stdexcept>

/**
 * Class representing an inode
 */
class Inode {
public:

    /**
     * Default constructor for inode class
     */
    Inode() = default;

    /**
     * Gets the node id of the inode
     * @return node id of the inode
     */
    int32_t getNodeId() const;

    /**
     * Sets the node id of the inode
     * @param nodeId - new node id
     */
    void setNodeId(int32_t nodeId);

    /**
     * Gets the directory flag
     * @return directory flag
     */
    bool getIsDirectory() const;

    /**
     * Sets the directory flag
     * @param isDirectory - new value of the directory flag
     */
    void setIsDirectory(bool isDirectory);

    /**
     * Gets the references count
     * @return references count
     */
    int8_t getReferences() const;

    /**
     * Sets the references count
     * @param references - new value of the references count
     */
    void setReferences(int8_t references);

    /**
     * Gets the file size
     * @return file size
     */
    int32_t getFileSize() const;

    /**
     * Sets the file size
     * @param fileSize - new file size
     */
    void setFileSize(int32_t fileSize);

    /**
     * Gets the direct pointer at the given index, if the index is valid (0-4)
     * @param index - index of the direct pointer
     * @return direct pointer at the given index
     */
    int32_t getDirect(int index) const;

    /**
     * Sets the direct pointer at the given index, if the index is valid (0-4)
     * @param index - index of the direct pointer
     * @param value - new value of the direct pointer
     */
    void setDirect(int index, int32_t value);

    /**
     * Gets the indirect pointer at the given index, if the index is valid (0-1)
     * @param index - index of the indirect pointer
     * @return indirect pointer at the given index
     */
    int32_t getIndirect(int index) const;

    /**
     * Sets the indirect pointer at the given index, if the index is valid (0-1)
     * @param index - index of the indirect pointer
     * @param value - new value of the indirect pointer
     */
    void setIndirect(int index, int32_t value);

private:
    int32_t nodeId;
    bool isDirectory;
    int8_t references;
    int32_t fileSize;
    int32_t direct[5];
    int32_t indirect[2];
};

#endif //SEMESTRALNIPRACE_INODE_HPP
