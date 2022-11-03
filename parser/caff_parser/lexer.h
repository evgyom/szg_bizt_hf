#ifndef LEXER_H
#define LEXER_H

#include <string.h>

#include "reader.h"

#define CAFF_HEADER_ID 0x01
#define CAFF_CREDITS_ID 0x02
#define CAFF_ANIMATION_ID 0x03

#define CAFF_FRAME_ID_SIZE 1
#define CAFF_FRAME_LENGTH_SIZE 8

#define CAFF_HEADER_MAGIC_SIZE 4
#define CAFF_HEADER_HEADER_SIZE_SIZE 8
#define CAFF_HEADER_NUM_ANIM_SIZE 8

#define CAFF_CREDITS_DATE_SIZE 6
#define CAFF_CREDITS_CREATOR_LEN_SIZE 8

#define CAFF_ANIMATION_DURATION_SIZE 8

typedef enum{
    FRAME_OK,
    BUFFER_TO_SMALL,
    INVALID_FRAME
}frame_status_t;

frame_status_t process_header(FILE * fp, long long * num_anims);

frame_status_t process_credits(FILE * fp, char * date, char * creator_buffer, int creator_buffer_size, long long * creator_name_length);

frame_status_t process_ciff_frame(FILE * fp);

char * str_in_buf(char * buffer, int buffer_size, char * string, int string_size);

long long arr_to_ll(char * buffer);

#endif