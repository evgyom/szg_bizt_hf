#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdbool.h>

// Reader function return statuses
typedef enum{
    READER_STATUS_SUCCESS,
    READER_FP_NULL,
    READER_BUFFER_NULL,
    READER_BUFFER_SIZE,
    READER_CHAR_NOT_FOUND,
    READER_EOF_REACHED
}reader_status_t;

/* Function: peek
* This function writes the next n while from the file stream to the passed buffer, and restores the filepointer to its original state.
*
* Params:
* fp: pointer to an opened file
* n: the number of characters to return
* buffer: the characters should be written here
* buffer_size: the size of the output buffer
*/
reader_status_t reader_peek(FILE * fp, int n, unsigned char * buffer, int buffer_size);

/* Function: consume
* This function writes the next n while from the file stream to the passed buffer, and the filepointer is updated accordingly.
*
* Params:
* fp: pointer to an opened file
* n: the number of characters to return
* buffer: the characters should be written here
* buffer_size: the size of the output buffer
*/
reader_status_t reader_consume(FILE * fp, int n, unsigned char * buffer, int buffer_size);

#endif