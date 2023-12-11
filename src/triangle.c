#include "triangle.h"
#include "display.h"
#include "swap.h"

vec3 get_triangle_normal(vec4 verticies[3]) {
  vec3 vector_a = vec3_from_vec4(verticies[0]);
  vec3 vector_b = vec3_from_vec4(verticies[1]);
  vec3 vector_c = vec3_from_vec4(verticies[2]);

  vec3 vector_ab = vec3_sub(vector_b, vector_a);
  vector_ab = vec3_normalize(vector_ab);

  vec3 vector_ac = vec3_sub(vector_c, vector_a);
  vector_ac = vec3_normalize(vector_ac);

  vec3 normal = vec3_cross(vector_ab, vector_ac);
  normal = vec3_normalize(normal);  // normalise the face normal vector

  return normal;
}


void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, u32 color) {
  f32 inverse_slope_1 = (f32) (x1 - x0) / (y1 - y0);
  f32 inverse_slope_2 = (f32) (x2 - x0) / (y2 - y0);

  f32 x_start = x0;
  f32 x_end   = x0;

  for(int y = y0; y <= y2; y++) {
    draw_line(x_start, y, x_end, y, color);

    x_start += inverse_slope_1;
    x_end   += inverse_slope_2;
  }
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, u32 color) {
  f32 inverse_slope_1 = (f32) (x2 - x0) / (y2 - y0);
  f32 inverse_slope_2 = (f32) (x2 - x1) / (y2 - y1);

  f32 x_start = x2;
  f32 x_end   = x2;

  for(int y = y2; y >= y0; y--) {
    draw_line(x_start, y, x_end, y, color);

    x_start -= inverse_slope_1;
    x_end   -= inverse_slope_2;
  }
}

vec3 barycentric(vec2 a, vec2 b, vec2 c, vec2 p) {
  vec2 ac = vec2_sub(c, a);
  vec2 ab = vec2_sub(b, a);
  vec2 pc = vec2_sub(c, p);
  vec2 pb = vec2_sub(b, p);
  vec2 ap = vec2_sub(p, a);

  f32 area_parallelogram_abc = (ac.x * ab.y - ac.y * ab.x);

  f32 alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;
  f32 beta  = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;
  f32 gamma = 1.0f - alpha - beta;

  vec3 weights = { alpha, beta, gamma };

  return weights;
}

void draw_triangle_pixel(int x, int y, u32 color, 
                         vec4 point_a, vec4 point_b, vec4 point_c) {
  vec2 p = { x, y };
  vec2 a = vec2_from_vec4(point_a);
  vec2 b = vec2_from_vec4(point_b);
  vec2 c = vec2_from_vec4(point_c);

  vec3 weights = barycentric(a, b, c, p);

  f32 alpha = weights.x;
  f32 beta  = weights.y;
  f32 gamma = weights.z;

  f32 interpolated_reciprocal_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta + (1 / point_c.w) * gamma;

  interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

  if (interpolated_reciprocal_w < get_z_buffer_at(x,y)) {
    draw_pixel(x, y, color);
    update_z_buffer_at(x, y, interpolated_reciprocal_w);
  }
}

void draw_filled_triangle(int x0, int y0, f32 z0, f32 w0,
                          int x1, int y1, f32 z1, f32 w1,
                          int x2, int y2, f32 z2, f32 w2, u32 color) {
  if (y0 > y1) {
    swap_int(&y0, &y1);
    swap_int(&x0, &x1);
    swap_f32(&z0, &z1);
    swap_f32(&w0, &w1);
  }

  if (y1 > y2) {
    swap_int(&y1, &y2);
    swap_int(&x1, &x2);
    swap_f32(&z1, &z2);
    swap_f32(&w1, &w2);
  }

  if (y0 > y1) {
    swap_int(&y0, &y1);
    swap_int(&x0, &x1);
    swap_f32(&z0, &z1);
    swap_f32(&w0, &w1);
  }

  vec4 point_a = { x0, y0, z0, w0 };
  vec4 point_b = { x1, y1, z1, w1 };
  vec4 point_c = { x2, y2, z2, w2 };

  f32 inverse_slope_1 = 0;
  f32 inverse_slope_2 = 0;

  if (y1 - y0 != 0) inverse_slope_1 = (f32) (x1 - x0) / abs(y1 - y0);
  if (y2 - y0 != 0) inverse_slope_2 = (f32) (x2 - x0) / abs(y2 - y0);

  if (y1 - y0 != 0) {
    for (int y = y0; y <= y1; y++) {
      int x_start = x1 + (y - y1) * inverse_slope_1;
      int x_end   = x0 + (y - y0) * inverse_slope_2;

      if (x_start > x_end) swap_int(&x_start, &x_end);

      for (int x = x_start; x <= x_end; x++) {
        draw_triangle_pixel(x, y, color, point_a, point_b, point_c);
      }
    }
  }

  inverse_slope_1 = 0;
  inverse_slope_2 = 0;

  if (y2 - y1 != 0) inverse_slope_1 = (f32) (x2 - x1) / abs(y2 - y1);
  if (y2 - y0 != 0) inverse_slope_2 = (f32) (x2 - x0) / abs(y2 - y0);

  if (y2 - y1 != 0) {
    for (int y = y1; y <= y2; y++) {
      int x_start = x1 + (y - y1) * inverse_slope_1;
      int x_end   = x0 + (y - y0) * inverse_slope_2;

      if (x_start > x_end) swap_int(&x_start, &x_end);

      for (int x = x_start; x <= x_end; x++) {
        draw_triangle_pixel(x, y, color, point_a, point_b, point_c);
      }
    }
  }
}

void draw_texel(int x, int y, upng_t* texture, 
                vec4 point_a, vec4 point_b, vec4 point_c, 
                f32 u0, f32 v0, f32 u1, f32 v1, f32 u2, f32 v2) {
  vec2 p = { x, y };
  vec2 a = vec2_from_vec4(point_a);
  vec2 b = vec2_from_vec4(point_b);
  vec2 c = vec2_from_vec4(point_c);

  vec3 weights = barycentric(a, b, c, p);

  f32 alpha = weights.x;
  f32 beta  = weights.y;
  f32 gamma = weights.z;

  f32 interpolated_u = (u0 / point_a.w) * alpha + (u1 / point_b.w) * beta + (u2 / point_c.w) * gamma;
  f32 interpolated_v = (v0 / point_a.w) * alpha + (v1 / point_b.w) * beta + (v2 / point_c.w) * gamma;

  f32 interpolated_reciprocal_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta + (1 / point_c.w) * gamma;

  interpolated_u /= interpolated_reciprocal_w;
  interpolated_v /= interpolated_reciprocal_w;

  int texture_width = upng_get_width(texture);
  int texture_height = upng_get_height(texture);

  int texture_x = abs((int) (interpolated_u * texture_width)) % texture_width;
  int texture_y = abs((int) (interpolated_v * texture_height)) % texture_height;
  
  interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;
  
  if (interpolated_reciprocal_w < get_z_buffer_at(x, y)) {
    u32* texture_buffer = (u32*) upng_get_buffer(texture);

    draw_pixel(x, y, texture_buffer[texture_y * texture_width + texture_x]);
    
    update_z_buffer_at(x, y, interpolated_reciprocal_w);
  }
}

void draw_textured_triangle(int x0, int y0, f32 z0, f32 w0, f32 u0, f32 v0,
                            int x1, int y1, f32 z1, f32 w1, f32 u1, f32 v1, 
                            int x2, int y2, f32 z2, f32 w2, f32 u2, f32 v2, upng_t* texture) {
  if (y0 > y1) {
    swap_int(&y0, &y1);
    swap_int(&x0, &x1);

    swap_f32(&z0, &z1);
    swap_f32(&w0, &w1);

    swap_f32(&u0, &u1);
    swap_f32(&v0, &v1);
  }

  if (y1 > y2) {
    swap_int(&y1, &y2);
    swap_int(&x1, &x2);

    swap_f32(&z1, &z2);
    swap_f32(&w1, &w2);

    swap_f32(&u1, &u2);
    swap_f32(&v1, &v2);
  }

  if (y0 > y1) {
    swap_int(&y0, &y1);
    swap_int(&x0, &x1);

    swap_f32(&z0, &z1);
    swap_f32(&w0, &w1);

    swap_f32(&u0, &u1);
    swap_f32(&v0, &v1);
  }

  v0 = 1.0 - v0;
  v1 = 1.0 - v1;
  v2 = 1.0 - v2;

  vec4 point_a = { x0, y0, z0, w0 };
  vec4 point_b = { x1, y1, z1, w1 };
  vec4 point_c = { x2, y2, z2, w2 };

  f32 inverse_slope_1 = 0;
  f32 inverse_slope_2 = 0;

  if (y1 - y0 != 0) inverse_slope_1 = (f32) (x1 - x0) / abs(y1 - y0);
  if (y2 - y0 != 0) inverse_slope_2 = (f32) (x2 - x0) / abs(y2 - y0);

  if (y1 - y0 != 0) {
    for (int y = y0; y <= y1; y++) {
      int x_start = x1 + (y - y1) * inverse_slope_1;
      int x_end   = x0 + (y - y0) * inverse_slope_2;

      if (x_start > x_end) swap_int(&x_start, &x_end);

      for (int x = x_start; x <= x_end; x++) {
        draw_texel(x, y, texture, point_a, point_b, point_c, u0, v0, u1, v1, u2, v2);
      }

    }
  }

  inverse_slope_1 = 0;
  inverse_slope_2 = 0;

  if (y2 - y1 != 0) inverse_slope_1 = (f32) (x2 - x1) / abs(y2 - y1);
  if (y2 - y0 != 0) inverse_slope_2 = (f32) (x2 - x0) / abs(y2 - y0);

  if (y2 - y1 != 0) {
    for (int y = y1; y <= y2; y++) {
      int x_start = x1 + (y - y1) * inverse_slope_1;
      int x_end   = x0 + (y - y0) * inverse_slope_2;

      if (x_start > x_end) swap_int(&x_start, &x_end);

      for (int x = x_start; x <= x_end; x++) {
        draw_texel(x, y, texture, point_a, point_b, point_c, u0, v0, u1, v1, u2, v2);
      }
    }
  }
}

