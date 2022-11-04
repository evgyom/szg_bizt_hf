#include "reader.h"

#define UNSIGNED_EOF 255

/* Function: peek */
reader_status_t reader_peek(FILE * fp, int n, unsigned char * buffer, int buffer_size){
    // Validate input parameters
    if(buffer == NULL)
        return READER_BUFFER_NULL;
    if(fp == NULL)
        return READER_FP_NULL;
    if(n > buffer_size)
        return READER_BUFFER_SIZE;    

    // Read values to the buffer
    int i;
    for(i=0; i<n; i++){
        unsigned char char_in = getc(fp);
        if(char_in == UNSIGNED_EOF){
            return READER_EOF_REACHED;
        }else{
            buffer[i] = char_in;
        }
    }

    // Move back the file pointer
    fseek(fp, ((int) sizeof(char)) * (-n), SEEK_CUR);

    return READER_STATUS_SUCCESS;
}

/* Function: consume */
reader_status_t reader_consume(FILE * fp, int n, unsigned char * buffer, int buffer_size){
    // Validate input parameters
    if(buffer == NULL)
        return READER_BUFFER_NULL;
    if(fp == NULL)
        return READER_FP_NULL;
    if(n > buffer_size)
        return READER_BUFFER_SIZE;      

    // Read values to the buffer
    int i;
    for(i=0; i<n; i++){
        unsigned char char_in = getc(fp);
        if(char_in == UNSIGNED_EOF){
            return READER_EOF_REACHED;
        }else{
            buffer[i] = char_in;
        }
    }

    return READER_STATUS_SUCCESS;
}