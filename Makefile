CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Object files
OBJS = Main.o Utils.o Constants.o Inode.o DirectoryItem.o Directory.o Superblock.o VirtualFileSystem.o CommandProcessor.o

# Name of the executable
EXEC = SemestralWork

# Default target
all: $(EXEC)

# Link the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS)

# Compile the source files into object files
Main.o: Main.cpp
	$(CXX) $(CXXFLAGS) -c Main.cpp

Utils.o: Utils.cpp Utils.hpp
	$(CXX) $(CXXFLAGS) -c Utils.cpp

Constants.o: Constants.cpp Constants.hpp
	$(CXX) $(CXXFLAGS) -c Constants.cpp

Inode.o: Inode.cpp Inode.hpp
	$(CXX) $(CXXFLAGS) -c Inode.cpp

DirectoryItem.o: DirectoryItem.cpp DirectoryItem.hpp
	$(CXX) $(CXXFLAGS) -c DirectoryItem.cpp

Directory.o: Directory.cpp Directory.hpp
	$(CXX) $(CXXFLAGS) -c Directory.cpp

Superblock.o: Superblock.cpp Superblock.hpp
	$(CXX) $(CXXFLAGS) -c Superblock.cpp

VirtualFileSystem.o: VirtualFileSystem.cpp VirtualFileSystem.hpp
	$(CXX) $(CXXFLAGS) -c VirtualFileSystem.cpp

CommandProcessor.o: CommandProcessor.cpp CommandProcessor.hpp
	$(CXX) $(CXXFLAGS) -c CommandProcessor.cpp

# Clean target
clean:
	rm -f $(OBJS) $(EXEC)
