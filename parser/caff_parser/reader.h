#ifndef READER_H
#define READER_H

#include <stdio.h>

// Parser status type
typedef struct{
    FILE * fp; // Open file pointer
    unsigned long long file_size; // The length of the file in bytes
    unsigned long long loc; // Location inside the file (the number of the next byte in the file stream)
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

/* Function: reader_peek
 * This function writes the next n while from the file stream to the passed buffer, and restores the filepointer to its original state.
 *
 * Params:
 * fp: pointer to an open file
 * n: the number of characters to return
 * buffer: the characters should be written here
 * buffer_size: the size of the output buffer
 */
reader_status_t reader_peek(file_status_t *f_stat, unsigned int n, unsigned char * buffer, unsigned int buffer_size);

/* Function: reader_consume
 * This function writes the next n while from the file stream to the passed buffer, and the filepointer is updated accordingly.
 *
 * Params:
 * fp: pointer to an open file
 * n: the number of characters to return
 * buffer: the characters should be written here
 * buffer_size: the size of the output buffer
 */
reader_status_t reader_consume(file_status_t *f_stat, unsigned int n, unsigned char * buffer, unsigned int buffer_size);

/* Function: reader_until_char
 * This function reads the file and copies its content to the buffer, until it finds the specified character or reaches the capacity of the buffer.
 * 
 * Params:
 * fp: pointer to an open file
 * char_until: the character that stops the reading. this is the last character that is copied to the buffer.
 * buffer: the characters should be written here
 * buffer_size: the size of the output buffer
 * read_size: the number of characters copied to the buffer
 */
reader_status_t reader_until_char(file_status_t *f_stat, unsigned char char_until, unsigned char * buffer, unsigned int buffer_size, unsigned int * read_size);

#endif