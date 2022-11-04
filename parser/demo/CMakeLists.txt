# Add execuatable that includes the source file
add_executable (parser_demo main.c)

# Link the executable to the caff_parser library
target_link_libraries(parser_demo LINK_PUBLIC caff_parser_static)