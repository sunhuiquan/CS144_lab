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
CMAKE_SOURCE_DIR = /home/Work/CS144_lab

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/Work/CS144_lab/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/spongechecks.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/spongechecks.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/spongechecks.dir/flags.make

tests/CMakeFiles/spongechecks.dir/byte_stream_test_harness.cc.o: tests/CMakeFiles/spongechecks.dir/flags.make
tests/CMakeFiles/spongechecks.dir/byte_stream_test_harness.cc.o: ../tests/byte_stream_test_harness.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Work/CS144_lab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/spongechecks.dir/byte_stream_test_harness.cc.o"
	cd /home/Work/CS144_lab/build/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spongechecks.dir/byte_stream_test_harness.cc.o -c /home/Work/CS144_lab/tests/byte_stream_test_harness.cc

tests/CMakeFiles/spongechecks.dir/byte_stream_test_harness.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spongechecks.dir/byte_stream_test_harness.cc.i"
	cd /home/Work/CS144_lab/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Work/CS144_lab/tests/byte_stream_test_harness.cc > CMakeFiles/spongechecks.dir/byte_stream_test_harness.cc.i

tests/CMakeFiles/spongechecks.dir/byte_stream_test_harness.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spongechecks.dir/byte_stream_test_harness.cc.s"
	cd /home/Work/CS144_lab/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Work/CS144_lab/tests/byte_stream_test_harness.cc -o CMakeFiles/spongechecks.dir/byte_stream_test_harness.cc.s

# Object files for target spongechecks
spongechecks_OBJECTS = \
"CMakeFiles/spongechecks.dir/byte_stream_test_harness.cc.o"

# External object files for target spongechecks
spongechecks_EXTERNAL_OBJECTS =

tests/libspongechecks.a: tests/CMakeFiles/spongechecks.dir/byte_stream_test_harness.cc.o
tests/libspongechecks.a: tests/CMakeFiles/spongechecks.dir/build.make
tests/libspongechecks.a: tests/CMakeFiles/spongechecks.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/Work/CS144_lab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libspongechecks.a"
	cd /home/Work/CS144_lab/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/spongechecks.dir/cmake_clean_target.cmake
	cd /home/Work/CS144_lab/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/spongechecks.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/spongechecks.dir/build: tests/libspongechecks.a

.PHONY : tests/CMakeFiles/spongechecks.dir/build

tests/CMakeFiles/spongechecks.dir/clean:
	cd /home/Work/CS144_lab/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/spongechecks.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/spongechecks.dir/clean

tests/CMakeFiles/spongechecks.dir/depend:
	cd /home/Work/CS144_lab/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/Work/CS144_lab /home/Work/CS144_lab/tests /home/Work/CS144_lab/build /home/Work/CS144_lab/build/tests /home/Work/CS144_lab/build/tests/CMakeFiles/spongechecks.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/spongechecks.dir/depend

