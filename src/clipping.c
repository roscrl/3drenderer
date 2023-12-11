#include "clipping.h"
#include "math.h"
#include "vector.h"

#define NUM_PLANES 6
plane frustum_planes[NUM_PLANES];

void init_frustrum_planes(f32 fov_x, f32 fov_y, f32 znear, f32 zfar) {
  f32 cos_half_fov_x = cos(fov_x / 2.0);
  f32 sin_half_fov_x = sin(fov_x / 2.0);

  f32 cos_half_fov_y = cos(fov_y / 2.0);
  f32 sin_half_fov_y = sin(fov_y / 2.0);

  frustum_planes[LEFT_FRUSTRUM_PLANE].point = vec3_new(0, 0, 0);
  frustum_planes[LEFT_FRUSTRUM_PLANE].normal.x = cos_half_fov_x;
  frustum_planes[LEFT_FRUSTRUM_PLANE].normal.y = 0;
  frustum_planes[LEFT_FRUSTRUM_PLANE].normal.z = sin_half_fov_x;

  frustum_planes[RIGHT_FRUSTRUM_PLANE].point = vec3_new(0, 0, 0);
  frustum_planes[RIGHT_FRUSTRUM_PLANE].normal.x = -cos_half_fov_x;
  frustum_planes[RIGHT_FRUSTRUM_PLANE].normal.y = 0;
  frustum_planes[RIGHT_FRUSTRUM_PLANE].normal.z = sin_half_fov_x;

  frustum_planes[TOP_FRUSTRUM_PLANE].point = vec3_new(0, 0, 0);
  frustum_planes[TOP_FRUSTRUM_PLANE].normal.x = 0;
  frustum_planes[TOP_FRUSTRUM_PLANE].normal.y = -cos_half_fov_y;
  frustum_planes[TOP_FRUSTRUM_PLANE].normal.z = sin_half_fov_y;

  frustum_planes[BOTTOM_FRUSTRUM_PLANE].point = vec3_new(0, 0, 0);
  frustum_planes[BOTTOM_FRUSTRUM_PLANE].normal.x = 0;
  frustum_planes[BOTTOM_FRUSTRUM_PLANE].normal.y = cos_half_fov_y;
  frustum_planes[BOTTOM_FRUSTRUM_PLANE].normal.z = sin_half_fov_y;

  frustum_planes[NEAR_FRUSTRUM_PLANE].point = vec3_new(0, 0, znear);
  frustum_planes[NEAR_FRUSTRUM_PLANE].normal.x = 0;
  frustum_planes[NEAR_FRUSTRUM_PLANE].normal.y = 0;
  frustum_planes[NEAR_FRUSTRUM_PLANE].normal.z = 1;

  frustum_planes[FAR_FRUSTRUM_PLANE].point = vec3_new(0, 0, zfar);
  frustum_planes[FAR_FRUSTRUM_PLANE].normal.x = 0;
  frustum_planes[FAR_FRUSTRUM_PLANE].normal.y = 0;
  frustum_planes[FAR_FRUSTRUM_PLANE].normal.z = -1;
}

polygon create_polygon_from_triangle(vec3 v0, vec3 v1, vec3 v2, tex2 t0, tex2 t1, tex2 t2) {
  polygon poly = {
    .vertices = { v0, v1, v2 },
    .texcoords = { t0, t1, t2 },
    .num_vertices = 3
  };

  return poly;
}

f32 f32_lerp (f32 a, f32 b, f32 t) {
  return a + (t * (b - a));
}

void clip_polygon_against_plane(polygon* polygon, int plane) {
  vec3 plane_point = frustum_planes[plane].point;
  vec3 plane_normal = frustum_planes[plane].normal;

  vec3 inside_vertices[MAX_NUM_POLY_VERTICES];
  tex2 inside_texcoords[MAX_NUM_POLY_VERTICES];
  int num_inside_vertices = 0;

  vec3* current_vertex = &polygon->vertices[0];
  tex2* current_texcoord = &polygon->texcoords[0];

  vec3* previous_vertex = &polygon->vertices[polygon->num_vertices - 1];
  tex2* previous_texcoord = &polygon->texcoords[polygon->num_vertices - 1];

  f32 current_dot = 0; 
  f32 previous_dot = vec3_dot(vec3_sub(*previous_vertex, plane_point), plane_normal);

  while (current_vertex != &polygon->vertices[polygon->num_vertices]) {
    current_dot = vec3_dot(vec3_sub(*current_vertex, plane_point), plane_normal);

    if (current_dot * previous_dot < 0) {
      f32 t = previous_dot / (previous_dot - current_dot);
      vec3 intersection_point = {
        .x = f32_lerp(previous_vertex->x, current_vertex->x, t),
        .y = f32_lerp(previous_vertex->y, current_vertex->y, t),
        .z = f32_lerp(previous_vertex->z, current_vertex->z, t),
      };

      tex2 interpolated_texcoord = {
        .u = f32_lerp(previous_texcoord->u, current_texcoord->u, t),
        .v = f32_lerp(previous_texcoord->v, current_texcoord->v, t),
      };

      inside_vertices[num_inside_vertices] = vec3_clone(&intersection_point);
      inside_texcoords[num_inside_vertices] = tex2_clone(&interpolated_texcoord);
      num_inside_vertices++;
    }

    if (current_dot > 0) {
      inside_vertices[num_inside_vertices] = vec3_clone(current_vertex);
      inside_texcoords[num_inside_vertices] = tex2_clone(current_texcoord);
      num_inside_vertices++;
    }

    previous_dot = current_dot;
    previous_vertex = current_vertex;
    previous_texcoord = current_texcoord;
    current_vertex++;
    current_texcoord++;
  }
  
  for (int i = 0; i < num_inside_vertices; i++) {
    polygon->vertices[i] = vec3_clone(&inside_vertices[i]);
    polygon->texcoords[i] = tex2_clone(&inside_texcoords[i]);
  }
  polygon->num_vertices = num_inside_vertices;
}

void clip_polygon(polygon* polygon) {
  clip_polygon_against_plane(polygon, LEFT_FRUSTRUM_PLANE);
  clip_polygon_against_plane(polygon, RIGHT_FRUSTRUM_PLANE);
  clip_polygon_against_plane(polygon, TOP_FRUSTRUM_PLANE);
  clip_polygon_against_plane(polygon, BOTTOM_FRUSTRUM_PLANE);
  clip_polygon_against_plane(polygon, NEAR_FRUSTRUM_PLANE);
  clip_polygon_against_plane(polygon, FAR_FRUSTRUM_PLANE);
};

void triangles_from_polygon(polygon* poly, triangle triangles[], int* num_triangles) {
  for (int i = 0; i < poly->num_vertices - 2; i++) {
    int index0 = 0;
    int index1 = i + 1;
    int index2 = i + 2;

    triangles[i].points[0] = vec4_from_vec3(poly->vertices[index0]);
    triangles[i].points[1] = vec4_from_vec3(poly->vertices[index1]);
    triangles[i].points[2] = vec4_from_vec3(poly->vertices[index2]);

    triangles[i].texcords[0] = poly->texcoords[index0];
    triangles[i].texcords[1] = poly->texcoords[index1];
    triangles[i].texcords[2] = poly->texcoords[index2];
  }

  *num_triangles = poly->num_vertices - 2;
}
