# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.23.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.23.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/corneria/Documents/Research/bias_research/pbrttest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib

# Utility rule file for NightlyMemCheck.

# Include any custom commands dependencies for this target.
include src/ext/ptex/CMakeFiles/NightlyMemCheck.dir/compiler_depend.make

# Include the progress variables for this target.
include src/ext/ptex/CMakeFiles/NightlyMemCheck.dir/progress.make

src/ext/ptex/CMakeFiles/NightlyMemCheck:
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex && /usr/local/Cellar/cmake/3.23.0/bin/ctest -D NightlyMemCheck

NightlyMemCheck: src/ext/ptex/CMakeFiles/NightlyMemCheck
NightlyMemCheck: src/ext/ptex/CMakeFiles/NightlyMemCheck.dir/build.make
.PHONY : NightlyMemCheck

# Rule to build all files generated by this target.
src/ext/ptex/CMakeFiles/NightlyMemCheck.dir/build: NightlyMemCheck
.PHONY : src/ext/ptex/CMakeFiles/NightlyMemCheck.dir/build

src/ext/ptex/CMakeFiles/NightlyMemCheck.dir/clean:
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex && $(CMAKE_COMMAND) -P CMakeFiles/NightlyMemCheck.dir/cmake_clean.cmake
.PHONY : src/ext/ptex/CMakeFiles/NightlyMemCheck.dir/clean

src/ext/ptex/CMakeFiles/NightlyMemCheck.dir/depend:
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/corneria/Documents/Research/bias_research/pbrttest /Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/ptex /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/CMakeFiles/NightlyMemCheck.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/ext/ptex/CMakeFiles/NightlyMemCheck.dir/depend

