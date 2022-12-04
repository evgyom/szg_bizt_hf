#include <gtest/gtest.h>
#include <stdio.h>
#include "../caff_parser/parser.h"

/* Checks if some of the returned data is correct from CAFF1
 */
TEST(parser_test, test_content_1){
    
    struct_out_t struct_out;
    char input_path[] = "../../caff_examples/1.caff";
    char out_path[] = "test.gif"; 
    int file_size = 4002260;

    int stat = parse(input_path, file_size, out_path, &struct_out);

    // Check the return status
    ASSERT_EQ(stat, PARSER_STATUS_OK);

    // Check the number of frames
    ASSERT_EQ(struct_out.n_frames, 2);

    // Check the creator name length
    ASSERT_EQ(struct_out.creator_name_length, 12);

    // Check the caption length
    ASSERT_EQ(struct_out.captions_length, 17);

    // Check the tags length
    ASSERT_EQ(struct_out.tags_length, 26);

    // Check the frame size
    ASSERT_EQ(struct_out.frame_height, 667);
    ASSERT_EQ(struct_out.frame_width, 1000);
}


/* Checks if the returned data is correct from CAFF2
 */
TEST(parser_test, test_content_2){
    struct_out_t struct_out;
    char input_path[] = "../../caff_examples/2.caff";
    char out_path[] = "test.gif"; 
    int file_size = 8004458;

    int stat = parse(input_path, file_size, out_path, &struct_out);

    // Check the return status
    ASSERT_EQ(stat, PARSER_STATUS_OK);

    // Check the number of frames
    ASSERT_EQ(struct_out.n_frames, 4);

    // Check the creator name length
    ASSERT_EQ(struct_out.creator_name_length, 14);

    // Check the caption length
    ASSERT_EQ(struct_out.captions_length, 17);

    // Check the tags length
    ASSERT_EQ(struct_out.tags_length, 26);

    // Check the frame size
    ASSERT_EQ(struct_out.frame_height, 667);
    ASSERT_EQ(struct_out.frame_width, 1000); 
}

/* Checks if the returned data is correct from CAFF3
 */
TEST(parser_test, test_content_3){
    struct_out_t struct_out;
    char input_path[] = "../../caff_examples/3.caff";
    char out_path[] = "test.gif"; 
    unsigned int file_size = 4002248;

    int stat = parse(input_path, file_size, out_path, &struct_out);

    // Check the return status
    ASSERT_EQ(stat, PARSER_STATUS_OK);

    // Check the number of frames
    ASSERT_EQ(struct_out.n_frames, 2);

    // Check the creator name length
    ASSERT_EQ(struct_out.creator_name_length, 0);

    // Check the caption length
    ASSERT_EQ(struct_out.captions_length, 17);

    // Check the tags length
    ASSERT_EQ(struct_out.tags_length, 26);

    // Check the frame size
    ASSERT_EQ(struct_out.frame_height, 667);
    ASSERT_EQ(struct_out.frame_width, 1000); 

}

/* Checks if the function can detect that not the whole file is processed
 */
TEST(parser_test, less_frames_specified){
    struct_out_t struct_out;
    char input_path[] = "../../caff_examples/1_less_frames_specified.caff";
    char out_path[] = "test.gif"; 
    unsigned int file_size = 4002260;

    int stat = parse(input_path, file_size, out_path, &struct_out);
    ASSERT_EQ(stat, CAFF_UNDEFINED_SECTION);
}

/* Checks if the function can detect that the caff contains less frames
 */
TEST(parser_test,more_frames_specified){
    struct_out_t struct_out;
    char input_path[] = "../../caff_examples/1_more_frames_specified.caff";
    char out_path[] = "test.gif"; 
    unsigned int file_size = 4002260;

    int stat = parse(input_path, file_size, out_path, &struct_out);
    ASSERT_NE(stat, PARSER_STATUS_OK);
}

/* 
 */
TEST(parser_test,invalid_creator_len){
    struct_out_t struct_out;
    char input_path[] = "../../caff_examples/1_invalid_creator_len.caff";
    char out_path[] = "test.gif"; 
    unsigned int file_size = 4002260;

    int stat = parse(input_path, file_size, out_path, &struct_out);
    ASSERT_NE(stat, PARSER_STATUS_OK);
}

/* 
 */
TEST(parser_test,invalid_caff_header_size){
    struct_out_t struct_out;
    char input_path[] = "../../caff_examples/1_invalid_caff_header_size.caff";
    char out_path[] = "test.gif"; 
    unsigned int file_size = 4002260;

    int stat = parse(input_path, file_size, out_path, &struct_out);
    ASSERT_NE(stat, PARSER_STATUS_OK);
}

/* Checks if the function can detect that the caff contains less frames
 */
TEST(parser_test,invalid_ciff_header_size){
    struct_out_t struct_out;
    char input_path[] = "../../caff_examples/1_invalid_ciff_header_size.caff";
    char out_path[] = "test.gif"; 
    unsigned int file_size = 4002260;

    int stat = parse(input_path, file_size, out_path, &struct_out);
    ASSERT_NE(stat, PARSER_STATUS_OK);
}

/* 
 */
TEST(parser_test,invalid_frame_sizes){
    struct_out_t struct_out;
    char input_path[] = "../../caff_examples/1_invalid_frame_sizes.caff";
    char out_path[] = "test.gif"; 
    unsigned int file_size = 4002260;

    int stat = parse(input_path, file_size, out_path, &struct_out);
    ASSERT_NE(stat, PARSER_STATUS_OK);
}

/* 
 */
TEST(parser_test,invalid_new_line_in_tags){
    struct_out_t struct_out;
    char input_path[] = "../../caff_examples/1_invalid_new_line_in_tags.caff";
    char out_path[] = "test.gif"; 
    unsigned int file_size = 4002260;

    int stat = parse(input_path, file_size, out_path, &struct_out);
    ASSERT_NE(stat, PARSER_STATUS_OK);
}