#ifndef PARSER_H
#define PARSER_H

extern "C"{
    #include "lexer.h"
}

typedef struct{
    unsigned int n_frames;
    unsigned int creator_name_length;
    unsigned char creator_name[CREATOR_BUF_SIZE];
    unsigned int creation_year;
    unsigned int creation_month;
    unsigned int creation_day; 
    unsigned int creation_hour;
    unsigned int creation_minute;
    unsigned char captions[CAPTIONS_BUFFER_SIZE];
    unsigned int captions_length;
    unsigned char tags[TAGS_BUFFER_SIZE]; 
    unsigned int tags_length;
    unsigned int n_tags;
    int total_duration;
    unsigned int frame_height;
    unsigned int frame_width;
}struct_out_t;

typedef enum{
    PARSER_STATUS_OK,
    PARSER_FP_NULL,
    PARSER_BUFFER_SIZE_ERROR,
    PARSER_EOF_REACHED,
    CAFF_FORMAT_ERROR,
    CAFF_FIELD_SIZE_ERROR,
    CAFF_UNDEFINED_SECTION,
    PARSER_GENERAL_ERROR
}parser_status_t;

extern "C"{
    #include "lexer.h"
    //Declare function as c to make it available for python
    parser_status_t parse(char * ciff_path, unsigned long long file_size, char * gif_path,  struct_out_t * struct_out);
}

#endif