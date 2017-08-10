cmake_minimum_required(VERSION 3.7)
project(PC)

set(CMAKE_CXX_STANDARD 11)

find_package( OpenCV REQUIRED )

include_directories ("${PROJECT_SOURCE_DIR}/Netzwerk")
add_subdirectory (Netzwerk)

set(SOURCE_FILES
        main.cpp
        Robot.cpp
        Robot.h
        MessageListener.cpp
        MessageListener.h
        ProtocolLibrary.cpp
        ProtocolLibrary.h)

set(COMMON_FLAGS "")
set(CMAKE_CXX_FLAGS "-std=c++11 -lpthread ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

add_executable(PC ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( PC ${OpenCV_LIBS} )
target_link_libraries (PC Netzwerk)