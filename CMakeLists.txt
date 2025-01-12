cmake_minimum_required(VERSION 3.15)
project(vector LANGUAGES CXX)

OPTION(DEBUG "Enable debug mode" ON)

add_subdirectory(lib)
add_subdirectory(cli)

if(DEBUG)
    message(STATUS "Debug mode enabled")
    add_compile_definitions(VECTOR_DEBUG)
endif()
