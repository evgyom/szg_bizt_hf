#include "lexer.h"

#define BUF_SIZE 50

frame_status_t process_header(FILE * fp, long long  * num_anims){
    char buffer[BUF_SIZE];

    // Find the start of the header
    int len_of_return_buf;
    reader_search_character(fp, buffer, BUF_SIZE, &len_of_return_buf, CAFF_HEADER_ID, BUF_SIZE);

    // Get the length of the header
    long long size_of_frame;
    reader_consume(fp, CAFF_FRAME_LENGTH_SIZE, buffer, BUF_SIZE);
    size_of_frame = arr_to_ll(buffer);
    printf("Size of frame: %d\n", size_of_frame);

    // Load the CAFF Magic part of sthe frame
    reader_consume(fp, CAFF_HEADER_MAGIC_SIZE, buffer, BUF_SIZE);
    // If the next characters are not 'CAFF' return an INVALID_FRAME error
    if(strcmp(buffer, "CAFF") != 0){
        return INVALID_FRAME;
    }

    // Load the header_size part of the frame
    reader_consume(fp, CAFF_HEADER_HEADER_SIZE_SIZE, buffer, BUF_SIZE);
    long long size_of_header;
    size_of_header = arr_to_ll(buffer);
    printf("Size of header: %d\n", size_of_header);
    // If the frame size and the header size don't match, the frame is invalid
    if(size_of_header != size_of_frame){
        return INVALID_FRAME;
    }

    // Load the num_anim part of the frame
    reader_consume(fp, CAFF_HEADER_NUM_ANIM_SIZE, buffer, BUF_SIZE);
    * num_anims = arr_to_ll(buffer);

    return FRAME_OK;
}

frame_status_t process_credits(FILE * fp, char * date, char * creator_buffer, int creator_buffer_size, long long * creator_name_length){
    char buffer[BUF_SIZE];

    // Find the start of the credits frame
    int len_of_return_buf;
    reader_search_character(fp, buffer, BUF_SIZE, &len_of_return_buf, CAFF_CREDITS_ID, BUF_SIZE);

    // Get the length of the credits frame
    long long size_of_frame;
    reader_consume(fp, CAFF_FRAME_LENGTH_SIZE, buffer, BUF_SIZE);
    size_of_frame = arr_to_ll(buffer);
    printf("Size of frame: %d\n", size_of_frame);

    // Get the date of creation
    reader_consume(fp, CAFF_CREDITS_DATE_SIZE, buffer, BUF_SIZE);
    memcpy(buffer, date, CAFF_CREDITS_DATE_SIZE);

    /*
    // Load the length of the creator section
    reader_consume(fp, CAFF_CREDITS_CREATOR_LEN_SIZE, buffer, BUF_SIZE);
    long long size_of_creator_name = arr_to_ll(buffer);
    if(size_of_creator_name > creator_buffer_size){
        return BUFFER_TO_SMALL;
    }

    // 
    reader_consume(fp, size_of_creator_name, buffer, BUF_SIZE);
    if(creator_name_length > creator_buffer_size){
        return BUFFER_TO_SMALL;
    }
    memcpy(buffer, creator_buffer, * creator_name_length);*/
    
    return FRAME_OK;
}

long long arr_to_ll(char * buffer){
    long long retval = 0;
    
    // Check endiannes
    int i;
    if(buffer[0] == 0 && buffer[7] != 0){
        // Big-endian
        for(i = 0; i<8; i++){
            retval += buffer[i]  << (8-i);
        }
    }else{
        // Little-endian
        // If all bytes are non-zero, we assume that the byte order is little-endian
        for(i = 0; i<8; i++){
            retval += buffer[i] << i;
        }
    }

    return retval;    
}

