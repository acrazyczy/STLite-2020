# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = "/mnt/d/Academy/OIACM/Code/Homework/STL Homework/STLite-2020/mapA"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/d/Academy/OIACM/Code/Homework/STL Homework/STLite-2020/mapA/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/STLite_mapA.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/STLite_mapA.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/STLite_mapA.dir/flags.make

CMakeFiles/STLite_mapA.dir/code.cpp.o: CMakeFiles/STLite_mapA.dir/flags.make
CMakeFiles/STLite_mapA.dir/code.cpp.o: ../code.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/d/Academy/OIACM/Code/Homework/STL Homework/STLite-2020/mapA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/STLite_mapA.dir/code.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/STLite_mapA.dir/code.cpp.o -c "/mnt/d/Academy/OIACM/Code/Homework/STL Homework/STLite-2020/mapA/code.cpp"

CMakeFiles/STLite_mapA.dir/code.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STLite_mapA.dir/code.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/d/Academy/OIACM/Code/Homework/STL Homework/STLite-2020/mapA/code.cpp" > CMakeFiles/STLite_mapA.dir/code.cpp.i

CMakeFiles/STLite_mapA.dir/code.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STLite_mapA.dir/code.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/d/Academy/OIACM/Code/Homework/STL Homework/STLite-2020/mapA/code.cpp" -o CMakeFiles/STLite_mapA.dir/code.cpp.s

CMakeFiles/STLite_mapA.dir/code.cpp.o.requires:

.PHONY : CMakeFiles/STLite_mapA.dir/code.cpp.o.requires

CMakeFiles/STLite_mapA.dir/code.cpp.o.provides: CMakeFiles/STLite_mapA.dir/code.cpp.o.requires
	$(MAKE) -f CMakeFiles/STLite_mapA.dir/build.make CMakeFiles/STLite_mapA.dir/code.cpp.o.provides.build
.PHONY : CMakeFiles/STLite_mapA.dir/code.cpp.o.provides

CMakeFiles/STLite_mapA.dir/code.cpp.o.provides.build: CMakeFiles/STLite_mapA.dir/code.cpp.o


# Object files for target STLite_mapA
STLite_mapA_OBJECTS = \
"CMakeFiles/STLite_mapA.dir/code.cpp.o"

# External object files for target STLite_mapA
STLite_mapA_EXTERNAL_OBJECTS =

STLite_mapA: CMakeFiles/STLite_mapA.dir/code.cpp.o
STLite_mapA: CMakeFiles/STLite_mapA.dir/build.make
STLite_mapA: CMakeFiles/STLite_mapA.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/d/Academy/OIACM/Code/Homework/STL Homework/STLite-2020/mapA/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable STLite_mapA"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/STLite_mapA.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/STLite_mapA.dir/build: STLite_mapA

.PHONY : CMakeFiles/STLite_mapA.dir/build

CMakeFiles/STLite_mapA.dir/requires: CMakeFiles/STLite_mapA.dir/code.cpp.o.requires

.PHONY : CMakeFiles/STLite_mapA.dir/requires

CMakeFiles/STLite_mapA.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/STLite_mapA.dir/cmake_clean.cmake
.PHONY : CMakeFiles/STLite_mapA.dir/clean

CMakeFiles/STLite_mapA.dir/depend:
	cd "/mnt/d/Academy/OIACM/Code/Homework/STL Homework/STLite-2020/mapA/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/d/Academy/OIACM/Code/Homework/STL Homework/STLite-2020/mapA" "/mnt/d/Academy/OIACM/Code/Homework/STL Homework/STLite-2020/mapA" "/mnt/d/Academy/OIACM/Code/Homework/STL Homework/STLite-2020/mapA/cmake-build-debug" "/mnt/d/Academy/OIACM/Code/Homework/STL Homework/STLite-2020/mapA/cmake-build-debug" "/mnt/d/Academy/OIACM/Code/Homework/STL Homework/STLite-2020/mapA/cmake-build-debug/CMakeFiles/STLite_mapA.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/STLite_mapA.dir/depend

