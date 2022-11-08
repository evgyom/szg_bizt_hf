#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "gifenc.h"
#include "color_map.h"

#define GIF_PALETTE_DEPTH 8
#define GIF_NO_TRANSPARENCY -1
#define GIF_INFINITE_LOOP 0

int parse(char * ciff_path, long long file_size, char * gif_path){

    // Open CAFF
    FILE * fp = fopen(ciff_path, "rb");
    if(fp == NULL){
        printf("Cannot open file. Returning.\n");
        return 1;
    }
    file_status_t file_stat = {fp, file_size, 0};

    // Read header
    long long num_anims;
    frame_status_t stat = process_header(&file_stat, &num_anims);    
    printf("process_header status: %d\n", stat);
    if(stat != LEXER_FRAME_OK)
        return 1;
    printf("Number of CIFFs in CAFF: %d\n", num_anims);

    // Read credits
    unsigned char date_buffer[8];
    unsigned char creator_buffer[100];
    long long creator_name_len;

    stat = process_credits(&file_stat, date_buffer, creator_buffer, 100, &creator_name_len);
    if(stat != LEXER_FRAME_OK)
        return 1;
    printf("process_credits status: %d\nDate: ", stat);
    int i;
    for(i = 0; i<6; i++){
        printf("0x%x ",date_buffer[i]);
    }

    printf("\nCreator: ");
    for(i = 0; i<creator_name_len; i++){
        printf("%c",creator_buffer[i]);
    }

    // Read the first ciff
    ciff_frame_t my_ciff;
    my_ciff.content_buffer_ptr = malloc(CONTENT_BUFFER_SIZE * sizeof(char));
    my_ciff.gif_content_buffer_ptr = malloc(GIF_CONTENT_BUFFER_SIZE * sizeof(char));

    stat = process_ciff_frame(&file_stat, &my_ciff);
    printf("\n0 -- process ciff status: %d\n", stat);
    if(stat != LEXER_FRAME_OK){
        return 1;
    }
    printf("CIFF size: %d x %d\n", my_ciff.width, my_ciff.height);

    //Create empty GIF
    ge_GIF *gif = ge_new_gif(gif_path, my_ciff.width, my_ciff.height, color_map_256, GIF_PALETTE_DEPTH, GIF_NO_TRANSPARENCY, GIF_INFINITE_LOOP);
    long long j;
    for(j = 0; j<(my_ciff.width * my_ciff.height); j++){
        color_t pixel = arr_to_color(&(my_ciff.content_buffer_ptr[3*j]));
        gif->frame[j] = get_most_similar_color(pixel, color_map_256, COLOR_MAP_SIZE);
    }
    ge_add_frame(gif, my_ciff.duration/10);

    // Read the remaining CIFFs
    for(i = 1; i<num_anims; i++){
        stat = process_ciff_frame(&file_stat, &my_ciff);
        printf("%d -- process_ciff status status: %d\n", (i+1), stat);
        if(stat != LEXER_FRAME_OK){
            return 1;
        }
        printf("CIFF size: %d x %d\n", my_ciff.width, my_ciff.height);
        
        for(j = 0; j<(my_ciff.width * my_ciff.height); j++){
            color_t pixel = arr_to_color(&(my_ciff.content_buffer_ptr[3*j]));
            gif->frame[j] = get_most_similar_color(pixel, color_map_256, COLOR_MAP_SIZE);
        }
        ge_add_frame(gif, my_ciff.duration/10);
    } 

    // close the GIF
    ge_close_gif(gif);

    free(my_ciff.content_buffer_ptr);
    free(my_ciff.gif_content_buffer_ptr);
}
