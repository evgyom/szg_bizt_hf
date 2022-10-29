#ifndef LEXER_H
#define LEXER_H

#include "reader.h"

#define CAFF_HEADER_ID 0x01
#define CAFF_CREADITS_ID 0x02
#define CAFF_ANIMATION_ID 0x03

#define CAFF_FRAME_ID_SIZE 1
#define CAFF_FRAME_LENGTH_SIZE 8

#define CAFF_HEADER_MAGIC_SIZE 4
#define CAFF_HEADER_HEADER_SIZE_SIZE 8
#define CAFF_HEADER_NUM_ANIM_SIZE 8

#define CAFF_CREDITS_YY_SIZE 2
#define CAFF_CREDITS_MO_SIZE 1
#define CAFF_CREDITS_D_SIZE 1
#define CAFF_CREDITS_H_SIZE 1
#define CAFF_CREDITS_MI_SIZE 1

#define CAFF_ANIMATION_DURATION_SIZE 8

typedef enum{
    FRAME_OK,
    INVALID_FRAME
}caff_frame_t;

typedef union{
    char array[8];
    long long long_long;
}int64_t;

caff_frame_t get_header(FILE * fp, int64_t * num_anims);

char * str_in_buf(char * buffer, int buffer_size, char * string, int string_size);

#endif