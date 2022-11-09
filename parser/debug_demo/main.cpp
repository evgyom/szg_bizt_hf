#include "../caff_parser/parser.h"

int main(){

    struct_out_t struct_out;
    char input_path[] = "../../afl_test/past_crashes/id:000005,sig:11,src:000068,op:flip1,pos:85";
    char out_path[] = "test.gif"; 
    int file_size = 1000000;

    parser_status_t stat = parse(input_path, file_size, out_path, &struct_out);
    printf("Status: %d\n", stat);
    printf("Number of frames: %d\n", struct_out.n_frames);
    printf("Created: %d.%d.%d - %d:%d\n", 
        struct_out.creation_year, 
        struct_out.creation_month, 
        struct_out.creation_day, 
        struct_out.creation_hour,
        struct_out.creation_minute);

    return 0;
}