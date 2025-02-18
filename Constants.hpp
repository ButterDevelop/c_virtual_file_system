#ifndef SEMESTRALNIPRACE_CONSTANTS_HPP
#define SEMESTRALNIPRACE_CONSTANTS_HPP

#include <string>

using std::string;

extern const char* SIGNATURE;
extern const int SIGNATURE_LENGTH;

extern const int CLUSTER_SIZE;
extern const int INT32_COUNT_IN_BLOCK;
extern const int FILENAME_LENGTH;
extern const int NEGATIVE_SIZE_OF_INT32;
extern const int INODE_SIZE;
extern const int ID_ITEM_FREE;

extern const int ERROR_CODE;
extern const int NO_ERROR_CODE;

extern const string HELP_COMMAND;
extern const string CP_COMMAND;
extern const string MV_COMMAND;
extern const string RM_COMMAND;
extern const string MKDIR_COMMAND;
extern const string RMDIR_COMMAND;
extern const string LS_COMMAND;
extern const string CAT_COMMAND;
extern const string CD_COMMAND;
extern const string PWD_COMMAND;
extern const string INFO_COMMAND;
extern const string INCP_COMMAND;
extern const string OUTCP_COMMAND;
extern const string LOAD_COMMAND;
extern const string FORMAT_COMMAND;
extern const string HARDLINK_COMMAND;
extern const string EXIT_COMMAND;
extern const string QUIT_COMMAND;

extern const string PROGRAM_INTRODUCTIONS_TEXT;
extern const string PROGRAM_ERROR_EXIT_TEXT;
extern const string LOADING_FILE_TEXT;
extern const string UNKNOWN_COMMAND_TEXT;
extern const string END_OF_PROGRAM_TEXT;
extern const string PLEASE_FORMAT_VFS_TEXT;
extern const string FILE_NOT_FOUND_TEXT;
extern const string NO_FREE_INODES_TEXT;
extern const string SOURCE_FILE_NOT_FOUND_TEXT;
extern const string FILE_COMPLETE_TEXT;
extern const string FILE_COPIED_SECCESSFULLY_TEXT;
extern const string FILE_SUCESSFULLY_COPIED_FROM_VFS_TO_TEXT;
extern const string TARGET_DIR_NOT_FOUND_TEXT;
extern const string PATH_NOT_FOUND_TEXT;
extern const string WRONG_NUMBER_OF_ARGS_TEXT;
extern const string PARENT_DIR_NOT_EXISTS_TEXT;
extern const string DIRECTORY_ALREADY_EXISTS_TEXT;
extern const string DIRECTORY_NOT_FOUND_TEXT;
extern const string DIRECTORY_NOT_FOUND_WITH_NAME_TEXT;
extern const string ERROR_PARSING_SIZE_STRING_TEXT;
extern const string DIRECTORY_NOT_FOUND_IN_VFS_TEXT;
extern const string FILE_NOT_FOUND_IN_VFS_TEXT;
extern const string COULD_NOT_OPEN_FILE_ON_HARD_DISK_FOR_WRITING;
extern const string DESTINATION_PATH_NOT_FOUND_TEXT;
extern const string FIlENAME_IS_TOO_LONG_TEXT;
extern const string ITEM_NOT_FOUND_TEXT;
extern const string USAGE_INFO_TEXT;
extern const string SOURCE_DIR_NOT_FOUND_TEXT;
extern const string DESTINATION_DIR_NOT_FOUND_TEXT;
extern const string FILE_ALREADY_EXISTS_TEXT;
extern const string FILE_ALREADY_EXISTS_IN_DESTINATION_DIR_TEXT;
extern const string NOT_ENOUGH_SPACE_BLOCKS_TEXT;
extern const string FORMAT_SUCCESSFUL_TEXT;
extern const string FORMAT_ERROR_TEXT;
extern const string OK_TEXT;
extern const string COMMAND_IS_NOT_AVAILABLE_TEXT;
extern const string DIR_NOT_FOUND_OR_NOT_EMPTY_TEXT;
extern const string NUMBER_PROBABLY_IS_WRONG;
extern const string THE_INDEX_VALUE_HAS_TO_BE_BETWEEN_0_AND_4_TEXT;
extern const string THE_INDEX_VALUE_HAS_TO_BE_BETWEEN_0_AND_1_TEXT;

extern const string PATH_DELIMETER;
extern const string M_SIZE;
extern const string G_SIZE;
extern const string K_SIZE;

extern const bool IS_DEBUG;

#endif //SEMESTRALNIPRACE_CONSTANTS_HPP