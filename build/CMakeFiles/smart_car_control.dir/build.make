# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /opt/st/stm32cubeclt_1.17.0/CMake/bin/cmake

# The command to remove a file.
RM = /opt/st/stm32cubeclt_1.17.0/CMake/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build

# Include any dependencies generated for this target.
include CMakeFiles/smart_car_control.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/smart_car_control.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/smart_car_control.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/smart_car_control.dir/flags.make

CMakeFiles/smart_car_control.dir/src/GPIO.cpp.o: CMakeFiles/smart_car_control.dir/flags.make
CMakeFiles/smart_car_control.dir/src/GPIO.cpp.o: /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/GPIO.cpp
CMakeFiles/smart_car_control.dir/src/GPIO.cpp.o: CMakeFiles/smart_car_control.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/smart_car_control.dir/src/GPIO.cpp.o"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/smart_car_control.dir/src/GPIO.cpp.o -MF CMakeFiles/smart_car_control.dir/src/GPIO.cpp.o.d -o CMakeFiles/smart_car_control.dir/src/GPIO.cpp.o -c /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/GPIO.cpp

CMakeFiles/smart_car_control.dir/src/GPIO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/smart_car_control.dir/src/GPIO.cpp.i"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/GPIO.cpp > CMakeFiles/smart_car_control.dir/src/GPIO.cpp.i

CMakeFiles/smart_car_control.dir/src/GPIO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/smart_car_control.dir/src/GPIO.cpp.s"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/GPIO.cpp -o CMakeFiles/smart_car_control.dir/src/GPIO.cpp.s

CMakeFiles/smart_car_control.dir/src/core.cpp.o: CMakeFiles/smart_car_control.dir/flags.make
CMakeFiles/smart_car_control.dir/src/core.cpp.o: /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/core.cpp
CMakeFiles/smart_car_control.dir/src/core.cpp.o: CMakeFiles/smart_car_control.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/smart_car_control.dir/src/core.cpp.o"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/smart_car_control.dir/src/core.cpp.o -MF CMakeFiles/smart_car_control.dir/src/core.cpp.o.d -o CMakeFiles/smart_car_control.dir/src/core.cpp.o -c /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/core.cpp

CMakeFiles/smart_car_control.dir/src/core.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/smart_car_control.dir/src/core.cpp.i"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/core.cpp > CMakeFiles/smart_car_control.dir/src/core.cpp.i

CMakeFiles/smart_car_control.dir/src/core.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/smart_car_control.dir/src/core.cpp.s"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/core.cpp -o CMakeFiles/smart_car_control.dir/src/core.cpp.s

CMakeFiles/smart_car_control.dir/src/encoder.cpp.o: CMakeFiles/smart_car_control.dir/flags.make
CMakeFiles/smart_car_control.dir/src/encoder.cpp.o: /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/encoder.cpp
CMakeFiles/smart_car_control.dir/src/encoder.cpp.o: CMakeFiles/smart_car_control.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/smart_car_control.dir/src/encoder.cpp.o"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/smart_car_control.dir/src/encoder.cpp.o -MF CMakeFiles/smart_car_control.dir/src/encoder.cpp.o.d -o CMakeFiles/smart_car_control.dir/src/encoder.cpp.o -c /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/encoder.cpp

CMakeFiles/smart_car_control.dir/src/encoder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/smart_car_control.dir/src/encoder.cpp.i"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/encoder.cpp > CMakeFiles/smart_car_control.dir/src/encoder.cpp.i

CMakeFiles/smart_car_control.dir/src/encoder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/smart_car_control.dir/src/encoder.cpp.s"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/encoder.cpp -o CMakeFiles/smart_car_control.dir/src/encoder.cpp.s

CMakeFiles/smart_car_control.dir/src/key.cpp.o: CMakeFiles/smart_car_control.dir/flags.make
CMakeFiles/smart_car_control.dir/src/key.cpp.o: /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/key.cpp
CMakeFiles/smart_car_control.dir/src/key.cpp.o: CMakeFiles/smart_car_control.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/smart_car_control.dir/src/key.cpp.o"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/smart_car_control.dir/src/key.cpp.o -MF CMakeFiles/smart_car_control.dir/src/key.cpp.o.d -o CMakeFiles/smart_car_control.dir/src/key.cpp.o -c /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/key.cpp

CMakeFiles/smart_car_control.dir/src/key.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/smart_car_control.dir/src/key.cpp.i"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/key.cpp > CMakeFiles/smart_car_control.dir/src/key.cpp.i

CMakeFiles/smart_car_control.dir/src/key.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/smart_car_control.dir/src/key.cpp.s"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/key.cpp -o CMakeFiles/smart_car_control.dir/src/key.cpp.s

CMakeFiles/smart_car_control.dir/src/main.cpp.o: CMakeFiles/smart_car_control.dir/flags.make
CMakeFiles/smart_car_control.dir/src/main.cpp.o: /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/main.cpp
CMakeFiles/smart_car_control.dir/src/main.cpp.o: CMakeFiles/smart_car_control.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/smart_car_control.dir/src/main.cpp.o"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/smart_car_control.dir/src/main.cpp.o -MF CMakeFiles/smart_car_control.dir/src/main.cpp.o.d -o CMakeFiles/smart_car_control.dir/src/main.cpp.o -c /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/main.cpp

CMakeFiles/smart_car_control.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/smart_car_control.dir/src/main.cpp.i"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/main.cpp > CMakeFiles/smart_car_control.dir/src/main.cpp.i

CMakeFiles/smart_car_control.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/smart_car_control.dir/src/main.cpp.s"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/main.cpp -o CMakeFiles/smart_car_control.dir/src/main.cpp.s

CMakeFiles/smart_car_control.dir/src/pid.cpp.o: CMakeFiles/smart_car_control.dir/flags.make
CMakeFiles/smart_car_control.dir/src/pid.cpp.o: /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/pid.cpp
CMakeFiles/smart_car_control.dir/src/pid.cpp.o: CMakeFiles/smart_car_control.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/smart_car_control.dir/src/pid.cpp.o"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/smart_car_control.dir/src/pid.cpp.o -MF CMakeFiles/smart_car_control.dir/src/pid.cpp.o.d -o CMakeFiles/smart_car_control.dir/src/pid.cpp.o -c /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/pid.cpp

CMakeFiles/smart_car_control.dir/src/pid.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/smart_car_control.dir/src/pid.cpp.i"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/pid.cpp > CMakeFiles/smart_car_control.dir/src/pid.cpp.i

CMakeFiles/smart_car_control.dir/src/pid.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/smart_car_control.dir/src/pid.cpp.s"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/pid.cpp -o CMakeFiles/smart_car_control.dir/src/pid.cpp.s

CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.o: CMakeFiles/smart_car_control.dir/flags.make
CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.o: /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/pwm_ctrl.cpp
CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.o: CMakeFiles/smart_car_control.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.o"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.o -MF CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.o.d -o CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.o -c /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/pwm_ctrl.cpp

CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.i"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/pwm_ctrl.cpp > CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.i

CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.s"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/pwm_ctrl.cpp -o CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.s

CMakeFiles/smart_car_control.dir/src/register.cpp.o: CMakeFiles/smart_car_control.dir/flags.make
CMakeFiles/smart_car_control.dir/src/register.cpp.o: /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/register.cpp
CMakeFiles/smart_car_control.dir/src/register.cpp.o: CMakeFiles/smart_car_control.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/smart_car_control.dir/src/register.cpp.o"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/smart_car_control.dir/src/register.cpp.o -MF CMakeFiles/smart_car_control.dir/src/register.cpp.o.d -o CMakeFiles/smart_car_control.dir/src/register.cpp.o -c /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/register.cpp

CMakeFiles/smart_car_control.dir/src/register.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/smart_car_control.dir/src/register.cpp.i"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/register.cpp > CMakeFiles/smart_car_control.dir/src/register.cpp.i

CMakeFiles/smart_car_control.dir/src/register.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/smart_car_control.dir/src/register.cpp.s"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/register.cpp -o CMakeFiles/smart_car_control.dir/src/register.cpp.s

CMakeFiles/smart_car_control.dir/src/tty.cpp.o: CMakeFiles/smart_car_control.dir/flags.make
CMakeFiles/smart_car_control.dir/src/tty.cpp.o: /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/tty.cpp
CMakeFiles/smart_car_control.dir/src/tty.cpp.o: CMakeFiles/smart_car_control.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/smart_car_control.dir/src/tty.cpp.o"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/smart_car_control.dir/src/tty.cpp.o -MF CMakeFiles/smart_car_control.dir/src/tty.cpp.o.d -o CMakeFiles/smart_car_control.dir/src/tty.cpp.o -c /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/tty.cpp

CMakeFiles/smart_car_control.dir/src/tty.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/smart_car_control.dir/src/tty.cpp.i"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/tty.cpp > CMakeFiles/smart_car_control.dir/src/tty.cpp.i

CMakeFiles/smart_car_control.dir/src/tty.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/smart_car_control.dir/src/tty.cpp.s"
	loongarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/src/tty.cpp -o CMakeFiles/smart_car_control.dir/src/tty.cpp.s

# Object files for target smart_car_control
smart_car_control_OBJECTS = \
"CMakeFiles/smart_car_control.dir/src/GPIO.cpp.o" \
"CMakeFiles/smart_car_control.dir/src/core.cpp.o" \
"CMakeFiles/smart_car_control.dir/src/encoder.cpp.o" \
"CMakeFiles/smart_car_control.dir/src/key.cpp.o" \
"CMakeFiles/smart_car_control.dir/src/main.cpp.o" \
"CMakeFiles/smart_car_control.dir/src/pid.cpp.o" \
"CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.o" \
"CMakeFiles/smart_car_control.dir/src/register.cpp.o" \
"CMakeFiles/smart_car_control.dir/src/tty.cpp.o"

# External object files for target smart_car_control
smart_car_control_EXTERNAL_OBJECTS =

smart_car_control: CMakeFiles/smart_car_control.dir/src/GPIO.cpp.o
smart_car_control: CMakeFiles/smart_car_control.dir/src/core.cpp.o
smart_car_control: CMakeFiles/smart_car_control.dir/src/encoder.cpp.o
smart_car_control: CMakeFiles/smart_car_control.dir/src/key.cpp.o
smart_car_control: CMakeFiles/smart_car_control.dir/src/main.cpp.o
smart_car_control: CMakeFiles/smart_car_control.dir/src/pid.cpp.o
smart_car_control: CMakeFiles/smart_car_control.dir/src/pwm_ctrl.cpp.o
smart_car_control: CMakeFiles/smart_car_control.dir/src/register.cpp.o
smart_car_control: CMakeFiles/smart_car_control.dir/src/tty.cpp.o
smart_car_control: CMakeFiles/smart_car_control.dir/build.make
smart_car_control: /home/cys/Project_Date/LS2K0300_workspace/opencv/install/lib/libopencv_core.so
smart_car_control: /home/cys/Project_Date/LS2K0300_workspace/opencv/install/lib/libopencv_imgproc.so
smart_car_control: /home/cys/Project_Date/LS2K0300_workspace/opencv/install/lib/libopencv_imgcodecs.so
smart_car_control: /home/cys/Project_Date/LS2K0300_workspace/opencv/install/lib/libopencv_highgui.so
smart_car_control: /home/cys/Project_Date/LS2K0300_workspace/opencv/install/lib/libopencv_videoio.so
smart_car_control: CMakeFiles/smart_car_control.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable smart_car_control"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/smart_car_control.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/smart_car_control.dir/build: smart_car_control
.PHONY : CMakeFiles/smart_car_control.dir/build

CMakeFiles/smart_car_control.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/smart_car_control.dir/cmake_clean.cmake
.PHONY : CMakeFiles/smart_car_control.dir/clean

CMakeFiles/smart_car_control.dir/depend:
	cd /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build /home/cys/Project_Date/Smart_Car_workspace/dinosaur_smartcar/build/CMakeFiles/smart_car_control.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/smart_car_control.dir/depend

