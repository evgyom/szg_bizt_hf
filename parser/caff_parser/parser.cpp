#include <stdio.h>
#include <stdlib.h>
#include "gif.h"
#include "parser.h"

#define GIF_BIT_DEPTH 8

int parse(char * ciff_path, long long file_size, char * gif_path, struct_out_t * struct_out){

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
    struct_out->n_frames = (int) num_anims;
    
    // Read credits
    unsigned char date_buffer[BUF_SIZE];
    unsigned char creator_buffer[CREATOR_BUF_SIZE];
    long long creator_name_len;
    stat = process_credits(&file_stat, date_buffer, creator_buffer, CREATOR_BUF_SIZE, &creator_name_len);
    if(stat != LEXER_FRAME_OK)
        return 1;
    // Copy the creator name details
    if(creator_name_len > CREATOR_BUF_SIZE)
        return 1;
    struct_out->creator_name_length = (int) creator_name_len;
    memcpy(struct_out->creator_name, creator_buffer, creator_name_len);
    // Copy the date to the output structure
    struct_out->creation_year = date_buffer[0] + date_buffer[1] * (1 << 8);
    struct_out->creation_month = date_buffer[2];
    struct_out->creation_day = date_buffer[3];
    struct_out->creation_hour = date_buffer[4];
    struct_out->creation_minute = date_buffer[5];

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

    //Copy data to output structure
    struct_out->frame_height = ciff_height;
    struct_out->frame_width = ciff_width;
    struct_out->total_duration = my_ciff.duration;

    //Create empty GIF
    GifWriter writer = {};
    GifBegin(&writer, gif_path, my_ciff.width, my_ciff.height, my_ciff.duration, GIF_BIT_DEPTH, true);
    
    // Add alpha channel to the pixels, for the library
    if(add_alpha_to_rgb(my_ciff, my_ciff.width * my_ciff.height))
        return 1;
    GifWriteFrame(&writer, my_ciff.gif_content_buffer_ptr, my_ciff.width, my_ciff.height, my_ciff.duration/10, GIF_BIT_DEPTH, true);

    // Read the remaining CIFFs
    for(int i = 1; i<num_anims; i++){
        stat = process_ciff_frame(&file_stat, &my_ciff);

        // Update output struct
        struct_out->total_duration += my_ciff.duration;

        if(stat != LEXER_FRAME_OK)
            return 1;
        // Check if the new ciff has the same dimensions
        if(my_ciff.width != ciff_width || my_ciff.height != ciff_height)
            return 2;
        // Add alpha channel to the pixels, for the library
        if(add_alpha_to_rgb(my_ciff, my_ciff.width * my_ciff.height))
            return 1;
        GifWriteFrame(&writer, my_ciff.gif_content_buffer_ptr, my_ciff.width, my_ciff.height, my_ciff.duration/10, GIF_BIT_DEPTH, true);
    } 

    // Write EOF
    GifEnd(&writer);

    // Free the buffers
    free(my_ciff.content_buffer_ptr);
    free(my_ciff.gif_content_buffer_ptr);

    return 0;
}
