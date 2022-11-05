#ifndef LEXER_H
#define LEXER_H

#include <string.h>

#include "reader.h"

#define CAFF_HEADER_ID 0x01
#define CAFF_CREDITS_ID 0x02
#define CAFF_ANIMATION_ID 0x03

#define CAFF_FRAME_ID_BYTES 1
#define CAFF_FRAME_LENGTH_BYTES 8

#define CAFF_HEADER_MAGIC_BYTES 4
#define CAFF_HEADER_HEADER_SIZE_BYTES 8
#define CAFF_HEADER_NUM_ANIM_BYTES 8

#define CAFF_CREDITS_DATE_BYTES 6
#define CAFF_CREDITS_CREATOR_LEN_BYTES 8

#define CAFF_ANIMATION_DURATION_BYTES 8

typedef enum{
    LEXER_FRAME_OK,
    LEXER_BUFFER_TOO_SMALL,
    LEXER_INVALID_ID,
    LEXER_INVALID_CAFF_MAGIC,
    LEXER_INVALID_SIZES,
    LEXER_INVALID_FRAME,
    LEXER_FP_ERROR,
    LEXER_BUFFER_SIZE_ERROR,
    LEXER_EOF_REACHED,
    LEXER_CREATOR_NON_ASCII
}frame_status_t;

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
frame_status_t process_header(FILE * fp, long long * num_anims);

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
frame_status_t process_credits(FILE * fp, unsigned char * date, unsigned char * creator_buffer, int creator_buffer_size, long long * creator_name_length);

frame_status_t process_ciff_frame(FILE * fp);

/* Function: arr_to_ll 
 *
 * 
 */
long long arr_to_ll(unsigned char * buffer);

#endif