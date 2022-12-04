#include "reader.h"

#define UNSIGNED_EOF 255

/* Function: reader_peek */
reader_status_t reader_peek(file_status_t *f_stat, unsigned int n, unsigned char * buffer, unsigned int buffer_size){
    // Validate input parameters
    if(buffer == NULL)
        return READER_BUFFER_NULL;
    if(f_stat->fp == NULL)
        return READER_FP_NULL;
    if(n > buffer_size)
        return READER_BUFFER_SIZE;   

    unsigned long long loc_copy = f_stat->loc; 

    // Read values to the buffer
    int i;
    for(i=0; i<n; i++){
        // Check if EOF
        if(loc_copy >= f_stat->file_size)
            return READER_EOF_REACHED;
        unsigned char char_in = getc(f_stat->fp);
        //Increment the location in the file
        loc_copy++;
        buffer[i] = char_in;
    }

    // Move back the file pointer
    fseek(f_stat->fp, ((int) sizeof(char)) * (-n), SEEK_CUR);

    return READER_STATUS_SUCCESS;
}

/* Function: reader_consume */
reader_status_t reader_consume(file_status_t *f_stat, unsigned int n, unsigned char * buffer, unsigned int buffer_size){
    // Validate input parameters
    if(buffer == NULL)
        return READER_BUFFER_NULL;
    if(f_stat->fp == NULL)
        return READER_FP_NULL;
    if(n > buffer_size)
        return READER_BUFFER_SIZE;      

    // Read values to the buffer
    int i;
    for(i=0; i<n; i++){
        // Check if EOF
        if(f_stat->loc >= f_stat->file_size)
            return READER_EOF_REACHED;
        unsigned char char_in = getc(f_stat->fp);
        //Increment the location in the file
        f_stat->loc++;
        buffer[i] = char_in;
    }

    return READER_STATUS_SUCCESS;
}

/* Function: reader_until_char */
reader_status_t reader_until_char(file_status_t *f_stat, unsigned char char_until, unsigned char * buffer, unsigned int buffer_size, unsigned int * read_size){
    // Validate input parameters
    if(buffer == NULL)
        return READER_BUFFER_NULL;
    if(f_stat->fp == NULL)
        return READER_FP_NULL;

    // Clear the 
    * read_size = 0;

    // Read values to the buffer
    int i;
    for(i=0; i<buffer_size; i++){
        // Check if EOF
        if(f_stat->loc >= f_stat->file_size)
            return READER_EOF_REACHED;
        unsigned char char_in = getc(f_stat->fp);
        //Increment the location in the file
        f_stat->loc++;
        buffer[i] = char_in;
        // Increment the read buffer size
        (* read_size)++;
        // If the searched character is reached return with SUCCESS
        if(char_in == char_until)
            return READER_STATUS_SUCCESS;
    }

    // If the character is not found
    return READER_CHAR_NOT_FOUND;    
}