#ifndef MATRIX_H
#define MATRIX_H

#include "types.h"
#include "vector.h"

typedef struct { 
  f32 m[4][4];
} mat4;

mat4 mat4_identity();
mat4 mat4_make_scale(f32 sx, f32 sy, f32 sz);
vec4 mat4_mul_vec4(mat4 m, vec4 v);
mat4 mat4_mul_mat4(mat4 a, mat4 b);
mat4 mat4_make_translation(f32 tx, f32 ty, f32 tz);
mat4 mat4_make_perspective(f32 fov, f32 aspect, f32 znear, f32 zfar);
vec4 mat4_mul_vec4_project(mat4 mat_proj, vec4 v);
mat4 mat4_make_rotation_x(f32 angle);
mat4 mat4_make_rotation_y(f32 angle);
mat4 mat4_make_rotation_z(f32 angle);
mat4 mat4_look_at(vec3 eye, vec3 target, vec3 up);

#endif
