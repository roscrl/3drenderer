#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"

typedef struct {
  vec3 direction;
} light;

void init_light_direction(vec3 direction);
vec3 get_light_direction();
u32 light_apply_intensity(u32 original_color, f32 percentage_factor);

#endif
