# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.24.2/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.24.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/vidhyamurali/Documents/cbdc/CBDC_F22

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/vidhyamurali/Documents/cbdc/CBDC_F22

# Include any dependencies generated for this target.
include CMakeFiles/mypipe.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/mypipe.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/mypipe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mypipe.dir/flags.make

CMakeFiles/mypipe.dir/mypipe.cpp.o: CMakeFiles/mypipe.dir/flags.make
CMakeFiles/mypipe.dir/mypipe.cpp.o: mypipe.cpp
CMakeFiles/mypipe.dir/mypipe.cpp.o: CMakeFiles/mypipe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vidhyamurali/Documents/cbdc/CBDC_F22/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mypipe.dir/mypipe.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mypipe.dir/mypipe.cpp.o -MF CMakeFiles/mypipe.dir/mypipe.cpp.o.d -o CMakeFiles/mypipe.dir/mypipe.cpp.o -c /Users/vidhyamurali/Documents/cbdc/CBDC_F22/mypipe.cpp

CMakeFiles/mypipe.dir/mypipe.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mypipe.dir/mypipe.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/vidhyamurali/Documents/cbdc/CBDC_F22/mypipe.cpp > CMakeFiles/mypipe.dir/mypipe.cpp.i

CMakeFiles/mypipe.dir/mypipe.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mypipe.dir/mypipe.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/vidhyamurali/Documents/cbdc/CBDC_F22/mypipe.cpp -o CMakeFiles/mypipe.dir/mypipe.cpp.s

# Object files for target mypipe
mypipe_OBJECTS = \
"CMakeFiles/mypipe.dir/mypipe.cpp.o"

# External object files for target mypipe
mypipe_EXTERNAL_OBJECTS =

mypipe: CMakeFiles/mypipe.dir/mypipe.cpp.o
mypipe: CMakeFiles/mypipe.dir/build.make
mypipe: CMakeFiles/mypipe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/vidhyamurali/Documents/cbdc/CBDC_F22/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable mypipe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mypipe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mypipe.dir/build: mypipe
.PHONY : CMakeFiles/mypipe.dir/build

CMakeFiles/mypipe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mypipe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mypipe.dir/clean

CMakeFiles/mypipe.dir/depend:
	cd /Users/vidhyamurali/Documents/cbdc/CBDC_F22 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vidhyamurali/Documents/cbdc/CBDC_F22 /Users/vidhyamurali/Documents/cbdc/CBDC_F22 /Users/vidhyamurali/Documents/cbdc/CBDC_F22 /Users/vidhyamurali/Documents/cbdc/CBDC_F22 /Users/vidhyamurali/Documents/cbdc/CBDC_F22/CMakeFiles/mypipe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mypipe.dir/depend

