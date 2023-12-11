#include <SDL2/SDL.h>
#include "matrix.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "array.h"
#include "display.h"
#include "mesh.h"
#include "triangle.h"
#include "vector.h"
#include "light.h"
#include "camera.h"
#include "clipping.h"

#define MAX_TRIANGLES_TO_DRAW 10000
triangle triangles_to_render[MAX_TRIANGLES_TO_DRAW];
int num_triangles_to_render = 0;

mat4 proj_matrix;

bool is_running = false;
int previous_frame_start_time = 0;
f32 delta_time = 0;

void setup() {
  set_render_method(RENDER_WIRE);
  set_cull_method(CULL_BACKSPACE);

  init_light_direction(vec3_new(0, 0, 1));

  f32 aspect_y = (f32) get_window_height() / (f32) get_window_width();
  f32 aspect_x = (f32) get_window_width() / (f32) get_window_height();
  f32 fov_y = M_PI / 3.0;
  f32 fov_x = atan(tan(fov_y / 2.0) * aspect_x) * 2.0;
  f32 znear = 0.1;
  f32 zfar = 100.0;
  proj_matrix = mat4_make_perspective(fov_y, aspect_y, znear, zfar);

  init_frustrum_planes(fov_x, fov_y, znear, zfar);

  load_mesh("./assets/f22.obj", "./assets/f22.png", vec3_new(1, 1, 1), vec3_new(-9, 0, 10), vec3_new(0, 0, 0));
  load_mesh("./assets/cube.obj", "./assets/cube.png", vec3_new(1, 1, 1), vec3_new(-12, 3.2, 10), vec3_new(0, 0, 0));
  load_mesh("./assets/efa.obj", "./assets/efa.png", vec3_new(1, 1, 1), vec3_new(-15, 0, 10), vec3_new(0, 0, 0));
  load_mesh("./assets/drone.obj", "./assets/drone.png", vec3_new(1, 1, 1), vec3_new(-12, -3.2, 10), vec3_new(0, 0, 0));
}

void process_input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        is_running = false;

        break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_c) set_cull_method(CULL_BACKSPACE);               
        if (event.key.keysym.sym == SDLK_x) set_cull_method(CULL_NONE);                    

        if (event.key.keysym.sym == SDLK_1) set_render_method(RENDER_WIRE_VERTEX);         
        if (event.key.keysym.sym == SDLK_2) set_render_method(RENDER_WIRE);                
        if (event.key.keysym.sym == SDLK_3) set_render_method(RENDER_FILL_TRIANGLE);       
        if (event.key.keysym.sym == SDLK_4) set_render_method(RENDER_FILL_TRIANGLE_WIRE);  
        if (event.key.keysym.sym == SDLK_5) set_render_method(RENDER_TEXTURED);            
        if (event.key.keysym.sym == SDLK_6) set_render_method(RENDER_TEXTURED_WIRE);       

        if (event.key.keysym.sym == SDLK_w) { 
          update_camera_forward_velocity(vec3_mul(get_camera_direction(), 5.0 * delta_time));
          update_camera_position(vec3_add(get_camera_position(), get_camera_forward_velocity()));

        }

        if (event.key.keysym.sym == SDLK_s) {
          update_camera_forward_velocity(vec3_mul(get_camera_direction(), 5.0 * delta_time));
          update_camera_position(vec3_sub(get_camera_position(), get_camera_forward_velocity()));
          
        }

        if (event.key.keysym.sym == SDLK_UP)   rotate_camera_pitch(3.0 * delta_time);  
        if (event.key.keysym.sym == SDLK_DOWN) rotate_camera_pitch(-3.0 * delta_time); 

        if (event.key.keysym.sym == SDLK_a)    rotate_camera_yaw(-0.5 * delta_time);   
        if (event.key.keysym.sym == SDLK_d)    rotate_camera_yaw(0.5 * delta_time);    

        if (event.key.keysym.sym == SDLK_ESCAPE) is_running = false;                   
    }
  }
}

void swap_triangle(triangle* a, triangle* b) {
  triangle tmp_a = *a;

  *a = *b;
  *b = tmp_a;
}

void process_graphics_pipeline_stages(mesh* mesh) {
  vec3 target = get_camera_lookat_target();
  vec3 up_direction = { 0, 1, 0 };
  mat4 view_matrix = mat4_look_at(get_camera_position(), target, up_direction);

  mat4 scale_matrix = mat4_make_scale(mesh->scale.x, mesh->scale.y, mesh->scale.z);
  mat4 rotation_matrix_x = mat4_make_rotation_x(mesh->rotation.x);
  mat4 rotation_matrix_y = mat4_make_rotation_y(mesh->rotation.y);
  mat4 rotation_matrix_z = mat4_make_rotation_z(mesh->rotation.z);
  mat4 translation_matrix = mat4_make_translation(mesh->translation.x, mesh->translation.y, mesh->translation.z);

  int num_mesh_faces = array_length(mesh->faces);
  for (int i = 0; i < num_mesh_faces; i++) {
    face mesh_face = mesh->faces[i];

    vec3 face_vertices[3];
    face_vertices[0] = mesh->vertices[mesh_face.a];
    face_vertices[1] = mesh->vertices[mesh_face.b];
    face_vertices[2] = mesh->vertices[mesh_face.c];

    vec4 transformed_verticies[3];

    for (int j = 0; j < 3; j++) {
      vec4 transformed_vertex = vec4_from_vec3(face_vertices[j]);

      mat4 world_matrix = mat4_identity();
      world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
      world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
      world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
      world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
      world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

      transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

      transformed_vertex = mat4_mul_vec4(view_matrix, transformed_vertex);

      transformed_verticies[j] = transformed_vertex;
    }

    vec3 face_normal = get_triangle_normal(transformed_verticies);

    if (is_cull_backface()) {
      vec3 camera_ray = vec3_sub(vec3_new(0, 0, 0), vec3_from_vec4(transformed_verticies[0]));

      f32 dot_normal_camera = vec3_dot(face_normal, camera_ray);

      if (dot_normal_camera < 0) continue;  // ignore triangles looking away
    }

    polygon poly = create_polygon_from_triangle(
      vec3_from_vec4(transformed_verticies[0]), 
      vec3_from_vec4(transformed_verticies[1]), 
      vec3_from_vec4(transformed_verticies[2]),
      mesh_face.a_uv, 
      mesh_face.b_uv, 
      mesh_face.c_uv
    );

    clip_polygon(&poly);

    triangle triangles_after_clipping[MAX_NUM_POLY_TRIANGLES];
    int num_triangles_after_clipping = 0;

    triangles_from_polygon(&poly, triangles_after_clipping, &num_triangles_after_clipping);

    for(int t = 0; t < num_triangles_after_clipping; t++) {
      triangle triangle_after_clipping = triangles_after_clipping[t];

      vec4 projected_points[3];

      for (int j = 0; j < 3; j++) {
        projected_points[j] = mat4_mul_vec4_project(proj_matrix,  triangle_after_clipping.points[j]);

        // scale
        projected_points[j].x *= get_window_width() / 2.0;
        projected_points[j].y *= get_window_height() / 2.0;

        // invert y for flipped screen y coord
        projected_points[j].y *= -1;

        // translate
        projected_points[j].x += get_window_width() / 2.0;
        projected_points[j].y += get_window_height() / 2.0;
      }

      f32 light_intesity_factor = -vec3_dot(face_normal, get_light_direction());
      u32 triangle_color = light_apply_intensity(mesh_face.color, light_intesity_factor);

      triangle triangle_to_render = {
        .points = {
          {projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w},
          {projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w},
          {projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w},
        },
        .color = triangle_color,
        .texcords = {
          { triangle_after_clipping.texcords[0].u, triangle_after_clipping.texcords[0].v },
          { triangle_after_clipping.texcords[1].u, triangle_after_clipping.texcords[1].v },
          { triangle_after_clipping.texcords[2].u, triangle_after_clipping.texcords[2].v },
        },
        .texture = mesh->texture,
      };

      if (num_triangles_to_render < MAX_TRIANGLES_TO_DRAW) {
        triangles_to_render[num_triangles_to_render] = triangle_to_render;
        num_triangles_to_render++;
      }
    }
  }
}

void update() {
  {
    u64 previous_frame_end_time = SDL_GetTicks64();
    u64 previous_frame_duration = previous_frame_end_time - previous_frame_start_time;
    if (previous_frame_duration < FRAME_TARGET_TIME) SDL_Delay(FRAME_TARGET_TIME - previous_frame_duration);

    delta_time = (SDL_GetTicks64() - previous_frame_start_time) / 1000.0;
    previous_frame_start_time = SDL_GetTicks64();
  }

  num_triangles_to_render = 0;

  for(int mesh_index = 0; mesh_index < get_num_meshes(); mesh_index++) {
    mesh* mesh = get_mesh(mesh_index);

    // per frame - change rotation / animation
    mesh->rotation.x += 0.5 * delta_time;
    mesh->rotation.y += 0.5 * delta_time;
    mesh->rotation.z += 0.5 * delta_time;

    /* mesh.scale.x += 0.005 * delta_time; */
    /* mesh.scale.y += 0.005 * delta_time; */
    /* mesh.scale.z += 0.005 * delta_time; */

    mesh->translation.x += 1 * delta_time;
    /* mesh.translation.z = 5.0; */

    /* camera.position.x += 0.0 * delta_time; */
    /* camera.position.y += 0.0 * delta_time; */

    process_graphics_pipeline_stages(mesh);
  }
}

void render() {
  clear_color_buffer(0xFFFFFFFF);
  clear_z_buffer();

  draw_grid();

  for (int i = 0; i < num_triangles_to_render; i++) {
    triangle triangle = triangles_to_render[i];
  
    if (should_render_wireframe()) {
      draw_rect(triangle.points[0].x, triangle.points[0].y, 5, 5, 0xFF0000FF);
      draw_rect(triangle.points[1].x, triangle.points[1].y, 5, 5, 0xFF0000FF);
      draw_rect(triangle.points[2].x, triangle.points[2].y, 5, 5, 0xFF0000FF);
    }

    if (should_render_textured_triangles()) {
      draw_textured_triangle(
        triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texcords[0].u, triangle.texcords[0].v,
        triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texcords[1].u, triangle.texcords[1].v,
        triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texcords[2].u, triangle.texcords[2].v,
        triangle.texture
      );
    }

    if (should_render_filled_triangles()) {
      draw_filled_triangle(
        triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w,
        triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w,
        triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w,
        triangle.color
      );
    }

    if (should_render_wire_vertex()) {
      draw_triangle(triangle, 0xFF0000FF);
    }
  }

  render_color_buffer();
}

void free_resources() {
  free_meshes();
  destroy_window();
}

int main(void) {
  is_running = initalize_window();

  setup();

  while (is_running) {
    process_input();
    update();
    render();
  }

  free_resources();

  return 0;
}
