# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_SOURCE_DIR = /home/yoyo/Desktop/PD/hw4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yoyo/Desktop/PD/hw4

# Include any dependencies generated for this target.
include CMakeFiles/Router.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Router.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Router.dir/flags.make

CMakeFiles/Router.dir/src/main.cpp.o: CMakeFiles/Router.dir/flags.make
CMakeFiles/Router.dir/src/main.cpp.o: src/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/yoyo/Desktop/PD/hw4/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Router.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Router.dir/src/main.cpp.o -c /home/yoyo/Desktop/PD/hw4/src/main.cpp

CMakeFiles/Router.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Router.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/yoyo/Desktop/PD/hw4/src/main.cpp > CMakeFiles/Router.dir/src/main.cpp.i

CMakeFiles/Router.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Router.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/yoyo/Desktop/PD/hw4/src/main.cpp -o CMakeFiles/Router.dir/src/main.cpp.s

CMakeFiles/Router.dir/src/main.cpp.o.requires:
.PHONY : CMakeFiles/Router.dir/src/main.cpp.o.requires

CMakeFiles/Router.dir/src/main.cpp.o.provides: CMakeFiles/Router.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Router.dir/build.make CMakeFiles/Router.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/Router.dir/src/main.cpp.o.provides

CMakeFiles/Router.dir/src/main.cpp.o.provides.build: CMakeFiles/Router.dir/src/main.cpp.o

CMakeFiles/Router.dir/src/FloorPlan.cpp.o: CMakeFiles/Router.dir/flags.make
CMakeFiles/Router.dir/src/FloorPlan.cpp.o: src/FloorPlan.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/yoyo/Desktop/PD/hw4/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Router.dir/src/FloorPlan.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Router.dir/src/FloorPlan.cpp.o -c /home/yoyo/Desktop/PD/hw4/src/FloorPlan.cpp

CMakeFiles/Router.dir/src/FloorPlan.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Router.dir/src/FloorPlan.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/yoyo/Desktop/PD/hw4/src/FloorPlan.cpp > CMakeFiles/Router.dir/src/FloorPlan.cpp.i

CMakeFiles/Router.dir/src/FloorPlan.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Router.dir/src/FloorPlan.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/yoyo/Desktop/PD/hw4/src/FloorPlan.cpp -o CMakeFiles/Router.dir/src/FloorPlan.cpp.s

CMakeFiles/Router.dir/src/FloorPlan.cpp.o.requires:
.PHONY : CMakeFiles/Router.dir/src/FloorPlan.cpp.o.requires

CMakeFiles/Router.dir/src/FloorPlan.cpp.o.provides: CMakeFiles/Router.dir/src/FloorPlan.cpp.o.requires
	$(MAKE) -f CMakeFiles/Router.dir/build.make CMakeFiles/Router.dir/src/FloorPlan.cpp.o.provides.build
.PHONY : CMakeFiles/Router.dir/src/FloorPlan.cpp.o.provides

CMakeFiles/Router.dir/src/FloorPlan.cpp.o.provides.build: CMakeFiles/Router.dir/src/FloorPlan.cpp.o

CMakeFiles/Router.dir/src/SpanningTree.cpp.o: CMakeFiles/Router.dir/flags.make
CMakeFiles/Router.dir/src/SpanningTree.cpp.o: src/SpanningTree.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/yoyo/Desktop/PD/hw4/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Router.dir/src/SpanningTree.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Router.dir/src/SpanningTree.cpp.o -c /home/yoyo/Desktop/PD/hw4/src/SpanningTree.cpp

CMakeFiles/Router.dir/src/SpanningTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Router.dir/src/SpanningTree.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/yoyo/Desktop/PD/hw4/src/SpanningTree.cpp > CMakeFiles/Router.dir/src/SpanningTree.cpp.i

CMakeFiles/Router.dir/src/SpanningTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Router.dir/src/SpanningTree.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/yoyo/Desktop/PD/hw4/src/SpanningTree.cpp -o CMakeFiles/Router.dir/src/SpanningTree.cpp.s

CMakeFiles/Router.dir/src/SpanningTree.cpp.o.requires:
.PHONY : CMakeFiles/Router.dir/src/SpanningTree.cpp.o.requires

CMakeFiles/Router.dir/src/SpanningTree.cpp.o.provides: CMakeFiles/Router.dir/src/SpanningTree.cpp.o.requires
	$(MAKE) -f CMakeFiles/Router.dir/build.make CMakeFiles/Router.dir/src/SpanningTree.cpp.o.provides.build
.PHONY : CMakeFiles/Router.dir/src/SpanningTree.cpp.o.provides

CMakeFiles/Router.dir/src/SpanningTree.cpp.o.provides.build: CMakeFiles/Router.dir/src/SpanningTree.cpp.o

CMakeFiles/Router.dir/src/SteinerTree.cpp.o: CMakeFiles/Router.dir/flags.make
CMakeFiles/Router.dir/src/SteinerTree.cpp.o: src/SteinerTree.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/yoyo/Desktop/PD/hw4/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Router.dir/src/SteinerTree.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Router.dir/src/SteinerTree.cpp.o -c /home/yoyo/Desktop/PD/hw4/src/SteinerTree.cpp

CMakeFiles/Router.dir/src/SteinerTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Router.dir/src/SteinerTree.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/yoyo/Desktop/PD/hw4/src/SteinerTree.cpp > CMakeFiles/Router.dir/src/SteinerTree.cpp.i

CMakeFiles/Router.dir/src/SteinerTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Router.dir/src/SteinerTree.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/yoyo/Desktop/PD/hw4/src/SteinerTree.cpp -o CMakeFiles/Router.dir/src/SteinerTree.cpp.s

CMakeFiles/Router.dir/src/SteinerTree.cpp.o.requires:
.PHONY : CMakeFiles/Router.dir/src/SteinerTree.cpp.o.requires

CMakeFiles/Router.dir/src/SteinerTree.cpp.o.provides: CMakeFiles/Router.dir/src/SteinerTree.cpp.o.requires
	$(MAKE) -f CMakeFiles/Router.dir/build.make CMakeFiles/Router.dir/src/SteinerTree.cpp.o.provides.build
.PHONY : CMakeFiles/Router.dir/src/SteinerTree.cpp.o.provides

CMakeFiles/Router.dir/src/SteinerTree.cpp.o.provides.build: CMakeFiles/Router.dir/src/SteinerTree.cpp.o

CMakeFiles/Router.dir/src/Graph/Graph.cpp.o: CMakeFiles/Router.dir/flags.make
CMakeFiles/Router.dir/src/Graph/Graph.cpp.o: src/Graph/Graph.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/yoyo/Desktop/PD/hw4/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Router.dir/src/Graph/Graph.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Router.dir/src/Graph/Graph.cpp.o -c /home/yoyo/Desktop/PD/hw4/src/Graph/Graph.cpp

CMakeFiles/Router.dir/src/Graph/Graph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Router.dir/src/Graph/Graph.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/yoyo/Desktop/PD/hw4/src/Graph/Graph.cpp > CMakeFiles/Router.dir/src/Graph/Graph.cpp.i

CMakeFiles/Router.dir/src/Graph/Graph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Router.dir/src/Graph/Graph.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/yoyo/Desktop/PD/hw4/src/Graph/Graph.cpp -o CMakeFiles/Router.dir/src/Graph/Graph.cpp.s

CMakeFiles/Router.dir/src/Graph/Graph.cpp.o.requires:
.PHONY : CMakeFiles/Router.dir/src/Graph/Graph.cpp.o.requires

CMakeFiles/Router.dir/src/Graph/Graph.cpp.o.provides: CMakeFiles/Router.dir/src/Graph/Graph.cpp.o.requires
	$(MAKE) -f CMakeFiles/Router.dir/build.make CMakeFiles/Router.dir/src/Graph/Graph.cpp.o.provides.build
.PHONY : CMakeFiles/Router.dir/src/Graph/Graph.cpp.o.provides

CMakeFiles/Router.dir/src/Graph/Graph.cpp.o.provides.build: CMakeFiles/Router.dir/src/Graph/Graph.cpp.o

CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o: CMakeFiles/Router.dir/flags.make
CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o: src/GnuplotLivePlotter/GnuplotLivePlotter.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/yoyo/Desktop/PD/hw4/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o -c /home/yoyo/Desktop/PD/hw4/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp

CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/yoyo/Desktop/PD/hw4/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp > CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.i

CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/yoyo/Desktop/PD/hw4/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp -o CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.s

CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o.requires:
.PHONY : CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o.requires

CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o.provides: CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o.requires
	$(MAKE) -f CMakeFiles/Router.dir/build.make CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o.provides.build
.PHONY : CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o.provides

CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o.provides.build: CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o

# Object files for target Router
Router_OBJECTS = \
"CMakeFiles/Router.dir/src/main.cpp.o" \
"CMakeFiles/Router.dir/src/FloorPlan.cpp.o" \
"CMakeFiles/Router.dir/src/SpanningTree.cpp.o" \
"CMakeFiles/Router.dir/src/SteinerTree.cpp.o" \
"CMakeFiles/Router.dir/src/Graph/Graph.cpp.o" \
"CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o"

# External object files for target Router
Router_EXTERNAL_OBJECTS =

Router: CMakeFiles/Router.dir/src/main.cpp.o
Router: CMakeFiles/Router.dir/src/FloorPlan.cpp.o
Router: CMakeFiles/Router.dir/src/SpanningTree.cpp.o
Router: CMakeFiles/Router.dir/src/SteinerTree.cpp.o
Router: CMakeFiles/Router.dir/src/Graph/Graph.cpp.o
Router: CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o
Router: CMakeFiles/Router.dir/build.make
Router: /usr/lib/x86_64-linux-gnu/libboost_system.so
Router: CMakeFiles/Router.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable Router"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Router.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Router.dir/build: Router
.PHONY : CMakeFiles/Router.dir/build

CMakeFiles/Router.dir/requires: CMakeFiles/Router.dir/src/main.cpp.o.requires
CMakeFiles/Router.dir/requires: CMakeFiles/Router.dir/src/FloorPlan.cpp.o.requires
CMakeFiles/Router.dir/requires: CMakeFiles/Router.dir/src/SpanningTree.cpp.o.requires
CMakeFiles/Router.dir/requires: CMakeFiles/Router.dir/src/SteinerTree.cpp.o.requires
CMakeFiles/Router.dir/requires: CMakeFiles/Router.dir/src/Graph/Graph.cpp.o.requires
CMakeFiles/Router.dir/requires: CMakeFiles/Router.dir/src/GnuplotLivePlotter/GnuplotLivePlotter.cpp.o.requires
.PHONY : CMakeFiles/Router.dir/requires

CMakeFiles/Router.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Router.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Router.dir/clean

CMakeFiles/Router.dir/depend:
	cd /home/yoyo/Desktop/PD/hw4 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yoyo/Desktop/PD/hw4 /home/yoyo/Desktop/PD/hw4 /home/yoyo/Desktop/PD/hw4 /home/yoyo/Desktop/PD/hw4 /home/yoyo/Desktop/PD/hw4/CMakeFiles/Router.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Router.dir/depend

