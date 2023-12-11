#ifndef VECTOR_H
#define VECTOR_H

#include "types.h"

typedef struct { f32 x, y;       } vec2;
typedef struct { f32 x, y, z;    } vec3;
typedef struct { f32 x, y, z, w; } vec4;

vec2 vec2_new(f32 x, f32 y);
f32  vec2_length(vec2 v);
vec2 vec2_add(vec2 a, vec2 b);
vec2 vec2_sub(vec2 a, vec2 b);
vec2 vec2_mul(vec2 v, f32 factor);
vec2 vec2_div(vec2 v, f32 factor);
f32  vec2_dot(vec2 a, vec2 b);
vec2 vec2_normalize(vec2 v);

vec3 vec3_new(f32 x, f32 y, f32 z);
vec3 vec3_rotate_x(vec3 v, f32 angle);
vec3 vec3_rotate_y(vec3 v, f32 angle);
vec3 vec3_rotate_z(vec3 v, f32 angle);
f32  vec3_length(vec3 v);
vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_mul(vec3 v, f32 factor);
vec3 vec3_div(vec3 v, f32 factor);
vec3 vec3_cross(vec3 a, vec3 b);
f32  vec3_dot(vec3 a, vec3 b);
vec3 vec3_normalize(vec3 v);
vec3 vec3_clone(vec3* v);

vec4 vec4_from_vec3(vec3 v);
vec3 vec3_from_vec4(vec4 v);
vec2 vec2_from_vec4(vec4 v);

#endif
