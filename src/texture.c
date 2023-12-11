#include <stdio.h>
#include <stdint.h>
#include "texture.h"
#include "types.h"
#include "upng.h"


tex2 tex2_clone(tex2* t) {
  tex2 result = {
    .u = t->u,
    .v = t->v,
  };

  return result;
}
