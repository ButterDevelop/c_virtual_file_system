
#ifndef SEMESTRALNIPRACE_COMMANDPROCESSOR_HPP
#define SEMESTRALNIPRACE_COMMANDPROCESSOR_HPP

#include <string>
#include <unordered_map>
#include <set>
#include <functional>
#include "VirtualFileSystem.hpp"
#include "Constants.hpp"

using std::string;
using std::vector;
using std::set;
using std::unordered_map;
using std::function;

class CommandProcessor {
public:
    /**
     * Constructor for command processor
     * Possible commands:
     * help          --    Display this helpful text
     * cp s1 s2      --    Copy file from path s1 to path s2. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND
     * mv s1 s2      --    Move or rename file from path s1 to path s2. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND
     * rm s1         --    Delete file s1. Possible results: OK, FILE NOT FOUND
     * mkdir a1      --    Create directory a1. Possible results: OK, PATH NOT FOUND, EXIST
     * rmdir a1      --    Delete empty directory a1. Possible results: OK, FILE NOT FOUND, NOT EMPTY
     * ls a1         --    List contents of directory a1 or current directory if a1 is omitted. Possible results: -FILE, +DIRECTORY, PATH NOT FOUND
     * cat s1        --    Display contents of file s1. Possible results: CONTENT, FILE NOT FOUND
     * cd a1         --    Change current path to directory a1. Possible results: OK, PATH NOT FOUND
     * pwd           --    Display current path. Possible results: PATH
     * info s1/a1    --    Display information about file/directory s1/a1 (i-node number, direct and indirect links). Possible results: NAME – SIZE – i-node NUMBER, FILE NOT FOUND
     * incp s1 s2    --    Upload file s1 from hard disk to path s2 in your FS. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND
     * outcp s1 s2   --    Upload file s1 from your FS to path s2 on hard disk. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND
     * load s1       --    Execute commands from file s1 on hard disk, one command per line. Possible results: OK, FILE NOT FOUND
     * format size   --    Format the file system to the specified size (1K, 1M, 1G). If the file already contains data, it will be overwritten. Possible results: OK, CANNOT CREATE FILE
     * ln s1 s2      --    Create a hard link to the file s1 named s2. Possible results: OK, FILE NOT FOUND, PATH NOT FOUND
     * @param vfs
     */
    explicit CommandProcessor(VirtualFileSystem* vfs);

    /**
     * Register command as available in limited mode
     * @param command - command to register
     */
    void registerLimitedFunctionalityCommand(const string& command);

    /**
     * Check if command is available in limited modee
     * @param command - command to check
     * @return true if command is available in limited mode, false otherwise
     */
    bool isCommandAvailableInLimitedMode(const string& command) const;

    /**
     * Process command line by splitting it into command and arguments and calling appropriate method
     * @param input command line to process
     */
    void processCommandLine(const string& input);

private:
    VirtualFileSystem* vfs;
    unordered_map<string, function<void(const string&)>> commandMap;
    set<string> limitedFunctionalityCommands;

    void processCp(const vector<string>& args);
    void processMv(const vector<string>& args);
    void processRm(const vector<string>& args);
    void processMkdir(const vector<string>& args);
    void processRmdir(const vector<string>& args);
    void processLs(const vector<string>& args);
    void processCat(const vector<string>& args);
    void processCd(const vector<string>& args);
    void processPwd(const vector<string>& args);
    void processInfo(const vector<string>& args);
    void processIncp(const vector<string>& args);
    void processOutcp(const vector<string>& args);
    void processLoad(const vector<string>& args);
    void processFormat(const vector<string>& args);
    void processLn(const vector<string>& args);
    void processHelp(const vector<string>& args);

};

#endif // SEMESTRALNIPRACE_COMMANDPROCESSOR_HPP