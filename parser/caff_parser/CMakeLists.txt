# Create a library
SET(lib_sources reader.c lexer.c parser.c)
add_library(caff_parser_static ${lib_sources})

# Make it available for linking
target_include_directories(caff_parser_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Create a sharad library as well
add_library(caff_parser_shared SHARED ${lib_sources})