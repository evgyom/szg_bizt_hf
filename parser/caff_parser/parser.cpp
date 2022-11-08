#include <stdio.h>
#include <stdlib.h>

extern "C"{
    // Include own c libraries
    #include "lexer.h"
    //Declare function as c to make it available for python
    int parse(char * ciff_path, long long file_size, char * gif_path,  char * creator_name_out);
}
#include "gif.h"

#define GIF_BIT_DEPTH 8

int parse(char * ciff_path, long long file_size, char * gif_path, char * creator_name_out){

    // Open CAFF
    FILE * fp = fopen(ciff_path, "rb");
    if(fp == NULL){
        return 1;
    }
    file_status_t file_stat = {fp, file_size, 0};

    // Read header
    long long num_anims;
    frame_status_t stat = process_header(&file_stat, &num_anims);    
    if(stat != LEXER_FRAME_OK)
        return 1;

    // Read credits
    unsigned char date_buffer[BUF_SIZE];
    unsigned char creator_buffer[CREATOR_BUF_SIZE];
    long long creator_name_len;
    stat = process_credits(&file_stat, date_buffer, creator_buffer, CREATOR_BUF_SIZE, &creator_name_len);
    if(stat != LEXER_FRAME_OK)
        return 1;

    // Copy the creator name to the allocated memory
    memcpy(creator_name_out, creator_buffer, creator_name_len);

    // Read the first ciff
    ciff_frame_t my_ciff;
    my_ciff.content_buffer_ptr = (unsigned char *) malloc(3 * CIFF_MAX_PIXELS * sizeof(char));
    my_ciff.gif_content_buffer_ptr = (unsigned char *) malloc(4 * CIFF_MAX_PIXELS * sizeof(char));
    stat = process_ciff_frame(&file_stat, &my_ciff);
    if(stat != LEXER_FRAME_OK){
        return 1;
    }
    int ciff_width = my_ciff.width;
    int ciff_height = my_ciff.height;

    //Create empty GIF
    GifWriter writer = {};
    GifBegin(&writer, gif_path, my_ciff.width, my_ciff.height, my_ciff.duration, GIF_BIT_DEPTH, true);
    
    // Add alpha channel to the pixels, for the library
    if(add_alpha_to_rgb(my_ciff, my_ciff.width * my_ciff.height))
        return 1;
    GifWriteFrame(&writer, my_ciff.gif_content_buffer_ptr, my_ciff.width, my_ciff.height, my_ciff.duration, GIF_BIT_DEPTH, true);

    // Read the remaining CIFFs
    for(int i = 1; i<num_anims; i++){
        stat = process_ciff_frame(&file_stat, &my_ciff);
        if(stat != LEXER_FRAME_OK)
            return 1;
        // Check if the new ciff has the same dimensions
        if(my_ciff.width != ciff_width || my_ciff.height != ciff_height)
            return 2;
        // Add alpha channel to the pixels, for the library
        if(add_alpha_to_rgb(my_ciff, my_ciff.width * my_ciff.height))
            return 1;
        GifWriteFrame(&writer, my_ciff.gif_content_buffer_ptr, my_ciff.width, my_ciff.height, my_ciff.duration, GIF_BIT_DEPTH, true);
    } 

    // Write EOF
    GifEnd(&writer);

    // Free the buffers
    free(my_ciff.content_buffer_ptr);
    free(my_ciff.gif_content_buffer_ptr);

    return 0;
}
