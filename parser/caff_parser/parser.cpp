#include <stdio.h>
#include <stdlib.h>
#include "gif.h"
#include "parser.h"

#define GIF_BIT_DEPTH 8

int parse(char * ciff_path, long long file_size, char * gif_path, struct_out_t * struct_out){

    // Open CAFF
    FILE * fp = fopen(ciff_path, "rb");
    if(fp == NULL){
        return PARSER_FP_NULL;
    }
    file_status_t file_stat = {fp, file_size, 0};

    // Read header
    long long num_anims;
    frame_status_t stat = process_header(&file_stat, &num_anims);
    if(stat != LEXER_FRAME_OK){
        fclose(fp);
        if(stat == LEXER_FP_ERROR)
            return PARSER_FP_NULL;
        if(stat == LEXER_BUFFER_SIZE_ERROR)
            return PARSER_BUFFER_SIZE_ERROR;
        if(stat == LEXER_EOF_REACHED)
            return PARSER_EOF_REACHED;
        if(stat == LEXER_INVALID_ID)
            return CAFF_FORMAT_ERROR;
        if(stat == LEXER_INVALID_CAFF_MAGIC)
            return CAFF_FORMAT_ERROR;
        if(stat == LEXER_INVALID_SIZES)
            return CAFF_FORMAT_ERROR;
        return PARSER_GENERAL_ERROR;
    }    
    struct_out->n_frames = (int) num_anims;
    
    // Read credits
    unsigned char date_buffer[BUF_SIZE];
    unsigned char creator_buffer[CREATOR_BUF_SIZE];
    long long creator_name_len;
    stat = process_credits(&file_stat, date_buffer, creator_buffer, CREATOR_BUF_SIZE, &creator_name_len);
    if(stat != LEXER_FRAME_OK){
        fclose(fp);
        if(stat == LEXER_FP_ERROR)
            return PARSER_FP_NULL;
        if(stat == LEXER_BUFFER_SIZE_ERROR)
            return PARSER_BUFFER_SIZE_ERROR;
        if(stat == LEXER_EOF_REACHED)
            return PARSER_EOF_REACHED;
        if(stat == LEXER_INVALID_ID)
            return CAFF_FORMAT_ERROR;
        if(stat == LEXER_INVALID_CAFF_MAGIC)
            return CAFF_FORMAT_ERROR;
        if(stat == LEXER_INVALID_SIZES)
            return CAFF_FORMAT_ERROR;
        if(stat == LEXER_CREATOR_BUFFER_ERROR)
            return CAFF_FIELD_SIZE_ERROR;
        if(stat == LEXER_CREATOR_NON_ASCII)
            return CAFF_FORMAT_ERROR;
        if(stat == LEXER_CREATOR_NAME_TOO_LONG)
            return CAFF_FORMAT_ERROR;
        return PARSER_GENERAL_ERROR;
    }
    
    // Copy the creator name details
    if(creator_name_len > CREATOR_BUF_SIZE){
        fclose(fp);
        return CAFF_FIELD_SIZE_ERROR;
    }        
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
    my_ciff.content_buffer_ptr = static_cast<unsigned char *>(malloc(3 * CIFF_MAX_PIXELS * sizeof(char)));
    my_ciff.gif_content_buffer_ptr = static_cast<unsigned char *>(malloc(4 * CIFF_MAX_PIXELS * sizeof(char)));
    stat = process_ciff_frame(&file_stat, &my_ciff);
    if(stat != LEXER_FRAME_OK){
        fclose(fp);
        free(my_ciff.content_buffer_ptr);
        free(my_ciff.gif_content_buffer_ptr);
        if(stat == LEXER_FP_ERROR)
            return PARSER_FP_NULL;
        if(stat == LEXER_BUFFER_SIZE_ERROR)
            return PARSER_BUFFER_SIZE_ERROR;
        if(stat == LEXER_EOF_REACHED)
            return PARSER_EOF_REACHED;
        if(stat == LEXER_INVALID_ID)
            return CAFF_FORMAT_ERROR;
        if(stat == LEXER_INVALID_CIFF_MAGIC)
            return CAFF_FORMAT_ERROR;
        if(stat == LEXER_INVALID_CIFF_CONTENT_SIZE)
            return CAFF_FORMAT_ERROR;
        if(stat == LEXER_CIFF_CAPTION_NOT_TERMINATED)
            return CAFF_FORMAT_ERROR;
        if(stat == LEXER_CIFF_CAPTIONS_TOO_LONG)
            return CAFF_FIELD_SIZE_ERROR;
        if(stat == LEXER_CIFF_TAGS_TOO_LONG)
            return CAFF_FIELD_SIZE_ERROR;
        if(stat == LEXER_CIFF_INVALID_NEW_LINE)
            return CAFF_FORMAT_ERROR;
        if(stat == LEXER_CIFF_CONTENT_SIZE_ERROR)
            return CAFF_FIELD_SIZE_ERROR;
        return PARSER_GENERAL_ERROR;
    }
    // Check if the CIFFs fit into the file
    if(my_ciff.height * my_ciff.width * 3 * num_anims > file_stat.file_size){
        fclose(fp);
        free(my_ciff.content_buffer_ptr);
        free(my_ciff.gif_content_buffer_ptr);
        return CAFF_FORMAT_ERROR;
    }

    //Copy data to output structure
    struct_out->frame_height = my_ciff.height;
    struct_out->frame_width = my_ciff.width;
    struct_out->total_duration = my_ciff.duration;
    // Copy the first caption and the first tags
    if(my_ciff.captions_length > CAPTIONS_BUFFER_SIZE || my_ciff.tags_length > TAGS_BUFFER_SIZE){
        fclose(fp);
        free(my_ciff.content_buffer_ptr);
        free(my_ciff.gif_content_buffer_ptr);
        return CAFF_FIELD_SIZE_ERROR;
    }
    struct_out->captions_length = my_ciff.captions_length;
    struct_out-> tags_length = my_ciff.tags_length;
    struct_out->n_tags = my_ciff.number_of_tags;
    memcpy(struct_out->captions, my_ciff.captions_buffer, my_ciff.captions_length);
    memcpy(struct_out->tags, my_ciff.tags_buffer, my_ciff.tags_length);

    //Create empty GIF
    GifWriter writer = {};
    GifBegin(&writer, gif_path, my_ciff.width, my_ciff.height, my_ciff.duration, GIF_BIT_DEPTH, true);
    
    // Add alpha channel to the pixels, for the library
    if(add_alpha_to_rgb(my_ciff, my_ciff.width * my_ciff.height)){
        fclose(fp);
        free(my_ciff.content_buffer_ptr);
        free(my_ciff.gif_content_buffer_ptr);
        return PARSER_GENERAL_ERROR;
    }
    GifWriteFrame(&writer, my_ciff.gif_content_buffer_ptr, my_ciff.width, my_ciff.height, my_ciff.duration/10, GIF_BIT_DEPTH, true);

    // Read the remaining CIFFs
    for(int i = 1; i<num_anims; i++){
        stat = process_ciff_frame(&file_stat, &my_ciff);
        if(stat != LEXER_FRAME_OK){
            fclose(fp);
            free(my_ciff.content_buffer_ptr);
            free(my_ciff.gif_content_buffer_ptr);
            if(stat == LEXER_FP_ERROR)
                return PARSER_FP_NULL;
            if(stat == LEXER_BUFFER_SIZE_ERROR)
                return PARSER_BUFFER_SIZE_ERROR;
            if(stat == LEXER_EOF_REACHED)
                return PARSER_EOF_REACHED;
            if(stat == LEXER_INVALID_ID)
                return CAFF_FORMAT_ERROR;
            if(stat == LEXER_INVALID_CIFF_MAGIC)
                return CAFF_FORMAT_ERROR;
            if(stat == LEXER_INVALID_CIFF_CONTENT_SIZE)
                return CAFF_FORMAT_ERROR;
            if(stat == LEXER_CIFF_CAPTION_NOT_TERMINATED)
                return CAFF_FORMAT_ERROR;
            if(stat == LEXER_CIFF_CAPTIONS_TOO_LONG)
                return CAFF_FIELD_SIZE_ERROR;
            if(stat == LEXER_CIFF_TAGS_TOO_LONG)
                return CAFF_FIELD_SIZE_ERROR;
            if(stat == LEXER_CIFF_INVALID_NEW_LINE)
                return CAFF_FORMAT_ERROR;
            if(stat == LEXER_CIFF_CONTENT_SIZE_ERROR)
                return CAFF_FIELD_SIZE_ERROR;
            return PARSER_GENERAL_ERROR;
        }
        // Update output struct with the duration
        struct_out->total_duration += my_ciff.duration;
        // Check if the new ciff has the same dimensions
        if(my_ciff.width != struct_out->frame_width  || my_ciff.height !=  struct_out->frame_height){
            fclose(fp);
            free(my_ciff.content_buffer_ptr);
            free(my_ciff.gif_content_buffer_ptr);
            return CAFF_FORMAT_ERROR;
        }
        // Add alpha channel to the pixels, for the library
        if(add_alpha_to_rgb(my_ciff, my_ciff.width * my_ciff.height)){
            fclose(fp);
            free(my_ciff.content_buffer_ptr);
            free(my_ciff.gif_content_buffer_ptr);
            return PARSER_GENERAL_ERROR;
        }            
        // Add the new frame to the GIF
        GifWriteFrame(&writer, my_ciff.gif_content_buffer_ptr, my_ciff.width, my_ciff.height, my_ciff.duration/10, GIF_BIT_DEPTH, true);
    } 

    // Write EOF
    GifEnd(&writer);

    // Free the buffers
    free(my_ciff.content_buffer_ptr);
    free(my_ciff.gif_content_buffer_ptr);

    // Close the file 
    fclose(fp);
    return 0;
}
