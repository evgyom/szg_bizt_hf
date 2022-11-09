cmake_minimum_required(VERSION 3.0.0)
project(caff_parser VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector-strong")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-strong")

add_subdirectory(caff_parser)
add_subdirectory(tests)
add_subdirectory(debug_demo)