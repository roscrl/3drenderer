#include "light.h"

static light light_direction;

void init_light_direction(vec3 direction) {
  light_direction.direction = vec3_normalize(direction);
}

vec3 get_light_direction() {
  return light_direction.direction;
}

u32 light_apply_intensity(u32 original_color, f32 percentage_factor) {
  if      (percentage_factor < 0) percentage_factor = 0;
  else if (percentage_factor > 1) percentage_factor = 1;

  u32 a = (original_color & 0xFF000000);
  u32 r = (original_color & 0x00FF0000) * percentage_factor;
  u32 g = (original_color & 0x0000FF00) * percentage_factor;
  u32 b = (original_color & 0x000000FF) * percentage_factor;

  u32 new_color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);

  return new_color;
}
