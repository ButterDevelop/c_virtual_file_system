#include <string>
#include <iostream>
#include "Utils.hpp"
#include "VirtualFileSystem.hpp"
#include "CommandProcessor.hpp"

using std::string;
using std::cout;


void startLoop(VirtualFileSystem* vfs) {
    auto* processor = new CommandProcessor(vfs);

    while (true) {
        cout << "> ";
        string input = removeEndOfLine(getLine());

        if (input.empty()) {
            continue;
        }

        string trimmed_input = trim_copy(input);
        processor->processCommandLine(trimmed_input);
    }
}


int main(int argc, const char* argv[]) {
    log(SIGNATURE);
    log(PROGRAM_INTRODUCTIONS_TEXT);

    if (argc == 2) {
        string filename = argv[1];
        log(LOADING_FILE_TEXT + filename);

        auto* vfs = new VirtualFileSystem(filename);

        startLoop(vfs);
    } else {
        log(PROGRAM_ERROR_EXIT_TEXT);
    }

    return 0;
}