#ifndef READER_H
#define READER_H

#include <stdio.h>

// Parser status type
typedef struct{
    FILE * fp; // Open file pointer
    long long file_size; // The length of the file in bytes
    long long loc; // Location inside the file
}file_status_t;

// Reader function return statuses
typedef enum{
    READER_STATUS_SUCCESS,
    READER_FP_NULL,
    READER_BUFFER_NULL,
    READER_BUFFER_SIZE,
    READER_EOF_REACHED,
    READER_CHAR_NOT_FOUND
}reader_status_t;

/* Function: peek
* This function writes the next n while from the file stream to the passed buffer, and restores the filepointer to its original state.
*
* Params:
* fp: pointer to an open file
* n: the number of characters to return
* buffer: the characters should be written here
* buffer_size: the size of the output buffer
*/
reader_status_t reader_peek(file_status_t *f_stat, int n, unsigned char * buffer, int buffer_size);

/* Function: consume
* This function writes the next n while from the file stream to the passed buffer, and the filepointer is updated accordingly.
*
* Params:
* fp: pointer to an open file
* n: the number of characters to return
* buffer: the characters should be written here
* buffer_size: the size of the output buffer
*/
reader_status_t reader_consume(file_status_t *f_stat, int n, unsigned char * buffer, int buffer_size);

/*
 *
 *
 * 
 */
reader_status_t reader_until_char(file_status_t *f_stat, unsigned char char_until, unsigned char * buffer, int buffer_size, int * read_size);

#endif