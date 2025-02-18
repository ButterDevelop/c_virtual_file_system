cmake_minimum_required(VERSION 3.22)
project(SemestralWork)

set(CMAKE_CXX_STANDARD 17)

add_executable(SemestralWork Main.cpp
        Utils.cpp
        Utils.cpp
        Constants.hpp
        Constants.cpp
        Utils.hpp
        Inode.hpp
        Inode.cpp
        DirectoryItem.hpp
        DirectoryItem.cpp
        Inode.cpp
        Directory.hpp
        Directory.cpp
        Superblock.hpp
        Superblock.cpp
        VirtualFileSystem.hpp
        VirtualFileSystem.cpp
        CommandProcessor.hpp
        CommandProcessor.cpp)
