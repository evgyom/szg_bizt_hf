#include <stdio.h>

#include "lexer.h"

int parse(char * path){
    printf("This is a demo for the caff_parser library.\n");
    FILE * fp = fopen(path, "rb");
    
    if(fp == NULL){
        printf("Cannot open file. Returning.\n");
        return 1;
    }

    // Read header
    long long num_anims;
    process_header(fp, &num_anims);    
    printf("CAFF header processed. Number of CIFFs in CAFF: %d\n", num_anims);

    // Read credits
    unsigned char date_buffer[8];
    unsigned char creator_buffer[100];
    long long creator_name_len;

    frame_status_t stat = process_credits(fp, date_buffer, creator_buffer, 100, &creator_name_len);
    if(stat != LEXER_FRAME_OK)
    printf("Process credits returned with status: %d", stat);
        return 1;

    printf("Date:\n");
    int i;
    for(i = 0; i<6; i++){
        printf("0x%x ",date_buffer[i]);
    }
    printf("\n");

    printf("Length of the creator name: %d", creator_name_len);
}
