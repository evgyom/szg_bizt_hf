#include <stdio.h>

#include "lexer.h"
#include <string.h>

int main(){
    FILE * fp = fopen("caff_examples/1.caff", "rb");

    int64_t num_anims;
    get_header(fp, &num_anims);    

    printf("Number of CIFFs in CAFF: %d\n", num_anims.long_long);
}