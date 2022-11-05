set(CMAKE_C_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY "https://github.com/google/googletest"
    GIT_TAG "main"
    CONFIGURE_COMMAND ""
  )
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(reader_test "reader_test.cc")
target_include_directories(reader_test PRIVATE caff_parser)
target_link_libraries(reader_test GTest::gtest_main caff_parser_static)

add_executable(lexer_test "lexer_test.cc")
target_include_directories(lexer_test PRIVATE caff_parser)
target_link_libraries(lexer_test GTest::gtest_main caff_parser_static)

include(GoogleTest)
gtest_discover_tests(reader_test)
gtest_discover_tests(lexer_test)