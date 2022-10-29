#include "lexer.h"

#include <string.h>

#define BUF_SIZE 50

caff_frame_t get_header(FILE * fp, int64_t * num_anims){
    char buffer[BUF_SIZE];

    // Find the start of the header
    int len_of_return_buf;
    reader_search_character(fp, buffer, BUF_SIZE, &len_of_return_buf, CAFF_HEADER_ID, BUF_SIZE);

    // Get the length of the header
    int64_t size_of_frame;
    reader_consume(fp, CAFF_FRAME_LENGTH_SIZE, buffer, BUF_SIZE);
    memcpy(size_of_frame.array, buffer, CAFF_FRAME_LENGTH_SIZE);

    // Load the CAFF Magic part of sthe frame
    reader_consume(fp, CAFF_HEADER_MAGIC_SIZE, buffer, BUF_SIZE);
    // If the next characters are not 'CAFF' return an INVALID_FRAME error
    if(strcmp(buffer, "CAFF") != 0){
        return INVALID_FRAME;
    }

    // Load the header_size part of the frame
    reader_consume(fp, CAFF_HEADER_HEADER_SIZE_SIZE, buffer, BUF_SIZE);
    int64_t size_of_header;
    memcpy(size_of_header.array, buffer, CAFF_HEADER_HEADER_SIZE_SIZE);
    // If the frame size and the header size don't match, the frame is invalid
    if(size_of_header.long_long != size_of_frame.long_long){
        return INVALID_FRAME;
    }

    // Load the num_anim part of the frame
    reader_consume(fp, CAFF_HEADER_NUM_ANIM_SIZE, buffer, BUF_SIZE);
    memcpy(num_anims->array, buffer, CAFF_FRAME_LENGTH_SIZE);

    return FRAME_OK;
}

char * str_in_buf(char * buffer, int buffer_size, char * string, int string_size){
    int i, j;
    for(i = 0; i<buffer_size-string_size+1; i++){
        if(buffer[i] == string[0]){
            for(j = 0; j<string_size; j++){
                if(buffer[i+j] != string[j])
                    break;
            }
            return &buffer[i];
        }
    }
    return NULL;
}

long long arr_to_ll(char * buffer){
    long long retval;
    
    // Check endiannes
    int i;
    if(buffer[0] == 0 && buffer[7] != 0){
        // Big-endian
        for(i = 0; i<8; i++){
            i += buffer[i] * (0xFF << i);
        }
    }else{
        // Little-endian
        // If all bytes are non-zero, we assume that the byte order is little-endian
        for(i = 0; i<8; i++){
            i += buffer[i] * (0xFF << (8-i));
        }
    }
    
}