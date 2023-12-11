#ifndef TEXTURE_H
#define TEXTURE_H

#include "types.h"
#include "upng.h"

typedef struct {
  f32 u;
  f32 v;
} tex2;

tex2 tex2_clone(tex2* t);

#endif
