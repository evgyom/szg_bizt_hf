#include "lexer.h"

#define BUF_SIZE 50

frame_status_t process_header(FILE * fp, long long  * num_anims){
    char buffer[BUF_SIZE];

    // Verify frame ID
    reader_consume(fp, CAFF_FRAME_ID_BYTES, buffer, BUF_SIZE);
    if(buffer[0] != CAFF_HEADER_ID)
        return INVALID_ID;

    // Get the length of the header
    long long size_of_frame;
    reader_consume(fp, CAFF_FRAME_LENGTH_BYTES, buffer, BUF_SIZE);
    size_of_frame = arr_to_ll(buffer);

    // Load the CAFF Magic part of sthe frame
    reader_consume(fp, CAFF_HEADER_MAGIC_BYTES, buffer, BUF_SIZE);
    // If the next characters are not 'CAFF' return an INVALID_FRAME error
    if(strcmp(buffer, "CAFF") != 0)
        return INVALID_CAFF_MAGIC;

    // Load the header_size part of the frame
    reader_consume(fp, CAFF_HEADER_HEADER_SIZE_BYTES, buffer, BUF_SIZE);
    long long size_of_header;
    size_of_header = arr_to_ll(buffer);
    printf("Size of header: %d\n", size_of_header);
    // If the frame size and the header size don't match, the frame is invalid
    if(size_of_header != size_of_frame)
        return INVALID_FRAME_SIZES;

    // Load the num_anim part of the frame
    reader_consume(fp, CAFF_HEADER_NUM_ANIM_BYTES, buffer, BUF_SIZE);
    * num_anims = arr_to_ll(buffer);

    return FRAME_OK;
}

frame_status_t process_credits(FILE * fp, char * date, char * creator_buffer, int creator_buffer_size, long long * creator_name_length){
    char buffer[BUF_SIZE];

    // Get the length of the credits frame
    long long size_of_frame;
    reader_consume(fp, CAFF_FRAME_LENGTH_BYTES, buffer, BUF_SIZE);
    size_of_frame = arr_to_ll(buffer);
    printf("Size of frame: %d\n", size_of_frame);

    // Get the date of creation
    reader_consume(fp, CAFF_CREDITS_DATE_BYTES, buffer, BUF_SIZE);
    memcpy(buffer, date, CAFF_CREDITS_DATE_BYTES);

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

long long arr_to_ll(unsigned char * buffer){
    long long retval = 0;
    
    // Enforce little-endiannes
    int i;
    for(i = 0; i<8; i++){
        retval += buffer[i] * (1 << (8*i));
    }

    return retval;    
}