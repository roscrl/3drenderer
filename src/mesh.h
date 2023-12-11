#ifndef MESH_H
#define MESH_H

#include "triangle.h"
#include "vector.h"
#include "upng.h"

typedef struct {
  vec3 *vertices;   // dynamic array of vertices
  face *faces;      // dynamic array of faces

  upng_t* texture;  // png texture pointer
                  
  vec3 rotation;    // rotation    with x, y, z
  vec3 scale;       // scale       with x, y, z
  vec3 translation; // translation with x, y, z
} mesh;

void load_mesh(char* filename, char* png_filename, vec3 scale, vec3 translation, vec3 rotation);
void load_mesh_obj_data(mesh* mesh, char *filename);
void load_mesh_png_data(mesh* mesh, char* png_filename);

int get_num_meshes();
mesh* get_mesh(int index);

void free_meshes();

#endif
