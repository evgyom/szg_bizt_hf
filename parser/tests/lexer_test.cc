#include <gtest/gtest.h>
#include <stdio.h>

extern "C"{
  #include "../caff_parser/lexer.h"
}

// CAFFF1
#define CAFF_1_PATH "../../caff_examples/1.caff"
#define CAFF_1_HEADER_1_PATH "../../caff_examples/1_header_only_1.caff"
#define CAFF_1_HEADER_2_PATH "../../caff_examples/1_header_only_2.caff"
#define CAFF_1_HEADER_INVALID_ID_PATH "../../caff_examples/1_header_only_invalid_id.caff"
#define CAFF_1_HEADER_INVALID_MAGIC_PATH "../../caff_examples/1_header_only_invalid_magic.caff"
#define CAFF_1_HEADER_INVALID_SIZES_PATH "../../caff_examples/1_header_only_invalid_sizes.caff"
#define CAFF_1_HEADER_EOF_PATH "../../caff_examples/1_header_only_eof.caff"
#define CAFF_1_CREDITS_PATH "../../caff_examples/1_credits_only_1.caff"
#define CAFF_1_CREDITS_EOF_PATH "../../caff_examples/1_credits_only_eof.caff"
#define CAFF_1_CREDITS_INVALID_ID_PATH "../../caff_examples/1_credits_only_invalid_id.caff"
#define CAFF_1_CREDITS_NONASCII_CREATOR_PATH "../../caff_examples/1_credits_only_nonascii.caff"
#define CAFF_1_CIFF_PATH "../../caff_examples/1_ciff_only.caff"

#define CAFF_1_LENGTH 0x3D11D4
#define CAFF_1_HEADER_1_LENGTH 0x1D
#define CAFF_1_HEADER_2_LENGTH 0x1D
#define CAFF_1_HEADER_INVALID_ID_LENGTH 0x1D
#define CAFF_1_HEADER_INVALID_MAGIC_LENGTH 0x1D
#define CAFF_1_HEADER_INVALID_SIZES_LENGTH 0x1D
#define CAFF_1_HEADER_EOF_LENGTH 0x1C
#define CAFF_1_CREDITS_LENGTH 0x23
#define CAFF_1_CREDITS_EOF_LENGTH 0x22
#define CAFF_1_CREDITS_INVALID_ID_LENGTH 0x23
#define CAFF_1_CREDITS_NONASCII_CREATOR_LENGTH 0x23
#define CAFF_1_CIFF_LENGTH 0x1E88D4

// CAFF2
#define CAFF_2_PATH "../../caff_examples/2.caff"
#define CAFF_2_HEADER_PATH "../../caff_examples/2_header_only.caff"
#define CAFF_2_CREDITS_PATH "../../caff_examples/2_credits_only.caff"
#define CAFF_2_CIFF_PATH "../../caff_examples/2_ciff_only.caff"

#define CAFF_2_LENGTH 0x7A236A
#define CAFF_2_HEADER_LENGTH 0x1D
#define CAFF_2_CREDITS_LENGTH 0x25
#define CAFF_2_CIFF_LENGTH 0x1E88D4

// CAFF 3
#define CAFF_3_PATH "../../caff_examples/3.caff"
#define CAFF_3_HEADER_PATH "../../caff_examples/3_header_only.caff"
#define CAFF_3_CREDITS_PATH "../../caff_examples/3_credits_only.caff"
#define CAFF_3_CIFF_PATH "../../caff_examples/3_ciff_only.caff"

#define CAFF_3_LENGTH 0x3D11C8
#define CAFF_3_HEADER_LENGTH 0x1D
#define CAFF_3_CREDITS_LENGTH 0x17
#define CAFF_3_CIFF_LENGTH 0x1E88D4

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
  ASSERT_EQ(out, 6+256*120);
}

/* process_header_content_1
 * check if the function extracts the correct number of animations from the header
 * input file: caff_1 header only
 */ 
TEST(lexer_test, process_header_content_1){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_1_HEADER_1_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }
  file_status_t file_stat = {fp, CAFF_1_HEADER_1_LENGTH, 0};

  unsigned long long num_anims;
  frame_status_t stat;
  stat = process_header(&file_stat, &num_anims);    

  // Check if the return status is oks
  ASSERT_EQ(stat, LEXER_FRAME_OK);
  // Check if the numberof animatiosn is correctly extracted
  ASSERT_EQ(num_anims, 2);

  // Close the file
  fclose(fp);
}

/* process_header_content_2
 * check if the function extracts the correct number of animations from the header. num anims in multiple bytes.
 * input file: caff_1 header modified
 */ 
TEST(lexer_test, process_header_content_2){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_1_HEADER_2_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }
  file_status_t file_stat = {fp, CAFF_1_HEADER_2_LENGTH, 0};  

  unsigned long long num_anims;
  frame_status_t stat;
  stat = process_header(&file_stat, &num_anims);    

  // Check if the return status is oks
  ASSERT_EQ(stat, LEXER_FRAME_OK);
  // Check if the numberof animatiosn is correctly extracted
  ASSERT_EQ(num_anims, 4112);

  // Close the file
  fclose(fp);
}

/* process_header_content_3
 * check if the function extracts the correct number of animations from the header. num anims in multiple bytes.
 * input file: caff_2 header only
 */ 
TEST(lexer_test, process_header_content_3){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_2_HEADER_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }
  file_status_t file_stat = {fp, CAFF_2_HEADER_LENGTH, 0};

  unsigned long long num_anims;
  frame_status_t stat;
  stat = process_header(&file_stat, &num_anims);    

  // Check if the return status is ok
  ASSERT_EQ(stat, LEXER_FRAME_OK);
  // Check if the numberof animatiosn is correctly extracted
  ASSERT_EQ(num_anims, 4);

  // Close the file
  fclose(fp);
}

/* process_header_content_4
 * check if the function extracts the correct number of animations from the header. num anims in multiple bytes.
 * input file: caff_3
 */ 
TEST(lexer_test, process_header_content_4){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_3_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }
  file_status_t file_stat = {fp, CAFF_3_LENGTH, 0};

  unsigned long long num_anims;
  frame_status_t stat;
  stat = process_header(&file_stat, &num_anims);    

  // Check if the return status is oks
  ASSERT_EQ(stat, LEXER_FRAME_OK);
  // Check if the numberof animatiosn is correctly extracted
  ASSERT_EQ(num_anims, 2);

  // Close the file
  fclose(fp);
}

/* process_header_check_fp
 * check if the function can detect that the file pointer is null
 */ 
TEST(lexer_test, process_header_check_fp){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = NULL;
  file_status_t file_stat = {fp, CAFF_1_LENGTH, 0};

  unsigned long long num_anims;
  frame_status_t stat;
  stat = process_header(&file_stat, &num_anims);    
  
  ASSERT_EQ(stat, LEXER_FP_ERROR);
}

/* process_header_check_eof
 * check if the function can detect that the file pointer is null
 */ 
TEST(lexer_test, process_header_check_eof){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_1_HEADER_EOF_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }
  file_status_t file_stat = {fp, CAFF_1_HEADER_EOF_LENGTH, 0};

  unsigned long long num_anims;
  frame_status_t stat;
  stat = process_header(&file_stat, &num_anims);    
  
  ASSERT_EQ(stat, LEXER_EOF_REACHED);

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
  file_status_t file_stat = {fp, CAFF_1_HEADER_INVALID_ID_LENGTH, 0};

  unsigned long long num_anims;
  frame_status_t stat;
  stat = process_header(&file_stat, &num_anims);    
  
  ASSERT_EQ(stat, LEXER_INVALID_ID);

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
  file_status_t file_stat = {fp, CAFF_1_HEADER_INVALID_MAGIC_LENGTH, 0};

  unsigned long long num_anims;
  frame_status_t stat;
  stat = process_header(&file_stat, &num_anims);    
  
  ASSERT_EQ(stat, LEXER_INVALID_CAFF_MAGIC);

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
  file_status_t file_stat = {fp, CAFF_1_HEADER_INVALID_SIZES_LENGTH, 0};

  unsigned long long num_anims;
  frame_status_t stat;
  stat = process_header(&file_stat, &num_anims);    
  
  ASSERT_EQ(stat, LEXER_INVALID_SIZES);

  // Close the file
  fclose(fp);
}

/* process_credits_content_1
 * check if the function can correcly extract the date, the name of the creator and the length of the creator name
 */ 
TEST(lexer_test, process_credits_content_1){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_1_CREDITS_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }
  file_status_t file_stat = {fp, CAFF_1_CREDITS_LENGTH, 0};

  // Expected outputs
  unsigned char correct_date[CAFF_CREDITS_DATE_BYTES] = {0xe4, 0x07, 0x07, 0x02, 0x0e, 0x32};
  unsigned char correct_creator_name[12] = {'T','e','s','t',' ','C','r','e','a','t','o','r'};
  // Inputs
  unsigned char date_buffer[CAFF_CREDITS_DATE_BYTES];
  unsigned char creator_buffer[100];
  unsigned long long creator_name_len;

  frame_status_t stat = process_credits(&file_stat, date_buffer, creator_buffer, 100, &creator_name_len);
  
  //Check if the return value is correct
  ASSERT_EQ(stat, LEXER_FRAME_OK);

  // Check if the date is correct
  int i;
  for(i = 0; i<CAFF_CREDITS_DATE_BYTES; i++){
    ASSERT_EQ(date_buffer[i], correct_date[i]);
  }

  //Check if the length of the creator name is correct
  ASSERT_EQ(creator_name_len,12);

  //Check if the creator name is correct
  for(i = 0; i<12; i++){
    ASSERT_EQ(creator_buffer[i], correct_creator_name[i]);
  }

  // Close the file
  fclose(fp);
}

/* process_credits_content_2
 * check if the function can correcly extract the date, the name of the creator and the length of the creator name
 */ 
TEST(lexer_test, process_credits_content_2){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_2_CREDITS_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }
  file_status_t file_stat = {fp, CAFF_2_CREDITS_LENGTH, 0};

  // Expected outputs
  unsigned char correct_date[CAFF_CREDITS_DATE_BYTES] = {0xe4, 0x07, 0x07, 0x02, 0x0e, 0x32};
  unsigned char correct_creator_name[14] = {'S','e','c','o','n','d','_','C','r','e','a','t','o','r'};
  
  // Inputs
  unsigned char date_buffer[CAFF_CREDITS_DATE_BYTES];
  unsigned char creator_buffer[100];
  unsigned long long creator_name_len;

  frame_status_t stat = process_credits(&file_stat, date_buffer, creator_buffer, 100, &creator_name_len);

  //Check if the return value is correct
  ASSERT_EQ(stat, LEXER_FRAME_OK);
  
  // Check if the date is correct
  int i;
  for(i = 0; i<CAFF_CREDITS_DATE_BYTES; i++){
    ASSERT_EQ(date_buffer[i], correct_date[i]);
  }

  //Check if the length of the creator name is correct
  ASSERT_EQ(creator_name_len,14);

  //Check if the creator name is correct
  for(i = 0; i<12; i++){
    ASSERT_EQ(creator_buffer[i], correct_creator_name[i]);
  }

  // Close the file
  fclose(fp);
}

/* process_credits_content_3
 * check if the function can correcly extract the date, the name of the creator and the length of the creator name
 * -- empty creator name
 */ 
TEST(lexer_test, process_credits_content_3){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_3_CREDITS_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }
  file_status_t file_stat = {fp, CAFF_3_CREDITS_LENGTH, 0};

  // Expected outputs
  unsigned char correct_date[CAFF_CREDITS_DATE_BYTES] = {0xe4, 0x07, 0x07, 0x02, 0x0e, 0x32};
  unsigned char correct_creator_name[2]; // Not really needed

  // Inputs
  unsigned char date_buffer[CAFF_CREDITS_DATE_BYTES];
  unsigned char creator_buffer[100];
  unsigned long long creator_name_len;

  frame_status_t stat = process_credits(&file_stat, date_buffer, creator_buffer, 100, &creator_name_len);
  
  //Check if the return value is correct
  ASSERT_EQ(stat, LEXER_FRAME_OK);
  
  // Check if the date is correct
  int i;
  for(i = 0; i<CAFF_CREDITS_DATE_BYTES; i++){
    ASSERT_EQ(date_buffer[i], correct_date[i]);
  }

  //Check if the length of the creator name is correct
  ASSERT_EQ(creator_name_len,0);

  //Check if the creator name is correct
  for(i = 0; i<0; i++){
    ASSERT_EQ(creator_buffer[i], correct_creator_name[i]);
  } 

  // Close the file
  fclose(fp);
}

/* process_credits_check_fp
 * check if the function can detect that the file pointer is null
 */ 
TEST(lexer_test, process_credits_check_fp){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = NULL;
  file_status_t file_stat = {fp, CAFF_1_LENGTH, 0};

  // Inputs
  unsigned char date_buffer[CAFF_CREDITS_DATE_BYTES];
  unsigned char creator_buffer[100];
  unsigned long long creator_name_len;

  frame_status_t stat = process_credits(&file_stat, date_buffer, creator_buffer, 100, &creator_name_len);   
  
  ASSERT_EQ(stat, LEXER_FP_ERROR);
}

/* process_credits_check_eof
 * check if the function can returns with the correct error code when the the end of file is reached while processing a caff
 */ 
TEST(lexer_test, process_credits_check_eof){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_1_CREDITS_EOF_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }
  file_status_t file_stat = {fp, CAFF_1_CREDITS_EOF_LENGTH, 0};

  // Inputs
  unsigned char date_buffer[CAFF_CREDITS_DATE_BYTES];
  unsigned char creator_buffer[100];
  unsigned long long creator_name_len;

  frame_status_t stat = process_credits(&file_stat, date_buffer, creator_buffer, 100, &creator_name_len);
  
  //Check if the return value is correct
  ASSERT_EQ(stat, LEXER_EOF_REACHED);
  
  // Close the file
  fclose(fp);
}

/* process_credits_invalid_id
 * check if the function can identify an incorrect frame id
 */ 
TEST(lexer_test, process_credits_invalid_id){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_1_CREDITS_INVALID_ID_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }
  file_status_t file_stat = {fp, CAFF_1_CREDITS_INVALID_ID_LENGTH, 0};

  // Inputs
  unsigned char date_buffer[CAFF_CREDITS_DATE_BYTES];
  unsigned char creator_buffer[100];
  unsigned long long creator_name_len;

  frame_status_t stat = process_credits(&file_stat, date_buffer, creator_buffer, 100, &creator_name_len);
  
  //Check if the return value is correct
  ASSERT_EQ(stat, LEXER_INVALID_ID);
  
  // Close the file
  fclose(fp);
}

/* process_credits_non_ascii_creator
 * check if the function can identify that a creator name is not all ascii characters
 */ 
TEST(lexer_test, process_credits_non_ascii_creator){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_1_CREDITS_NONASCII_CREATOR_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }
  file_status_t file_stat = {fp, CAFF_1_CREDITS_NONASCII_CREATOR_LENGTH, 0};

  // Inputs
  unsigned char date_buffer[CAFF_CREDITS_DATE_BYTES];
  unsigned char creator_buffer[100];
  unsigned long long creator_name_len;

  frame_status_t stat = process_credits(&file_stat, date_buffer, creator_buffer, 100, &creator_name_len);

  printf("Creator:\n");
  int i;
  for(i =0; i<creator_name_len; i++){
    printf("%d ",creator_buffer[i]);
  }
  printf("\n");
  
  //Check if the return value is correct
  ASSERT_EQ(stat, LEXER_CREATOR_NON_ASCII);
  
  // Close the file
  fclose(fp);
}

/* process_credits_creator_buffer_size
 * check if the function returns with the correct error code if the buffer for the creator name is smaller than the name of the creator
 */ 
TEST(lexer_test, process_credits_creator_buffer_size){

  // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_1_CREDITS_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }
  file_status_t file_stat = {fp, CAFF_1_CREDITS_LENGTH, 0};

  // Inputs
  unsigned char date_buffer[CAFF_CREDITS_DATE_BYTES];
  unsigned char creator_buffer[8];
  unsigned long long creator_name_len;

  frame_status_t stat = process_credits(&file_stat, date_buffer, creator_buffer, 8, &creator_name_len);
  
  //Check if the return value is correct
  ASSERT_EQ(stat, LEXER_CREATOR_BUFFER_ERROR);
  
  // Close the file
  fclose(fp);
}

/* process_ciff_content_ok
 * this test verifies if the retrieved content is correct. does not check the ciff content
 */
TEST(lexer_test, process_ciff_content_ok){
  
   // Assume that the file can be opened - otherwise check the logs
  FILE * fp = fopen(CAFF_1_CIFF_PATH, "rb");
  if(fp == NULL){
    printf("File could not be opened. \n");
  }else{
    printf("File opened successfully.\n");
  }
  file_status_t file_stat = {fp, CAFF_1_CIFF_LENGTH, 0};

  // Inputs
  ciff_frame_t my_ciff;
  my_ciff.content_buffer_ptr = (unsigned char *) malloc(CONTENT_BUFFER_SIZE * sizeof(char));
  frame_status_t stat = process_ciff_frame(&file_stat, &my_ciff);

  ASSERT_EQ(stat, LEXER_FRAME_OK);
  
  // Close the file
  fclose(fp);

  //free heap
  free(my_ciff.content_buffer_ptr);
}