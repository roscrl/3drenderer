#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#include "triangle.h"
#include "types.h"

#define FPS 144
#define FRAME_TARGET_TIME (1000 / FPS)

enum cull_method {
  CULL_NONE,
  CULL_BACKSPACE
};

enum render_method {
  RENDER_WIRE,
  RENDER_WIRE_VERTEX,
  RENDER_FILL_TRIANGLE,
  RENDER_FILL_TRIANGLE_WIRE,
  RENDER_TEXTURED,
  RENDER_TEXTURED_WIRE
};

bool initalize_window();
int get_window_width();
int get_window_height();

void set_render_method(int render_method);
void set_cull_method(int cull_method);
bool is_cull_backface();

bool should_render_filled_triangles();
bool should_render_wireframe();
bool should_render_wire_vertex();
bool should_render_textured_triangles();

void draw_pixel(int x, int y, u32 color);
void draw_line(int x0, int y0, int x1, int y1, u32 color);
void draw_triangle(triangle triangle, u32 color);
void draw_rect(int x, int y, int width, int height, u32 color);
void draw_grid();

void render_color_buffer();
void clear_color_buffer(u32 color);
void clear_z_buffer();

f32 get_z_buffer_at(int x, int y);
void update_z_buffer_at(int x, int y, f32 z);

void destroy_window();

#endif
