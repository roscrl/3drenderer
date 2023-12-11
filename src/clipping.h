#ifndef CLIPPING_H
#define CLIPPING_H

#include "triangle.h"
#include "vector.h"

#define MAX_NUM_POLY_VERTICES 10
#define MAX_NUM_POLY_TRIANGLES 10

enum {
  LEFT_FRUSTRUM_PLANE,
  RIGHT_FRUSTRUM_PLANE,
  TOP_FRUSTRUM_PLANE,
  BOTTOM_FRUSTRUM_PLANE,
  NEAR_FRUSTRUM_PLANE,
  FAR_FRUSTRUM_PLANE
};

typedef struct {
  vec3 point;
  vec3 normal;
} plane;

typedef struct {
  vec3 vertices[MAX_NUM_POLY_VERTICES];
  tex2 texcoords[MAX_NUM_POLY_VERTICES];
  int num_vertices;
} polygon;

void init_frustrum_planes(f32 fov_x, f32 fov_y, f32 znear, f32 zfar);
polygon create_polygon_from_triangle(vec3 v0, vec3 v1, vec3 v2, tex2 t0, tex2 t1, tex2 t2);
void clip_polygon(polygon* polygon);
void triangles_from_polygon(polygon* poly, triangle triangles[], int* num_triangles);

#endif
