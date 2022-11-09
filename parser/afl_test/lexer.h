#ifndef LEXER_H
#define LEXER_H

#include "reader.h"

// Frame IDs
#define CAFF_HEADER_ID 0x01
#define CAFF_CREDITS_ID 0x02
#define CAFF_ANIMATION_ID 0x03

// CAFF fields sizes
#define CAFF_FRAME_ID_BYTES 1
#define CAFF_FRAME_LENGTH_BYTES 8

#define CAFF_HEADER_MAGIC_BYTES 4
#define CAFF_HEADER_HEADER_SIZE_BYTES 8
#define CAFF_HEADER_NUM_ANIM_BYTES 8

#define CAFF_CREDITS_DATE_BYTES 6
#define CAFF_CREDITS_CREATOR_LEN_BYTES 8

#define CAFF_ANIMATION_DURATION_BYTES 8

// CIFF fields sizes
#define CIFF_MAGIC_BYTES 4
#define CIFF_HEADER_SIZE_BYTES 8
#define CIFF_CONTENT_SIZE_BYTES 8
#define CIFF_WIDTH_BYTES 8
#define CIFF_HEIGHT_BYTES 8

// Ciff special characters
#define CIFF_CAPTION_END '\n'
#define CIFF_TAGS_SEPARATOR '\0'

// Buffer size definitions
#define BUF_SIZE 16
#define CREATOR_BUF_SIZE 512
#define CAPTIONS_BUFFER_SIZE 512
#define TAGS_BUFFER_SIZE 512
#define CIFF_MAX_PIXELS 2100000 // 1920x1080=2,073,600
#define CONTENT_BUFFER_SIZE CIFF_MAX_PIXELS*3

/* Status and error codes of the lexer functions */
typedef enum{
    LEXER_FRAME_OK,
    LEXER_CREATOR_BUFFER_ERROR,
    LEXER_INVALID_ID,
    LEXER_INVALID_CAFF_MAGIC,
    LEXER_INVALID_SIZES,
    LEXER_INVALID_FRAME,
    LEXER_FP_ERROR,
    LEXER_BUFFER_SIZE_ERROR,
    LEXER_EOF_REACHED,
    LEXER_CREATOR_NON_ASCII,
    LEXER_CREATOR_NAME_TOO_LONG,
    LEXER_INVALID_CIFF_MAGIC,
    LEXER_INVALID_CIFF_CONTENT_SIZE,
    LEXER_CIFF_CAPTIONS_TOO_LONG,
    LEXER_CIFF_TAGS_TOO_LONG,
    LEXER_CIFF_CAPTION_NOT_TERMINATED,
    LEXER_CIFF_INVALID_NEW_LINE,
    LEXER_CIFF_CONTENT_SIZE_ERROR
}frame_status_t;

/* Struct to hold information about the ciff frame */
typedef struct{
    long long duration; 
    long long height;
    long long width;
    unsigned char captions_buffer[CAPTIONS_BUFFER_SIZE];
    long long captions_length;
    unsigned char tags_buffer[TAGS_BUFFER_SIZE];  
    long long tags_length;
    long long number_of_tags;
    unsigned char * content_buffer_ptr; // Pointer to a buffer on the heap
    unsigned char * gif_content_buffer_ptr; // Pointer to a buffer on the heap that contains the gif color coded values of the frame
}ciff_frame_t;

/* Function: process_header
* This function extracts the contents of the header frame of the CAFF file. The function has to be called, with the appropriate state of the file pointer.
*
* Params:
* fp: pointer to an open file
* num_anims (output): the number of CIFFs in the CAFF
*
* Return value:
* frame_status_t
*/
frame_status_t process_header(file_status_t *f_stat, long long * num_anims);

/* Function: process_credits
* This function extracts the contents of the credits frame of the CAFF file. The function has to be called, with the appropriate state of the file pointer.
*
* Params:
* fp: pointer to an open file
* date (output): creation date and time (fixed 6 bytes)
* creator_buffer (output): the buffer, where the creator name will be copied to
* creator_buffer_size: the size of the creator_buffer
* creator_name_length (output): the actual length of the creator 
* 
* Return value:
* frame_status_t
*/
frame_status_t process_credits(file_status_t *f_stats, unsigned char * date, unsigned char * creator_buffer, int creator_buffer_size, long long * creator_name_length);

/* Function: process_credits
* This function extracts the contents of the credits frame of the CAFF file. The function has to be called, with the appropriate state of the file pointer.
*
* Params:
* 
* Return value:
* frame_status_t
*/
frame_status_t process_ciff_frame(file_status_t *f_stat, ciff_frame_t * ciff);

/* Function: 
 *
 *
 * 
 * 
 */
int add_alpha_to_rgb(ciff_frame_t ciff_in, long long n_pixels);


/* Function: arr_to_ll 
 * This function converts a 8 byte buffer to a long long variable. 
 */
long long arr_to_ll(const unsigned char * buffer);

#endif