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
CMAKE_SOURCE_DIR = /home/water/Work/CS144_lab

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/water/Work/CS144_lab/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/fsm_retx.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/fsm_retx.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/fsm_retx.dir/flags.make

tests/CMakeFiles/fsm_retx.dir/fsm_retx.cc.o: tests/CMakeFiles/fsm_retx.dir/flags.make
tests/CMakeFiles/fsm_retx.dir/fsm_retx.cc.o: ../tests/fsm_retx.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/water/Work/CS144_lab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/fsm_retx.dir/fsm_retx.cc.o"
	cd /home/water/Work/CS144_lab/build/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fsm_retx.dir/fsm_retx.cc.o -c /home/water/Work/CS144_lab/tests/fsm_retx.cc

tests/CMakeFiles/fsm_retx.dir/fsm_retx.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fsm_retx.dir/fsm_retx.cc.i"
	cd /home/water/Work/CS144_lab/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/water/Work/CS144_lab/tests/fsm_retx.cc > CMakeFiles/fsm_retx.dir/fsm_retx.cc.i

tests/CMakeFiles/fsm_retx.dir/fsm_retx.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fsm_retx.dir/fsm_retx.cc.s"
	cd /home/water/Work/CS144_lab/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/water/Work/CS144_lab/tests/fsm_retx.cc -o CMakeFiles/fsm_retx.dir/fsm_retx.cc.s

# Object files for target fsm_retx
fsm_retx_OBJECTS = \
"CMakeFiles/fsm_retx.dir/fsm_retx.cc.o"

# External object files for target fsm_retx
fsm_retx_EXTERNAL_OBJECTS =

tests/fsm_retx: tests/CMakeFiles/fsm_retx.dir/fsm_retx.cc.o
tests/fsm_retx: tests/CMakeFiles/fsm_retx.dir/build.make
tests/fsm_retx: tests/libspongechecks.a
tests/fsm_retx: libsponge/libsponge.a
tests/fsm_retx: tests/CMakeFiles/fsm_retx.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/water/Work/CS144_lab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable fsm_retx"
	cd /home/water/Work/CS144_lab/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fsm_retx.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/fsm_retx.dir/build: tests/fsm_retx

.PHONY : tests/CMakeFiles/fsm_retx.dir/build

tests/CMakeFiles/fsm_retx.dir/clean:
	cd /home/water/Work/CS144_lab/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/fsm_retx.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/fsm_retx.dir/clean

tests/CMakeFiles/fsm_retx.dir/depend:
	cd /home/water/Work/CS144_lab/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/water/Work/CS144_lab /home/water/Work/CS144_lab/tests /home/water/Work/CS144_lab/build /home/water/Work/CS144_lab/build/tests /home/water/Work/CS144_lab/build/tests/CMakeFiles/fsm_retx.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/fsm_retx.dir/depend
