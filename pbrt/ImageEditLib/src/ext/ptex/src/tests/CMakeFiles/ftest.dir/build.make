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
include src/ext/ptex/src/tests/CMakeFiles/ftest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/ext/ptex/src/tests/CMakeFiles/ftest.dir/compiler_depend.make

# Include the progress variables for this target.
include src/ext/ptex/src/tests/CMakeFiles/ftest.dir/progress.make

# Include the compile flags for this target's objects.
include src/ext/ptex/src/tests/CMakeFiles/ftest.dir/flags.make

src/ext/ptex/src/tests/CMakeFiles/ftest.dir/ftest.cpp.o: src/ext/ptex/src/tests/CMakeFiles/ftest.dir/flags.make
src/ext/ptex/src/tests/CMakeFiles/ftest.dir/ftest.cpp.o: ../src/ext/ptex/src/tests/ftest.cpp
src/ext/ptex/src/tests/CMakeFiles/ftest.dir/ftest.cpp.o: src/ext/ptex/src/tests/CMakeFiles/ftest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/ext/ptex/src/tests/CMakeFiles/ftest.dir/ftest.cpp.o"
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/src/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/ext/ptex/src/tests/CMakeFiles/ftest.dir/ftest.cpp.o -MF CMakeFiles/ftest.dir/ftest.cpp.o.d -o CMakeFiles/ftest.dir/ftest.cpp.o -c /Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/ptex/src/tests/ftest.cpp

src/ext/ptex/src/tests/CMakeFiles/ftest.dir/ftest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ftest.dir/ftest.cpp.i"
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/src/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/ptex/src/tests/ftest.cpp > CMakeFiles/ftest.dir/ftest.cpp.i

src/ext/ptex/src/tests/CMakeFiles/ftest.dir/ftest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ftest.dir/ftest.cpp.s"
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/src/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/ptex/src/tests/ftest.cpp -o CMakeFiles/ftest.dir/ftest.cpp.s

# Object files for target ftest
ftest_OBJECTS = \
"CMakeFiles/ftest.dir/ftest.cpp.o"

# External object files for target ftest
ftest_EXTERNAL_OBJECTS =

src/ext/ptex/src/tests/ftest: src/ext/ptex/src/tests/CMakeFiles/ftest.dir/ftest.cpp.o
src/ext/ptex/src/tests/ftest: src/ext/ptex/src/tests/CMakeFiles/ftest.dir/build.make
src/ext/ptex/src/tests/ftest: src/ext/ptex/src/ptex/libPtex.dylib
src/ext/ptex/src/tests/ftest: /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk/usr/lib/libz.tbd
src/ext/ptex/src/tests/ftest: src/ext/ptex/src/tests/CMakeFiles/ftest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ftest"
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/src/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ftest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/ext/ptex/src/tests/CMakeFiles/ftest.dir/build: src/ext/ptex/src/tests/ftest
.PHONY : src/ext/ptex/src/tests/CMakeFiles/ftest.dir/build

src/ext/ptex/src/tests/CMakeFiles/ftest.dir/clean:
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/src/tests && $(CMAKE_COMMAND) -P CMakeFiles/ftest.dir/cmake_clean.cmake
.PHONY : src/ext/ptex/src/tests/CMakeFiles/ftest.dir/clean

src/ext/ptex/src/tests/CMakeFiles/ftest.dir/depend:
	cd /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/corneria/Documents/Research/bias_research/pbrttest /Users/corneria/Documents/Research/bias_research/pbrttest/src/ext/ptex/src/tests /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/src/tests /Users/corneria/Documents/Research/bias_research/pbrttest/ImageEditLib/src/ext/ptex/src/tests/CMakeFiles/ftest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/ext/ptex/src/tests/CMakeFiles/ftest.dir/depend

