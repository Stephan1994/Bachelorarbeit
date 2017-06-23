#cmake_minimum_required(VERSION 3.7)
#project(PC)
#
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#
#add_custom_target(PC all COMMAND make -C ${/home/stephan/Bachelorarbeit/PC} CLION_EXE_DIR=${PROJECT_BINARY_DIR})
#set(SOURCE_FILES main.cpp Robot.cpp Robot.h Funktion_zum_Umwandeln.c Netzwerk.c Netzwerk.h ProzessPi.c ProzessPi.h)
#add_subdirectory(/home/stephan/Bachelorarbeit/PC ${CMAKE_CURRENT_BINARY_DIR}/Netzwerk)
#add_executable(PC ${SOURCE_FILES})
cmake_minimum_required(VERSION 2.8.4)
project(ProjectName)

# put your needed flags here
set(COMMON_FLAGS "")
set(CMAKE_CXX_FLAGS "-std=c++1z ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
set(CMAKE_C_COMPILER path/to/gcc)
set(CMAKE_CXX_COMPILER path/to/g++)

# we just grab all the cpp and h files. not ideal, but works well enough
# means a "reload CMake project" when you add new files
file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)
file(GLOB_RECURSE TEST_FILES test/*.cpp test/*.h)
include_directories(src test)
include_directories(SYSTEM include)
# custom target to build within the IDE using make
# special target in the Makefile symlinks the executable built
# into the place CLion expects it (see below)
add_custom_target(project-exe
        make -j50 ddist-clion CLION_DIR=${PROJECT_BINARY_DIR}
        COMMENT making ddist-clion
        WORKING_DIRECTORY ${heimdall_SOURCE_DIR})
add_custom_target(project-test
        make -j50 debug-test-clion CLION_DIR=${PROJECT_BINARY_DIR}
        COMMENT making debug-test-clion
        WORKING_DIRECTORY ${heimdall_SOURCE_DIR})
# fake target here is never actually built; but has to be here in order for
# CLion to "see" the files and index them.
add_executable(fake-target-for-clion ${SOURCE_FILES} ${TEST_FILES})