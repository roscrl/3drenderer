#include "vector.h"

#include <math.h>
#include "matrix.h"

vec2 vec2_new(f32 x, f32 y) {
  vec2 v = {
    .x = x,
    .y = y,
  };

  return v;
}

vec3 vec3_rotate_x(vec3 v, f32 angle) {
  vec3 rotated_vector = {
    .x = v.x, .y = v.y * cos(angle) - v.z * sin(angle), 
    .z = v.y * sin(angle) + v.z * cos(angle)
  };

  return rotated_vector;
}

vec3 vec3_rotate_y(vec3 v, f32 angle) {
  vec3 rotated_vector = {
    .x = v.x * cos(angle) - v.z * sin(angle), 
    .y = v.y, .z = v.x * sin(angle) + v.z * cos(angle)
  };

  return rotated_vector;
}

vec3 vec3_rotate_z(vec3 v, f32 angle) {
  vec3 rotated_vector = {
    .x = v.x * cos(angle) - v.y * sin(angle), 
    .y = v.x * sin(angle) + v.y * cos(angle), .z = v.z
  };

  return rotated_vector;
}

f32 vec2_length(vec2 v) {
  f32 x_squared = v.x * v.x;
  f32 y_squared = v.y * v.y;

  return sqrt(x_squared + y_squared);
}

vec2 vec2_add(vec2 a, vec2 b) {
  vec2 result = {
      .x = a.x + b.x,
      .y = a.y + b.y,
  };

  return result;
}

vec2 vec2_sub(vec2 a, vec2 b) {
  vec2 result = {
      .x = a.x - b.x,
      .y = a.y - b.y,
  };

  return result;
}

vec2 vec2_mul(vec2 v, f32 factor) {
  vec2 result = {
      .x = v.x * factor,
      .y = v.y * factor,
  };

  return result;
}

vec2 vec2_div(vec2 v, f32 factor) {
  vec2 result = {
      .x = v.x / factor,
      .y = v.y / factor,
  };

  return result;
}

f32 vec2_dot(vec2 a, vec2 b) { 
  return (a.x * b.x) + (a.y * b.y); 
}

vec2 vec2_normalize(vec2 v) {
  f32 x_squared = v.x * v.x;
  f32 y_squared = v.y * v.y;

  f32 length = sqrt(x_squared + y_squared);

  vec2 result = {
      .x = v.x / length,
      .y = v.y / length,
  };

  return result;
}

f32 vec3_length(vec3 v) {
  f32 x_squared = v.x * v.x;
  f32 y_squared = v.y * v.y;
  f32 z_squared = v.z * v.z;

  return sqrt(x_squared + y_squared + z_squared);
}

vec3 vec3_new(f32 x, f32 y, f32 z) {
  vec3 v = {
    .x = x,
    .y = y,
    .z = z,
  };

  return v;
}

vec3 vec3_add(vec3 a, vec3 b) {
  vec3 result = {
      .x = a.x + b.x,
      .y = a.y + b.y,
      .z = a.z + b.z,
  };

  return result;
}

vec3 vec3_sub(vec3 a, vec3 b) {
  vec3 result = {
      .x = a.x - b.x,
      .y = a.y - b.y,
      .z = a.z - b.z,
  };

  return result;
}

vec3 vec3_mul(vec3 v, f32 factor) {
  vec3 result = {
      .x = v.x * factor,
      .y = v.y * factor,
      .z = v.z * factor,
  };

  return result;
}

vec3 vec3_div(vec3 v, f32 factor) {
  vec3 result = {
      .x = v.x / factor,
      .y = v.y / factor,
      .z = v.z / factor,
  };

  return result;
}

vec3 vec3_cross(vec3 a, vec3 b) {
  vec3 result = {
      .x = (a.y * b.z) - (a.z * b.y),
      .y = (a.z * b.x) - (a.x * b.z),
      .z = (a.x * b.y) - (a.y * b.x),
  };

  return result;
}

f32 vec3_dot(vec3 a, vec3 b) { 
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); 
}

vec3 vec3_normalize(vec3 v) {
  f32 x_squared = v.x * v.x;
  f32 y_squared = v.y * v.y;
  f32 z_squared = v.z * v.z;

  f32 length = sqrt(x_squared + y_squared + z_squared);

  vec3 result = {
      .x = v.x / length,
      .y = v.y / length,
      .z = v.z / length,
  };

  return result;
}

vec3 vec3_clone(vec3* v) {
  vec3 result = {
    .x = v->x,
    .y = v->y,
    .z = v->z,
  };

  return result;
}

vec4 mat4_mul_vec4(mat4 m, vec4 v) {
  vec4 result = {
    .x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w,
    .y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w,
    .z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w,
    .w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w,
  };

  return result;
}

vec4 vec4_from_vec3(vec3 v) {
  vec4 result = {
    .x = v.x,
    .y = v.y,
    .z = v.z,
    .w = 1.0,
  };

  return result;
}

vec3 vec3_from_vec4(vec4 v) {
  vec3 result = {
    .x = v.x,
    .y = v.y,
    .z = v.z,
  };

  return result;
}

vec2 vec2_from_vec4(vec4 v) {
  vec2 result = {
    .x = v.x,
    .y = v.y,
  };

  return result;
}
