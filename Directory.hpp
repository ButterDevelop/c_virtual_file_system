#ifndef SEMESTRALNIPRACE_DIRECTORY_HPP
#define SEMESTRALNIPRACE_DIRECTORY_HPP

#include <string>
#include "DirectoryItem.hpp"

using std::string;

/**
 * Class representing a directory
 */
class Directory {
public:

    /**
     * Default constructor for directory class
     */
    Directory() = default;

    /**
     * Constructor for directory item
     * @param parent - parent directory
     * @param current - current directory
     * @param subdir - subdirectory
     * @param file - file
     */
    Directory(Directory* parent, DirectoryItem* current, DirectoryItem* subdir, DirectoryItem* file);

    /**
     * Destructor for directory class
     */
    ~Directory();

    /**
     * Gets the parent of the directory
     * @return parent of the directory
     */
    Directory* getParent() const;

    /**
     * Sets the parent of the directory
     * @param newParent - new parent
     */
    void setParent(Directory* parent);

    /**
     * Gets the current item of the directory
     * @return current item of the directory
     */
    DirectoryItem* getCurrent() const;

    /**
     * Sets the current item of the directory
     * @param newCurrent - new current item
     */
    void setCurrent(DirectoryItem* current);

    /**
     * Gets the subdirectory of the directory
     * @return subdirectory of the directory
     */
    DirectoryItem*& getSubdir();

    /**
     * Sets the subdirectory of the directory
     * @param newSubdir - new subdirectory
     */
    void setSubdir(DirectoryItem* subdir);

    /**
     * Gets the file of the directory
     * @return file of the directory
     */
    DirectoryItem*& getFile();

    /**
     * Sets the file of the directory
     * @param newFile - new file
     */
    void setFile(DirectoryItem* file);

    /**
     * Adds a subdirectory to the directory
     * @param item - subdirectory to add
     */
    void addSubdirectory(DirectoryItem* item);

    /**
     * Adds a file to the directory
     * @param item - file to add
     */
    void addFile(DirectoryItem* item);

    /**
     * Deletes a file from the directory
     * @param fileName - name of the file to delete
     * @return deleted file or nullptr if the file was not found
     */
    DirectoryItem* deleteFileFromDirectory(const string& fileName);

private:
    Directory* parent;
    DirectoryItem* current;
    DirectoryItem* subdir;
    DirectoryItem* file;
};

#endif //SEMESTRALNIPRACE_DIRECTORY_HPP
