#ifndef PARSER_H
#define PARSER_H

extern "C"{
    #include "lexer.h"
}

typedef struct{
    int n_frames;
    int creator_name_length;
    unsigned char creator_name[CREATOR_BUF_SIZE];
    int creation_year;
    int creation_month;
    int creation_day; 
    int creation_hour;
    int creation_minute;
    unsigned char captions[CAPTIONS_BUFFER_SIZE];
    int captions_length;
    unsigned char tags[TAGS_BUFFER_SIZE]; 
    int tags_length;
    int n_tags;
    int total_duration;
    int frame_height;
    int frame_width;
}struct_out_t;

extern "C"{
    #include "lexer.h"
    //Declare function as c to make it available for python
    int parse(char * ciff_path, long long file_size, char * gif_path,  struct_out_t * struct_out);
}

typedef enum{
    PARSER_FP_NULL,
    PARSER_BUFFER_SIZE_ERROR,
    PARSER_EOF_REACHED,
    CAFF_FORMAT_ERROR,
    CAFF_FIELD_SIZE_ERROR,
    PARSER_GENERAL_ERROR
}parser_status_t;

#endif