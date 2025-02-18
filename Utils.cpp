#include <iostream>
#include <algorithm>
#include <sstream>
#include "Constants.hpp"
#include "Utils.hpp"

using std::string;
using std::getline;
using std::cin;
using std::remove_if;
using std::endl;
using std::cerr;
using std::cout;
using std::strtol;
using std::ptr_fun;
using std::istringstream;
using std::to_string;

void log(const string& message, bool endLine) {
    if (IS_DEBUG) {
        cout << message;
        if (endLine) {
            cout << endl;
        }
    }
}

string getLine() {
    string line;
    getline(cin, line);
    return line;
}

string removeEndOfLine(const string& message) {
    string newMessage = message;
    newMessage.erase(
            remove_if(newMessage.begin(), newMessage.end(),
                           [](char ch) { return ch == '\n' || ch == '\r'; }),
            newMessage.end());
    return newMessage;
}

int32_t getSizeFromString(const string& stringSize) {
    if (stringSize.empty()) {
        return ERROR_CODE;
    }

    char* end;
    long number = strtol(stringSize.c_str(), &end, 10);

    if (number == 0 || *end == '\0') {
        log(ERROR_PARSING_SIZE_STRING_TEXT + stringSize);
        return ERROR_CODE;
    }

    string units(end);
    if (units == K_SIZE) {
        number *= 1000;
    } else if (units == M_SIZE) {
        number *= 1000000;
    } else if (units == G_SIZE) {
        number *= 1000000000;
    } else if (!units.empty()) {
        number = 0;
    }

    return static_cast<int32_t>(number);
}

string &ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(),
                                    not1(ptr_fun<int, int>(isspace))));
    return s;
}

string &rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(),
                         not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}

string &trim(string &s) {
    return ltrim(rtrim(s));
}

string trim_copy(string s) {
    trim(s);
    return s;
}

string getDirPath(const string& fullPath) {
    size_t lastSlashPos = fullPath.find_last_of('/');
    return (lastSlashPos != string::npos) ? fullPath.substr(0, lastSlashPos) : ".";
}


string getFileName(const string& fullPath) {
    size_t lastSlashPos = fullPath.find_last_of('/');
    return (lastSlashPos != string::npos) ? fullPath.substr(lastSlashPos + 1) : fullPath;
}

string intToString(int32_t number) {
    return to_string(number);
}

vector<string> splitString(const string& str, char delim) {
    istringstream iss(str);
    vector<string> args;
    string arg;

    while (iss >> arg) {
        args.push_back(arg);
    }

    return args;
}