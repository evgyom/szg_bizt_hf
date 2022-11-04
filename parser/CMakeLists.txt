cmake_minimum_required(VERSION 3.0.0)
project(caff_parser VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_subdirectory(demo)
add_subdirectory(caff_parser)
#add_subdirectory(tests)