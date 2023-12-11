#include "camera.h"
#include "matrix.h"

static camera_t camera;

void init_camera(vec3 position, vec3 direction) {
  camera.position = position;
  camera.direction = direction;
  camera.forward_velocity = vec3_new(0, 0, 0);
  camera.yaw = 0.0;
  camera.pitch = 0.0;
}

vec3 get_camera_position() {
  return camera.position;
}

vec3 get_camera_direction() {
  return camera.direction;
}

vec3 get_camera_forward_velocity() {
  return camera.forward_velocity;
}

void update_camera_position(vec3 position) {
  camera.position = position;
}

void update_camera_direction(vec3 direction) {
  camera.direction = direction;
}

void update_camera_forward_velocity(vec3 forward_velocity) {
  camera.forward_velocity = forward_velocity;
}

f32 get_camera_yaw() {
  return camera.yaw;
}

f32 get_camera_pitch() {
  return camera.pitch;
}

void rotate_camera_yaw(f32 angle) {
  camera.yaw += angle;
}

void rotate_camera_pitch(f32 angle) {
  camera.pitch += angle;
}

vec3 get_camera_lookat_target() {
  vec3 target = { 0, 0, 1 };

  mat4 camera_yaw_rotation = mat4_make_rotation_y(get_camera_yaw());
  mat4 camera_pitch_rotation = mat4_make_rotation_x(get_camera_pitch());

  mat4 camera_rotation = mat4_identity();
  camera_rotation = mat4_mul_mat4(camera_yaw_rotation, camera_rotation);
  camera_rotation = mat4_mul_mat4(camera_pitch_rotation, camera_rotation);

  vec4 camera_direction = mat4_mul_vec4(camera_rotation, vec4_from_vec3(target));
  camera.direction = vec3_from_vec4(camera_direction);

  target = vec3_add(camera.position, camera.direction);

  return target;
}
