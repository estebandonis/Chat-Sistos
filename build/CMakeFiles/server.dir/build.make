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
include CMakeFiles/server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server.dir/flags.make

CMakeFiles/server.dir/src/server.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/src/server.cpp.o: /Users/estebandonis/Documents/Séptimo\ semestre/Sistemas\ Operativos/Proyecto/Chat-Sistos/src/server.cpp
CMakeFiles/server.dir/src/server.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/server.dir/src/server.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/src/server.cpp.o -MF CMakeFiles/server.dir/src/server.cpp.o.d -o CMakeFiles/server.dir/src/server.cpp.o -c "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/src/server.cpp"

CMakeFiles/server.dir/src/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/server.dir/src/server.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/src/server.cpp" > CMakeFiles/server.dir/src/server.cpp.i

CMakeFiles/server.dir/src/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/server.dir/src/server.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/src/server.cpp" -o CMakeFiles/server.dir/src/server.cpp.s

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/src/server.cpp.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

server: CMakeFiles/server.dir/src/server.cpp.o
server: CMakeFiles/server.dir/build.make
server: libprotocol.a
server: /opt/homebrew/lib/libprotobuf.dylib
server: /opt/homebrew/lib/libabsl_log_internal_check_op.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_log_internal_message.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_examine_stack.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_log_internal_format.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_str_format_internal.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_log_internal_proto.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_log_internal_nullguard.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_log_internal_log_sink_set.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_log_internal_globals.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_log_globals.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_hash.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_city.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_bad_variant_access.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_low_level_hash.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_vlog_config_internal.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_bad_optional_access.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_log_internal_fnmatch.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_synchronization.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_stacktrace.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_symbolize.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_debugging_internal.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_demangle_internal.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_graphcycles_internal.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_malloc_internal.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_kernel_timeout_internal.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_log_sink.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_log_entry.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_strerror.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_time.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_civil_time.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_time_zone.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_strings.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_strings_internal.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_string_view.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_throw_delegate.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_base.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_raw_logging_internal.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_log_severity.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_spinlock_wait.2401.0.0.dylib
server: /opt/homebrew/lib/libabsl_int128.2401.0.0.dylib
server: CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server.dir/build: server
.PHONY : CMakeFiles/server.dir/build

CMakeFiles/server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server.dir/clean

CMakeFiles/server.dir/depend:
	cd "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos" "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos" "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/build" "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/build" "/Users/estebandonis/Documents/Séptimo semestre/Sistemas Operativos/Proyecto/Chat-Sistos/build/CMakeFiles/server.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/server.dir/depend
