#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

typedef struct {
  vec3 position;
  vec3 direction;
  vec3 forward_velocity;
  f32 yaw;
  f32 pitch;
} camera_t;

void init_camera(vec3 position, vec3 direction);

vec3 get_camera_position();
vec3 get_camera_direction();
vec3 get_camera_forward_velocity();

void update_camera_position(vec3 position);
void update_camera_direction(vec3 direction);
void update_camera_forward_velocity(vec3 forward_velocity);

f32 get_camera_yaw();
f32 get_camera_pitch();

void rotate_camera_yaw(f32 angle);
void rotate_camera_pitch(f32 angle);

vec3 get_camera_lookat_target();

#endif
