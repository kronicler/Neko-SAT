# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/ttg/catkin_ws/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ttg/catkin_ws/build

# Utility rule file for cam_process_generate_messages_cpp.

# Include the progress variables for this target.
include cam_process/CMakeFiles/cam_process_generate_messages_cpp.dir/progress.make

cam_process/CMakeFiles/cam_process_generate_messages_cpp: /home/ttg/catkin_ws/devel/include/cam_process/BoundingBox.h


/home/ttg/catkin_ws/devel/include/cam_process/BoundingBox.h: /opt/ros/kinetic/lib/gencpp/gen_cpp.py
/home/ttg/catkin_ws/devel/include/cam_process/BoundingBox.h: /home/ttg/catkin_ws/src/cam_process/msg/BoundingBox.msg
/home/ttg/catkin_ws/devel/include/cam_process/BoundingBox.h: /opt/ros/kinetic/share/gencpp/msg.h.template
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ttg/catkin_ws/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating C++ code from cam_process/BoundingBox.msg"
	cd /home/ttg/catkin_ws/src/cam_process && /home/ttg/catkin_ws/build/catkin_generated/env_cached.sh /usr/bin/python /opt/ros/kinetic/share/gencpp/cmake/../../../lib/gencpp/gen_cpp.py /home/ttg/catkin_ws/src/cam_process/msg/BoundingBox.msg -Icam_process:/home/ttg/catkin_ws/src/cam_process/msg -Isensor_msgs:/opt/ros/kinetic/share/sensor_msgs/cmake/../msg -Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg -Igeometry_msgs:/opt/ros/kinetic/share/geometry_msgs/cmake/../msg -p cam_process -o /home/ttg/catkin_ws/devel/include/cam_process -e /opt/ros/kinetic/share/gencpp/cmake/..

cam_process_generate_messages_cpp: cam_process/CMakeFiles/cam_process_generate_messages_cpp
cam_process_generate_messages_cpp: /home/ttg/catkin_ws/devel/include/cam_process/BoundingBox.h
cam_process_generate_messages_cpp: cam_process/CMakeFiles/cam_process_generate_messages_cpp.dir/build.make

.PHONY : cam_process_generate_messages_cpp

# Rule to build all files generated by this target.
cam_process/CMakeFiles/cam_process_generate_messages_cpp.dir/build: cam_process_generate_messages_cpp

.PHONY : cam_process/CMakeFiles/cam_process_generate_messages_cpp.dir/build

cam_process/CMakeFiles/cam_process_generate_messages_cpp.dir/clean:
	cd /home/ttg/catkin_ws/build/cam_process && $(CMAKE_COMMAND) -P CMakeFiles/cam_process_generate_messages_cpp.dir/cmake_clean.cmake
.PHONY : cam_process/CMakeFiles/cam_process_generate_messages_cpp.dir/clean

cam_process/CMakeFiles/cam_process_generate_messages_cpp.dir/depend:
	cd /home/ttg/catkin_ws/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ttg/catkin_ws/src /home/ttg/catkin_ws/src/cam_process /home/ttg/catkin_ws/build /home/ttg/catkin_ws/build/cam_process /home/ttg/catkin_ws/build/cam_process/CMakeFiles/cam_process_generate_messages_cpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : cam_process/CMakeFiles/cam_process_generate_messages_cpp.dir/depend

