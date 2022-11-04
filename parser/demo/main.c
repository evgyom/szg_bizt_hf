#include <stdio.h>

#include "lexer.h"

int main(){
    printf("This is a demo for the caff_parser library.\n");
    FILE * fp = fopen("caff_examples/1.caff", "rb");
    
    if(fp == NULL){
        printf("Cannot open file. Returning.\n");
        return 1;
    }

    // Read header
    long long num_anims = 20;
    process_header(fp, &num_anims);    
    printf("CAFF header processed. Number of CIFFs in CAFF: %d\n", num_anims);

    // Read credits
    char date_buffer [9];
    char creator_buffer [100];
    long long creator_name_len;
    
    if(FRAME_OK == process_credits(fp, date_buffer, creator_buffer, 100, &creator_name_len)){
        date_buffer[8] = '\0';
        printf("Date: %s", date_buffer);

        printf("Creator:\n");
        int i;
        for(i = 0; i<creator_name_len; i++){
            printf("%d", creator_buffer[i]);
        }
        printf("\n");
    }
}
