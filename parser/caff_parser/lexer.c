#include "lexer.h"

/* Private functions */
frame_status_t process_ciff(file_status_t *f_stat, ciff_frame_t * ciff);

frame_status_t process_header(file_status_t *f_stat, long long  * num_anims){
    unsigned char buffer[BUF_SIZE];
    reader_status_t stat;

    // Verify frame ID
    stat = reader_consume(&f_stat, CAFF_FRAME_ID_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    if(buffer[0] != CAFF_HEADER_ID)
        return LEXER_INVALID_ID;

    // Get the length of the header
    long long size_of_frame;
    stat = reader_consume(&f_stat, CAFF_FRAME_LENGTH_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    size_of_frame = arr_to_ll(buffer);

    // Load the CAFF Magic part of sthe frame
    stat = reader_consume(&f_stat, CAFF_HEADER_MAGIC_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    // If the next characters are not 'CAFF' return an INVALID_FRAME error
    if(strcmp(buffer, "CAFF") != 0)
        return LEXER_INVALID_CAFF_MAGIC;

    // Load the header_size part of the frame
    stat = reader_consume(&f_stat, CAFF_HEADER_HEADER_SIZE_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }

    long long size_of_header;
    size_of_header = arr_to_ll(buffer);
    printf("Size of header: %d\n", size_of_header);
    // If the frame size and the header size don't match, the frame is invalid
    if(size_of_header != size_of_frame)
        return LEXER_INVALID_SIZES;
    
    if(size_of_header != 20)
        return LEXER_INVALID_SIZES;

    // Load the num_anim part of the frame
    stat = reader_consume(&f_stat, CAFF_HEADER_NUM_ANIM_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    * num_anims = arr_to_ll(buffer);

    return LEXER_FRAME_OK;
}

frame_status_t process_credits(file_status_t *f_stat, unsigned char * date, unsigned char * creator_buffer, int creator_buffer_size, long long * creator_name_length){
    unsigned char buffer[BUF_SIZE];
    unsigned char creator_buffer_local[CREATOR_BUF_SIZE];
    reader_status_t stat;

    // Verify frame ID
    stat = reader_consume(&f_stat, CAFF_FRAME_ID_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    if(buffer[0] != CAFF_CREDITS_ID)
        return LEXER_INVALID_ID;

    // Get the length of the credits frame
    long long size_of_frame;
    stat = reader_consume(f_stat, CAFF_FRAME_LENGTH_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    size_of_frame = arr_to_ll(buffer);

    // Get the date of creation
    stat = reader_consume(&f_stat, CAFF_CREDITS_DATE_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    memcpy(date, buffer, CAFF_CREDITS_DATE_BYTES);

    // Load the length of the creator section
    stat = reader_consume(&f_stat, CAFF_CREDITS_CREATOR_LEN_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    * creator_name_length = arr_to_ll(buffer);
    if(* creator_name_length > creator_buffer_size)
        return LEXER_CREATOR_BUFFER_ERROR;
    if(* creator_name_length >= size_of_frame)
        return LEXER_INVALID_SIZES;

    // Load the creator name
    stat = reader_consume(&f_stat, * creator_name_length, creator_buffer_local, CREATOR_BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    // Validate if the creator name contains only ascii characters
    int i;
    for(i = 0; i<(*creator_name_length); i++){
        if((creator_buffer_local[i] == 0) || (creator_buffer_local[i] > 127))
            return LEXER_CREATOR_NON_ASCII;
    }
    // Copy the creator name to the output buffer
    memcpy(creator_buffer, creator_buffer_local, * creator_name_length);
    
    return LEXER_FRAME_OK;
}

frame_status_t process_ciff_frame(file_status_t *f_stat, ciff_frame_t * ciff){
    unsigned char buffer[BUF_SIZE];
    reader_status_t stat;

    // Verify frame ID
    stat = reader_consume(&f_stat, CAFF_FRAME_ID_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    if(buffer[0] != CAFF_CREDITS_ID)
        return LEXER_INVALID_ID;

    // Get the length of the frame containg the ciff
    stat = reader_consume(&f_stat, CAFF_FRAME_LENGTH_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    long long size_of_frame = arr_to_ll(buffer);

    // Get the duration of the ciff animation
    stat = reader_consume(&f_stat, CAFF_ANIMATION_DURATION_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    long long duration_of_ciff = arr_to_ll(buffer);

    ciff_frame_t ciff_in;
    frame_status_t ciff_stat = process_ciff(&f_stat, &ciff_in);

}

frame_status_t process_ciff(file_status_t *f_stat, ciff_frame_t * ciff){
    unsigned char buffer[BUF_SIZE];
    reader_status_t stat;

    // Read the CIFF magic field
    stat = reader_consume(&f_stat, CIFF_MAGIC_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    // If the next characters are not 'CAFF' return an INVALID_FRAME error
    if(strcmp(buffer, "CIFF") != 0)
        return LEXER_INVALID_CIFF_MAGIC;

    // Read the header_size field
    stat = reader_consume(&f_stat, CIFF_HEADER_SIZE_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    long long ciff_header_size = arr_to_ll(buffer);

    // Read the content size
    stat = reader_consume(&f_stat, CIFF_CONTENT_SIZE_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    long long ciff_content_size = arr_to_ll(buffer);

    // Read the width
    stat = reader_consume(&f_stat, CIFF_WIDTH_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    long long ciff_width = arr_to_ll(buffer);

    // Read the height
    stat = reader_consume(&f_stat, CIFF_HEIGHT_BYTES, buffer, BUF_SIZE);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }
    long long ciff_height = arr_to_ll(buffer);

    if(ciff_content_size != ciff_height * ciff_width * 3)
        return LEXER_INVALID_CIFF_CONTENT_SIZE;
    
    // Write the height and width of the ciff to the output frame
    ciff->height = ciff_height;
    ciff->width = ciff_width;

    // Read the caption
    unsigned char caption_buffer[CAPTIONS_BUFFER_SIZE]; 
    int caption_length;
    stat = reader_until_char(&f_stat, CIFF_CAPTION_END, caption_buffer, CAPTIONS_BUFFER_SIZE, & caption_length);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
        if(stat == READER_CHAR_NOT_FOUND)
            return LEXER_CIFF_CAPTION_NOT_TERMINATED;
    }

    // Copy the caption to the ciff struct
    memcpy(ciff->captions_buffer, caption_buffer, caption_length);

    // Read the tags
    unsigned char tags_buffer[CAPTIONS_BUFFER_SIZE]; 
    int tags_length = ciff_header_size - (CIFF_MAGIC_BYTES + CIFF_HEADER_SIZE_BYTES + CIFF_CONTENT_SIZE_BYTES + CIFF_WIDTH_BYTES + CIFF_HEIGHT_BYTES + caption_length);
    stat = reader_consume(&f_stat, TAGS_BUFFER_SIZE, tags_buffer, tags_length);
    if(stat != READER_STATUS_SUCCESS){
        if(stat == READER_FP_NULL)
            return LEXER_FP_ERROR;
        if(stat == READER_BUFFER_SIZE)
            return LEXER_BUFFER_SIZE_ERROR;
        if(stat == READER_EOF_REACHED)
            return LEXER_EOF_REACHED;
    }

    int tag_count = 0;
    int i;
    for(i = 0; i<tags_length; i++){
        if(tags_buffer == '\0')
            tag_count ++;
        else if(tags_buffer[i] == '\n')
            return LEXER_CIFF_INVALID_NEW_LINE;
    }

    //Copy the tags info
    memcpy(ciff->tags_buffer, tags_buffer, tags_length);
    ciff->number_of_tags = tag_count;
    ciff->tags_length = tags_length;

    return LEXER_FRAME_OK;
}

long long arr_to_ll(unsigned char * buffer){
    long long retval = 0;

    // Enforce little-endiannes
    int i;
    for(i = 0; i<8; i++){
        retval += buffer[i] * (1 << (8*i));
    }

    return retval;    
}