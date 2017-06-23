cmake_minimum_required(VERSION 3.7)
project(PC_ROBOT)

set(CMAKE_CXX_STANDARD 11)

# put your needed flags here
set(COMMON_FLAGS "")
set(CMAKE_CXX_FLAGS "-std=c++11 ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# we just grab all the cpp and h files. not ideal, but works well enough
# means a "reload CMake project" when you add new files
file(GLOB_RECURSE ROBOT_FILES *.cpp *.h)
file(GLOB_RECURSE NETWORK_FILES Netzwerk/*.c Netzwerk/*.h)
include_directories(Netzwerk)
include_directories(SYSTEM include)
# custom target to build within the IDE using make
# special target in the Makefile symlinks the executable built
# into the place CLion expects it (see below)
add_custom_target(PC_ROBOT
        make Robot -C /home/stephan/Bachelorarbeit/PC CLION_DIR=${PROJECT_BINARY_DIR}
        COMMENT making Robot
        WORKING_DIRECTORY ${heimdall_SOURCE_DIR})
add_custom_target(PC_Netzwerk
        make -C /home/stephan/Bachelorarbeit/PC/Netzwerk CLION_DIR=${PROJECT_BINARY_DIR}
        COMMENT making Netzwerk
        WORKING_DIRECTORY ${heimdall_SOURCE_DIR})
# fake target here is never actually built; but has to be here in order for
# CLion to "see" the files and index them.
add_executable(fake-target-for-clion ${ROBOT_FILES} ${NETWORK_FILES})