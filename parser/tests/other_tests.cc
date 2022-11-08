#include <gtest/gtest.h>
#include <stdio.h>

extern "C"{
  #include "../caff_parser/color_map.h"
}

/* color_similiarity
 * this test verifies if the retrieved content is correct. does not check the ciff content
 */
TEST(other_tests, color_similiarity)
{
  color_t color_1 = {255,127,191};
  color_t color_2 = {32,82,200};
  float dist =  get_color_similiarity(color_1, color_2);

  ASSERT_EQ(dist, 15550.5);
}
