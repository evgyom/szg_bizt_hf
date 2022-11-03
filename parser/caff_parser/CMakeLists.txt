# Create a library
add_library(caff_parser reader.c lexer.c)

# Make it available for linking
target_include_directories (caff_parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})