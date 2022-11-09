#include "../caff_parser/parser.h"

int main( int argc, char *argv[]){

    struct_out_t struct_out;
    char out_path[] = "test.gif"; 
    int file_size = 4002260;

    int stat = parse(argv[0], file_size, out_path, &struct_out);

    return 0;
}