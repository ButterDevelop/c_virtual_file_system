#include "Constants.hpp"

using std::string;

const char* SIGNATURE            = "Maxim Lyutikov";
const int   SIGNATURE_LENGTH     = 20;

const int CLUSTER_SIZE           = 4096;
const int INT32_COUNT_IN_BLOCK   = CLUSTER_SIZE / 4;
const int FILENAME_LENGTH        = 12;
const int NEGATIVE_SIZE_OF_INT32 = -4;
const int INODE_SIZE             = 38;
const int ID_ITEM_FREE           = -1;

const int ERROR_CODE             = -1;
const int NO_ERROR_CODE          = 0;

const string HELP_COMMAND        = "help";
const string CP_COMMAND          = "cp";
const string MV_COMMAND          = "mv";
const string RM_COMMAND          = "rm";
const string MKDIR_COMMAND       = "mkdir";
const string RMDIR_COMMAND       = "rmdir";
const string LS_COMMAND          = "ls";
const string CAT_COMMAND         = "cat";
const string CD_COMMAND          = "cd";
const string PWD_COMMAND         = "pwd";
const string INFO_COMMAND        = "info";
const string INCP_COMMAND        = "incp";
const string OUTCP_COMMAND       = "outcp";
const string LOAD_COMMAND        = "load";
const string FORMAT_COMMAND      = "format";
const string HARDLINK_COMMAND    = "ln";
const string EXIT_COMMAND        = "exit";
const string QUIT_COMMAND        = "quit";



const string UNKNOWN_COMMAND_TEXT                           = "Unknown command : ";
const string PROGRAM_INTRODUCTIONS_TEXT                     = "Semestral work I-node file system";
const string THE_INDEX_VALUE_HAS_TO_BE_BETWEEN_0_AND_4_TEXT = "The index value has to be between 0 and 4";
const string THE_INDEX_VALUE_HAS_TO_BE_BETWEEN_0_AND_1_TEXT = "The index value has to be between 0 and 1";
const string NO_FREE_INODES_TEXT                            = "No free inodes found. Probably need more space.";
const string END_OF_PROGRAM_TEXT                            = "Goodbye.";
const string PLEASE_FORMAT_VFS_TEXT                         = "Please, format VFS! You are in limited mode for now!";
const string FILE_NOT_FOUND_TEXT                            = "File was not found!";
const string FILE_COMPLETE_TEXT                             = "File was read successfully!";
const string PATH_NOT_FOUND_TEXT                            = "Path was not found!";
const string WRONG_NUMBER_OF_ARGS_TEXT                      = "Wrong number of arguments for this command!";
const string PARENT_DIR_NOT_EXISTS_TEXT                     = "Parent directory does not exist!";
const string DIRECTORY_ALREADY_EXISTS_TEXT                  = "The directory with this name already exists!";
const string DIRECTORY_NOT_FOUND_WITH_NAME_TEXT             = "Directory was not found :";
const string ERROR_PARSING_SIZE_STRING_TEXT                 = "Error parsing size string : ";
const string DIRECTORY_NOT_FOUND_IN_VFS_TEXT                = "Directory was not found in VFS : ";
const string FILE_NOT_FOUND_IN_VFS_TEXT                     = "File was not found in VFS : ";
const string COULD_NOT_OPEN_FILE_ON_HARD_DISK_FOR_WRITING   = "Could not open file on hard disk for writing : ";
const string SOURCE_DIR_NOT_FOUND_TEXT                      = "Source directory was not found!";
const string SOURCE_FILE_NOT_FOUND_TEXT                     = "Source file was not found!";
const string DIRECTORY_NOT_FOUND_TEXT                       = "Directory was not found!";
const string DESTINATION_DIR_NOT_FOUND_TEXT                 = "Destination directory was not found!";
const string DESTINATION_PATH_NOT_FOUND_TEXT                = "Destination path was not found!";
const string FIlENAME_IS_TOO_LONG_TEXT                      = "Filename is too long!";
const string ITEM_NOT_FOUND_TEXT                            = "Item was not found : ";
const string USAGE_INFO_TEXT                                = "Usage: info <path>";
const string PROGRAM_ERROR_EXIT_TEXT                        = "Incorrect parameters! End of program!";
const string LOADING_FILE_TEXT                              = "Loading file name : ";
const string FILE_ALREADY_EXISTS_TEXT                       = "File with this name already exists!";
const string FILE_ALREADY_EXISTS_IN_DESTINATION_DIR_TEXT    = "File with this name already exists in destination directory!";
const string NOT_ENOUGH_SPACE_BLOCKS_TEXT                   = "Not enough data blocks found. Probably need more space.";
const string FILE_COPIED_SECCESSFULLY_TEXT                  = "File copied successfully!";
const string FILE_SUCESSFULLY_COPIED_FROM_VFS_TO_TEXT       = "File copied successfully from VFS to : ";
const string TARGET_DIR_NOT_FOUND_TEXT                      = "Target directory was not found!";
const string FORMAT_SUCCESSFUL_TEXT                         = "VFS formatted successfully!";
const string FORMAT_ERROR_TEXT                              = "VFS format error!";
const string OK_TEXT                                        = "Em... OK";
const string COMMAND_IS_NOT_AVAILABLE_TEXT                  = "You are using program in limited mode. This command is not available.";
const string DIR_NOT_FOUND_OR_NOT_EMPTY_TEXT                = "Directory was not found or not empty!";
const string NUMBER_PROBABLY_IS_WRONG                       = "This number is probably wrong!";

const string PATH_DELIMETER         = "/";
const string M_SIZE                 = "M";
const string K_SIZE                 = "K";
const string G_SIZE                 = "G";

const bool IS_DEBUG = true;