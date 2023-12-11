#include "swap.h"
#include "types.h"

void swap_int(int* a, int* b) {
  int tmp_a = *a;

  *a = *b;
  *b = tmp_a;
}

void swap_f32(f32* a, f32* b) {
  f32 tmp_a = *a;

  *a = *b;
  *b = tmp_a;
}
