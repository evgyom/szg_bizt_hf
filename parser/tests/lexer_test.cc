#include <gtest/gtest.h>
#include <stdio.h>

extern "C"{
  #include "../caff_parser/lexer.h"
}

#define CAFF_1_PATH "../../caff_examples/1.caff"
#define CAFF_1_HEADER_1_PATH "../../caff_examples/1_header_only_1.caff"
#define CAFF_1_HEADER_2_PATH "../../caff_examples/1_header_only_2.caff"
#define CAFF_1_HEADER_INVALID_ID_PATH "../../caff_examples/1_header_only_invalid_id.caff"
#define CAFF_1_HEADER_INVALID_MAGIC_PATH "../../caff_examples/1_header_only_invalid_magic.caff"
#define CAFF_1_HEADER_INVALID_SIZES_PATH "../../caff_examples/1_header_only_invalid_sizes.caff"

#define CAFF_1_CREDITS_PATH "../../caff_examples/1_credits_only.caff"
#define CAFF_1_CIFF_PATH "../../caff_examples/1_ciff_only.caff"

/* arr_to_ll_test_1
 * little endian array. only one byte.
 */
TEST(lexer_test, arr_to_ll_test_1){
  unsigned char array[8] = {6, 0, 0, 0, 0, 0, 0, 0};

  long long out = arr_to_ll(array);

  ASSERT_EQ(out, 6);
}

/* arr_to_ll_test_2
 * little endian array. two bytes.
 */
TEST(lexer_test, arr_to_ll_test_2){
  unsigned char array[8] = {6, 120, 0, 0, 0, 0, 0, 0};

  long long out = arr_to_ll(array);
  printf("out: %ul", out);

  ASSERT_EQ(out, 6+256*120);
}

/* process_header_content_1
 * check if the function extracts the correct number of animations from the header
 */ 
TEST(lexer_test, process_header_content_1){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_1_HEADER_1_PATH, "rb");

  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }

  long long num_anims;
  frame_status_t stat;
  stat = process_header(fp, &num_anims);    

  // Check if the return status is oks
  ASSERT_EQ(stat, FRAME_OK);
  // Check if the numberof animatiosn is correctly extracted
  ASSERT_EQ(num_anims, 2);

  // Close the file
  fclose(fp);
}

/* process_header_content_2
 * check if the function extracts the correct number of animations from the header. num anims in multiple bytes.
 */ 
TEST(lexer_test, process_header_content_2){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_1_HEADER_2_PATH, "rb");

  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }

  long long num_anims;
  frame_status_t stat;
  stat = process_header(fp, &num_anims);    

  // Check if the return status is oks
  ASSERT_EQ(stat, FRAME_OK);
  // Check if the numberof animatiosn is correctly extracted
  ASSERT_EQ(num_anims, 4112);

  // Close the file
  fclose(fp);
}

/* process_header_check_id
 * check if the function can filter out incorrect frame id for the header
 */
TEST(lexer_test, process_header_check_id){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_1_HEADER_INVALID_ID_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }

  long long num_anims;
  frame_status_t stat;
  stat = process_header(fp, &num_anims);    
  
  ASSERT_EQ(stat, INVALID_ID);

  // Close the file
  fclose(fp);
}

/* process_header_check_magic
 * check if the function can identify caff magic in the header (CAFF)
 */
TEST(lexer_test, process_header_check_magic){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_1_HEADER_INVALID_MAGIC_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }

  long long num_anims;
  frame_status_t stat;
  stat = process_header(fp, &num_anims);    
  
  ASSERT_EQ(stat, INVALID_CAFF_MAGIC);

  // Close the file
  fclose(fp);
}

/* process_header_check_sizes
 * check if the function can filter out inconsistency in the two sizes
 */ 
TEST(lexer_test, process_header_check_sizes){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_1_HEADER_INVALID_SIZES_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }

  long long num_anims;
  frame_status_t stat;
  stat = process_header(fp, &num_anims);    
  
  ASSERT_EQ(stat, INVALID_FRAME_SIZES);

  // Close the file
  fclose(fp);
}