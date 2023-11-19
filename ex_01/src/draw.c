#define _POSIX_C_SOURCE 199309L
#include "draw.h"
#include "stdio.h"

// If you want you can use the provided struct and static function

typedef struct COLOR_s {
    char r; // red channel
    char g; // green channel
    char b; // blue channel
} COLOR;

static COLOR color_converter(double value) {
    COLOR c;
    c.r = 0;
    c.g = 0;
    c.b = 0;

    // TODO color mapping
    if (value<=0.25){
        c.r = 0;
        c.g = (255*value)/0.25;
        c.b = 255;
    } else if(value<=0.5){
        c.r = 0;
        c.g = 255;
        c.b = (255*(0.5-value))/0.25;
    } else if(value<=0.75){
        c.r = (255*(value-0.5))/0.25;
        c.g = 255;
        c.b = 0;
    } else{
        c.r = 255;
        c.g = (255*(1-value))/0.25;
        c.b = 0;
    }
    return c;
}

void draw_grid(double* grid, uint32_t x, uint32_t y, const char* filepath) {
    // TODO mandatory
    // Open or create file
    FILE *file= fopen(filepath,"w");
    if(file==NULL){
        printf("Error opening file");
        return;
    }
    // Write header with meta information
    fprintf(file, "P3\n");
    fprintf(file, "%d %d\n", x, y);
    fprintf(file, "255\n");
    // Write RGB data
    COLOR grid_pix_col;
    for(uint32_t i=0;i<y;i++){
        for(uint32_t j=0;j<x;j++){
            grid_pix_col = color_converter(grid[i*x+j]);
            fprintf(file, "%d %d %d ", (unsigned char)grid_pix_col.r, (unsigned char)grid_pix_col.g, (unsigned char)grid_pix_col.b);
        }
        fprintf(file, "\n");
    }
    // Close file
    fclose(file);
}
