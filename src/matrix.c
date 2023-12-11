#include "matrix.h"
#include "math.h"

mat4 mat4_identity() {
  mat4 identity = {
    .m = {
      { 1, 0, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 0, 1, 0 },
      { 0, 0, 0, 1 }
    }
  };

  return identity;
}

mat4 mat4_make_scale(f32 sx, f32 sy, f32 sz) {
  // sx 0  0 0
  // 0  sy 0 0
  // 0  0 sz 0
  // 0  0 0  1
  mat4 m = mat4_identity();
  m.m[0][0] = sx;
  m.m[1][1] = sy;
  m.m[2][2] = sz;

  return m;
}

mat4 mat4_make_translation(f32 tx, f32 ty, f32 tz) {
  // 1 0 0 tx
  // 0 1 0 ty
  // 0 0 1 tz
  // 0 0 0 1
  mat4 m = mat4_identity();
  m.m[0][3] = tx;
  m.m[1][3] = ty;
  m.m[2][3] = tz;

  return m;
}

mat4 mat4_make_rotation_z(f32 angle) {
  f32 c = cos(angle);
  f32 s = sin(angle);

  // c -s 0 0
  // s c  0 0
  // 0 0  1 0
  // 0 0  0 1
  mat4 m = mat4_identity();
  m.m[0][0] = c;
  m.m[0][1] = -s;
  m.m[1][0] = s;
  m.m[1][1] = c;

  return m;
}

mat4 mat4_make_rotation_x(f32 angle) {
  f32 c = cos(angle);
  f32 s = sin(angle);

  // 1 0 0  0
  // 0 c -s 0
  // 0 s c  0
  // 0 0 0  1
  mat4 m = mat4_identity();
  m.m[1][1] = c;
  m.m[1][2] = -s;
  m.m[2][1] = s;
  m.m[2][2] = c;

  return m;
}

mat4 mat4_make_rotation_y(f32 angle) {
  f32 c = cos(angle);
  f32 s = sin(angle);

  // c  0 s 0
  // 0  1 0 0
  // -s 0 c 0
  // 0  0 0 1
  mat4 m = mat4_identity();
  m.m[0][0] = c;
  m.m[0][2] = s;
  m.m[2][0] = -s;
  m.m[2][2] = c;

  return m;
}

mat4 mat4_mul_mat4(mat4 a, mat4 b) {
  mat4 result;

  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      result.m[x][y] = (a.m[x][0] * b.m[0][y]) + (a.m[x][1] * b.m[1][y]) + (a.m[x][2] * b.m[2][y]) + (a.m[x][3] * b.m[3][y]);
    }
  }

  return result;
}

mat4 mat4_make_perspective(f32 fov, f32 aspect, f32 znear, f32 zfar) {
  mat4 result = {{{ 0 }}};

  result.m[0][0] = aspect * ( 1 / tan(fov / 2));
  result.m[1][1] = 1 / tan(fov / 2);
  result.m[2][2] = zfar / (zfar - znear);
  result.m[2][3] = (-zfar * znear) / (zfar - znear);
  result.m[3][2] = 1.0;

  return result;
}

vec4 mat4_mul_vec4_project(mat4 mat_proj, vec4 v) {
  vec4 result = mat4_mul_vec4(mat_proj, v);

  if (result.w != 0.0) {
    result.x /= result.w;
    result.y /= result.w;
    result.z /= result.w;
  }

  return result;
}

mat4 mat4_look_at(vec3 eye, vec3 target, vec3 up) {
  vec3 z = vec3_normalize(vec3_sub(target, eye));
  vec3 x = vec3_normalize(vec3_cross(up, z));
  vec3 y = vec3_normalize(vec3_cross(z, x));

  mat4 view_matrix = {
    .m = {
      { x.x, x.y, x.z, -vec3_dot(x, eye) },
      { y.x, y.y, y.z, -vec3_dot(y, eye) },
      { z.x, z.y, z.z, -vec3_dot(z, eye) },
      { 0, 0, 0, 1 }
    }
  };

  return view_matrix;
}
