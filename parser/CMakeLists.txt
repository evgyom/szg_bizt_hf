cmake_minimum_required(VERSION 3.0.0)
project(caff_parser VERSION 0.1.0)

include_directories(include)
file(GLOB SOURCES "source/*.c")
add_executable(caff_parser ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
