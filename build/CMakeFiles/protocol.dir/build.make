# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.29.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.29.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/build"

# Include any dependencies generated for this target.
include CMakeFiles/protocol.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/protocol.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/protocol.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/protocol.dir/flags.make

CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.o: CMakeFiles/protocol.dir/flags.make
CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.o: /Users/estebandonis/Documents/Séptimo\ semestre/Sistemas\ Operativos/Proyecto/Chat-Sistos/src/protocol/chat.pb.cc
CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.o: CMakeFiles/protocol.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.o -MF CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.o.d -o CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.o -c "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/src/protocol/chat.pb.cc"

CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/src/protocol/chat.pb.cc" > CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.i

CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/src/protocol/chat.pb.cc" -o CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.s

CMakeFiles/protocol.dir/src/protocol/message.cpp.o: CMakeFiles/protocol.dir/flags.make
CMakeFiles/protocol.dir/src/protocol/message.cpp.o: /Users/estebandonis/Documents/Séptimo\ semestre/Sistemas\ Operativos/Proyecto/Chat-Sistos/src/protocol/message.cpp
CMakeFiles/protocol.dir/src/protocol/message.cpp.o: CMakeFiles/protocol.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/protocol.dir/src/protocol/message.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/protocol.dir/src/protocol/message.cpp.o -MF CMakeFiles/protocol.dir/src/protocol/message.cpp.o.d -o CMakeFiles/protocol.dir/src/protocol/message.cpp.o -c "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/src/protocol/message.cpp"

CMakeFiles/protocol.dir/src/protocol/message.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/protocol.dir/src/protocol/message.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/src/protocol/message.cpp" > CMakeFiles/protocol.dir/src/protocol/message.cpp.i

CMakeFiles/protocol.dir/src/protocol/message.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/protocol.dir/src/protocol/message.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/src/protocol/message.cpp" -o CMakeFiles/protocol.dir/src/protocol/message.cpp.s

# Object files for target protocol
protocol_OBJECTS = \
"CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.o" \
"CMakeFiles/protocol.dir/src/protocol/message.cpp.o"

# External object files for target protocol
protocol_EXTERNAL_OBJECTS =

libprotocol.a: CMakeFiles/protocol.dir/src/protocol/chat.pb.cc.o
libprotocol.a: CMakeFiles/protocol.dir/src/protocol/message.cpp.o
libprotocol.a: CMakeFiles/protocol.dir/build.make
libprotocol.a: CMakeFiles/protocol.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libprotocol.a"
	$(CMAKE_COMMAND) -P CMakeFiles/protocol.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/protocol.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/protocol.dir/build: libprotocol.a
.PHONY : CMakeFiles/protocol.dir/build

CMakeFiles/protocol.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/protocol.dir/cmake_clean.cmake
.PHONY : CMakeFiles/protocol.dir/clean

CMakeFiles/protocol.dir/depend:
	cd "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos" "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos" "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/build" "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/build" "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/build/CMakeFiles/protocol.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/protocol.dir/depend

