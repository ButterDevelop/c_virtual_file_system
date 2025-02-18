#ifndef SEMESTRALNIPRACE_UTILS_HPP
#define SEMESTRALNIPRACE_UTILS_HPP

#include <string>
#include <vector>

class VirtualFileSystem;
class Directory;
class DirectoryItem;

using std::string;
using std::vector;

/**
 * Prints message to standard output if IS_DEBUG is true
 * @param message
 * @param endLine
 */
void log(const string& message, bool endLine = true);

/**
 * Gets line from standard input
 * @return line from standard input
 */
string getLine();

/**
 * Removes end of line characters from string
 * @param message
 * @return string without end of line characters
 */
string removeEndOfLine(const string& message);

/**
 * Converts size string (e.g. 10K, 10M, 10G) to int (10000, 10000000, 10000000000)
 * @param stringSize
 * @return size in bytes
 */
int32_t getSizeFromString(const string& stringSize);

/**
 * Trims string from start
 * @param s string to trim
 * @return trimmed string
 */
string &ltrim(string &s);

/**
 * Trims string from end
 * @param s string to trim
 * @return trimmed string
 */
string &rtrim(string &s);

/**
 * Trims string from both ends
 * @param s string to trim
 * @return trimmed string
 */
string &trim(string &s);

/**
 * Trims string from start (copying)
 * @param s string to trim
 * @return trimmed string
 */
string trim_copy(string s);

/**
 * Gets directory path from full path
 * @param fullPath full path
 * @return directory path
 */
string getDirPath(const string& fullPath);

/**
 * Gets file name from full path
 * @param fullPath full path
 * @return file name
 */
string getFileName(const string& fullPath);

/**
 * Converts int to string
 * @param number int to convert
 * @return string representation of int
 */
string intToString(int32_t number);

/**
 * Splits string by delimiter
 * @param str string to split
 * @param delim delimiter
 * @return vector of strings
 */
vector<string> splitString(const string& str, char delim = ' ');

#endif