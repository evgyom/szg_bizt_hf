#include <stdio.h>

#include "lexer.h"

int parse(char * path){
    printf("This is a demo for the caff_parser library.\n");
    FILE * fp = fopen(path, "rb");
    
    if(fp == NULL){
        printf("Cannot open file. Returning.\n");
        return 1;
    }

    file_status_t file_stat;
    file_stat.fp = fp;

    // Frame status
    frame_status_t stat;

    // Read header
    long long num_anims;
    stat = process_header(&file_stat, &num_anims);    
    printf("Process header returned with status: %d\n", stat);
    printf("CAFF header processed. Number of CIFFs in CAFF: %d\n", num_anims);

    // Read credits
    unsigned char date_buffer[8];
    unsigned char creator_buffer[100];
    long long creator_name_len;

    stat = process_credits(&file_stat, date_buffer, creator_buffer, 100, &creator_name_len);
    printf("Process credits returned with status: %d\n", stat);

    printf("Date:\n");
    int i;
    for(i = 0; i<6; i++){
        printf("0x%x ",date_buffer[i]);
    }
    printf("\n");

    printf("Length of the creator name: %d", creator_name_len);
}
