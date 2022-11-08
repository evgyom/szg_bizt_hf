#ifndef COLOR_MAP_H
#define COLOR_MAP_H

#define COLOR_MAP_SIZE 256

typedef struct{
    unsigned char r;
    unsigned char g;
    unsigned char b;
}color_t;

extern unsigned char color_map_256[COLOR_MAP_SIZE*3];

int get_most_similar_color(color_t color, unsigned char * color_map, int color_map_length);

float get_color_similiarity(color_t color_1, color_t color_2);

color_t arr_to_color(unsigned char * arr);

#endif