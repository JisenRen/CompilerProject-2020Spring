# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rjs/CompilerProject-2020Spring

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rjs/CompilerProject-2020Spring/build

# Utility rule file for ContinuousMemCheck.

# Include the progress variables for this target.
include jsoncpp-build/CMakeFiles/ContinuousMemCheck.dir/progress.make

jsoncpp-build/CMakeFiles/ContinuousMemCheck:
	cd /home/rjs/CompilerProject-2020Spring/build/jsoncpp-build && /usr/bin/ctest -D ContinuousMemCheck

ContinuousMemCheck: jsoncpp-build/CMakeFiles/ContinuousMemCheck
ContinuousMemCheck: jsoncpp-build/CMakeFiles/ContinuousMemCheck.dir/build.make

.PHONY : ContinuousMemCheck

# Rule to build all files generated by this target.
jsoncpp-build/CMakeFiles/ContinuousMemCheck.dir/build: ContinuousMemCheck

.PHONY : jsoncpp-build/CMakeFiles/ContinuousMemCheck.dir/build

jsoncpp-build/CMakeFiles/ContinuousMemCheck.dir/clean:
	cd /home/rjs/CompilerProject-2020Spring/build/jsoncpp-build && $(CMAKE_COMMAND) -P CMakeFiles/ContinuousMemCheck.dir/cmake_clean.cmake
.PHONY : jsoncpp-build/CMakeFiles/ContinuousMemCheck.dir/clean

jsoncpp-build/CMakeFiles/ContinuousMemCheck.dir/depend:
	cd /home/rjs/CompilerProject-2020Spring/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rjs/CompilerProject-2020Spring /home/rjs/CompilerProject-2020Spring/3rdparty/jsoncpp /home/rjs/CompilerProject-2020Spring/build /home/rjs/CompilerProject-2020Spring/build/jsoncpp-build /home/rjs/CompilerProject-2020Spring/build/jsoncpp-build/CMakeFiles/ContinuousMemCheck.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : jsoncpp-build/CMakeFiles/ContinuousMemCheck.dir/depend

