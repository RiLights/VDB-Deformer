# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_COMMAND = /Volumes/resources/pkg/cmake/3.4.0/platform-linux/arch-x86_64/bin/cmake

# The command to remove a file.
RM = /Volumes/resources/pkg/cmake/3.4.0/platform-linux/arch-x86_64/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/opochtarenko/workspace/VDB-Deformer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/opochtarenko/workspace/VDB-Deformer/build/gcc-4.8

# Include any dependencies generated for this target.
include CMakeFiles/vdb_deformer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/vdb_deformer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/vdb_deformer.dir/flags.make

CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o: CMakeFiles/vdb_deformer.dir/flags.make
CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o: ../../src/vdb_deformer.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/opochtarenko/workspace/VDB-Deformer/build/gcc-4.8/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o -c /home/opochtarenko/workspace/VDB-Deformer/src/vdb_deformer.C

CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/opochtarenko/workspace/VDB-Deformer/src/vdb_deformer.C > CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.i

CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/opochtarenko/workspace/VDB-Deformer/src/vdb_deformer.C -o CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.s

CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o.requires:

.PHONY : CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o.requires

CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o.provides: CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o.requires
	$(MAKE) -f CMakeFiles/vdb_deformer.dir/build.make CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o.provides.build
.PHONY : CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o.provides

CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o.provides.build: CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o


# Object files for target vdb_deformer
vdb_deformer_OBJECTS = \
"CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o"

# External object files for target vdb_deformer
vdb_deformer_EXTERNAL_OBJECTS =

vdb_deformer.so: CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o
vdb_deformer.so: CMakeFiles/vdb_deformer.dir/build.make
vdb_deformer.so: CMakeFiles/vdb_deformer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/opochtarenko/workspace/VDB-Deformer/build/gcc-4.8/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library vdb_deformer.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vdb_deformer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/vdb_deformer.dir/build: vdb_deformer.so

.PHONY : CMakeFiles/vdb_deformer.dir/build

CMakeFiles/vdb_deformer.dir/requires: CMakeFiles/vdb_deformer.dir/src/vdb_deformer.C.o.requires

.PHONY : CMakeFiles/vdb_deformer.dir/requires

CMakeFiles/vdb_deformer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/vdb_deformer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/vdb_deformer.dir/clean

CMakeFiles/vdb_deformer.dir/depend:
	cd /home/opochtarenko/workspace/VDB-Deformer/build/gcc-4.8 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/opochtarenko/workspace/VDB-Deformer /home/opochtarenko/workspace/VDB-Deformer /home/opochtarenko/workspace/VDB-Deformer/build/gcc-4.8 /home/opochtarenko/workspace/VDB-Deformer/build/gcc-4.8 /home/opochtarenko/workspace/VDB-Deformer/build/gcc-4.8/CMakeFiles/vdb_deformer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/vdb_deformer.dir/depend

