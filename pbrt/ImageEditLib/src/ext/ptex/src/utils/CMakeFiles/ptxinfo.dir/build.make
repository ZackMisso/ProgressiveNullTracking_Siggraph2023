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

# Include any dependencies generated for this target.
include src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/compiler_depend.make

# Include the progress variables for this target.
include src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/progress.make

# Include the compile flags for this target's objects.
include src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/flags.make

src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/ptxinfo.cpp.o: src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/flags.make
src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/ptxinfo.cpp.o: ../src/ext/ptex/src/utils/ptxinfo.cpp
src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/ptxinfo.cpp.o: src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/ptxinfo.cpp.o"
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/src/utils && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/ptxinfo.cpp.o -MF CMakeFiles/ptxinfo.dir/ptxinfo.cpp.o.d -o CMakeFiles/ptxinfo.dir/ptxinfo.cpp.o -c /Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/ptex/src/utils/ptxinfo.cpp

src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/ptxinfo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ptxinfo.dir/ptxinfo.cpp.i"
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/src/utils && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/ptex/src/utils/ptxinfo.cpp > CMakeFiles/ptxinfo.dir/ptxinfo.cpp.i

src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/ptxinfo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ptxinfo.dir/ptxinfo.cpp.s"
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/src/utils && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/ptex/src/utils/ptxinfo.cpp -o CMakeFiles/ptxinfo.dir/ptxinfo.cpp.s

# Object files for target ptxinfo
ptxinfo_OBJECTS = \
"CMakeFiles/ptxinfo.dir/ptxinfo.cpp.o"

# External object files for target ptxinfo
ptxinfo_EXTERNAL_OBJECTS =

src/ext/ptex/src/utils/ptxinfo: src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/ptxinfo.cpp.o
src/ext/ptex/src/utils/ptxinfo: src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/build.make
src/ext/ptex/src/utils/ptxinfo: src/ext/ptex/src/ptex/libPtex.a
src/ext/ptex/src/utils/ptxinfo: /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk/usr/lib/libz.tbd
src/ext/ptex/src/utils/ptxinfo: src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ptxinfo"
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/src/utils && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ptxinfo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/build: src/ext/ptex/src/utils/ptxinfo
.PHONY : src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/build

src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/clean:
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/src/utils && $(CMAKE_COMMAND) -P CMakeFiles/ptxinfo.dir/cmake_clean.cmake
.PHONY : src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/clean

src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/depend:
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/corneria/Documents/Research/bias_research/pbrttest /Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/ptex/src/utils /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/src/utils /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/ext/ptex/src/utils/CMakeFiles/ptxinfo.dir/depend

