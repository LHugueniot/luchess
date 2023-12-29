# Common ======================================================================
cmake_minimum_required(VERSION 3.25)

set(PROJECTNAME "LUCHESS")
project(${PROJECTNAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

# Src (core and app) ==========================================================
add_subdirectory(src)

# Tests =======================================================================
add_subdirectory(test)
