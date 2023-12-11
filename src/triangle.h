#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "texture.h"
#include "vector.h"
#include "upng.h"

typedef struct {
  int  a, b, c;
  tex2 a_uv, b_uv, c_uv;

  f32 color;
} face;

typedef struct {
  vec4 points[3];

  tex2 texcords[3];
  f32  color;
  upng_t* texture;
} triangle;

vec3 get_triangle_normal(vec4 vertices[3]);

void draw_filled_triangle(int x0, int y0, f32 z0, f32 w0,
                          int x1, int y1, f32 z1, f32 w1,
                          int x2, int y2, f32 z2, f32 w2, u32 color);

void draw_textured_triangle(int x0, int y0, f32 z0, f32 w0, f32 u0, f32 v0, 
                            int x1, int y1, f32 z1, f32 w1, f32 u1, f32 v1, 
                            int x2, int y2, f32 z2, f32 w2, f32 u2, f32 v2, upng_t* texture);

void draw_texel(int x, int y, upng_t* texture, 
                vec4 point_a, vec4 point_b, vec4 point_c, 
                f32 u0, f32 v0, f32 u1, f32 v1, f32 u2, f32 v2);

#endif
