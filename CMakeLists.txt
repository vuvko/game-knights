cmake_minimum_required(VERSION 2.6)

# Project name
project(shadrikov)

# Compiler flags
add_definitions(-O2 -Wall)
    
include_directories(${CMAKE_CURRENT_LIST_DIR})
# Source files
aux_source_directory(. SRC_LIST)
aux_source_directory(./support SUPPORT_SRC)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${SUPPORT_SRC})

