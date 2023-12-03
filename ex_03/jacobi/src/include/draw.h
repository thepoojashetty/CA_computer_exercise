#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>

/* Draws a grid of dimension x and y with double values between [0.0 ; 1.0] as a .ppm file.
 * The PPM file standard is described at https://en.wikipedia.org/wiki/Netpbm#File_formats
 */
void draw_grid(double* grid, uint32_t x, uint32_t y, const char* filepath);

#endif
