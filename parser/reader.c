#include "reader.h"

/* Function: peek */
reader_status_t reader_peek(FILE * fp, int n, char * buffer, int buffer_size){

    // Validate input parameters
    if(buffer == NULL)
        return READER_FP_NULL;
    if(fp == NULL)
        return READER_FP_NULL;
    if(n > buffer_size)
        return READER_BUFFER_SIZE;    

    // Read values to the buffer
    int i;
    for(i=0; i<n; i++){
        char char_in = getc(fp);
        if(char_in == EOF){
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
reader_status_t reader_consume(FILE * fp, int n, char * buffer, int buffer_size){
    // Validate input parameters
    if(buffer == NULL)
        return READER_FP_NULL;
    if(fp == NULL)
        return READER_FP_NULL;
    if(n > buffer_size)
        return READER_BUFFER_SIZE;    

    // Read values to the buffer
    int i;
    for(i=0; i<n; i++){
        char char_in = getc(fp);
        if(char_in == EOF){
            return READER_EOF_REACHED;
        }else{
            buffer[i] = char_in;
        }
    }

    return READER_STATUS_SUCCESS;
}

/* Function: reader_search_character */
reader_status_t reader_search_character(FILE * fp, char * buffer, int buffer_size, int * length_of_return_buffer, char char_to_find, int search_max){
     // Validate input parameters
    if(buffer == NULL)
        return READER_FP_NULL;
    if(fp == NULL)
        return READER_FP_NULL;
    if(search_max > buffer_size)
        return READER_BUFFER_SIZE;

    // Read values to the buffer until the given a character is reached
    int i;
    for(i = 0; i<search_max; i++){
        char char_in = getc(fp);
        if(char_in == EOF){
            return READER_EOF_REACHED;
        }else{
            buffer[i] = char_in;
            if(char_in == char_to_find)
                return READER_STATUS_SUCCESS;
        }
    }

    // Indicate the buffer size
    * length_of_return_buffer = i+1;

    return READER_CHAR_NOT_FOUND;
}

/* Function: is_eof */
bool is_eof(FILE * fp){
    if(fgetc(fp) == EOF){
        return true;
    }else{
        fseek(fp, ((int) sizeof(char)) * (-1), SEEK_CUR);
        return false;
    }
}