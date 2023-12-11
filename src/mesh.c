#include "mesh.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "triangle.h"
#include "vector.h"

#define MAX_VERTICES 10
static mesh meshes[MAX_VERTICES];
static int num_meshes = 0;

void load_mesh(char* obj_filename, char* png_filename, vec3 scale, vec3 translation, vec3 rotation) {
  load_mesh_obj_data(&meshes[num_meshes], obj_filename);
  load_mesh_png_data(&meshes[num_meshes], png_filename);

  meshes[num_meshes].scale = scale;
  meshes[num_meshes].translation = translation;
  meshes[num_meshes].rotation = rotation;

  num_meshes++;
}

void load_mesh_obj_data(mesh* mesh, char *obj_filename) {
  char *text;
  {
    FILE *textfile = fopen(obj_filename, "r");

    fseek(textfile, 0L, SEEK_END);
    long numbytes = ftell(textfile);
    fseek(textfile, 0L, SEEK_SET);

    text = (char *)malloc(numbytes + 1);

    fread(text, sizeof(char), numbytes, textfile);
    text[numbytes] = '\0';
    fclose(textfile);
  }

  char *line = text;
  char *nextLine;

  char *v_prefix = "v ";
  char *f_prefix = "f ";

  tex2* texcoords = NULL;

  while ((nextLine = strchr(line, '\n')) != NULL) {
    *nextLine = '\0';

    if (strncmp(line, v_prefix, strlen(v_prefix)) == 0) {
      vec3 vertex;

      sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);

      array_push(mesh->vertices, vertex);
    } else if (strncmp(line, f_prefix, strlen(f_prefix)) == 0) {
      int vertex_indices[3];
      int texture_indices[3];
      int normal_indices[3];

      sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
             &vertex_indices[0], &texture_indices[0], &normal_indices[0], 
             &vertex_indices[1], &texture_indices[1], &normal_indices[1], 
             &vertex_indices[2], &texture_indices[2], &normal_indices[2]);

      face face = {
          .a     = vertex_indices[0] - 1,
          .b     = vertex_indices[1] - 1,
          .c     = vertex_indices[2] - 1,
          .a_uv  = texcoords[texture_indices[0] - 1],
          .b_uv  = texcoords[texture_indices[1] - 1],
          .c_uv  = texcoords[texture_indices[2] - 1],
          .color = 0xFFFFFFFF
      };

      array_push(mesh->faces, face);
    } else if (strncmp(line, "vt ", 3) == 0) {
      tex2 texcoord;

      sscanf(line, "vt %f %f", &texcoord.u, &texcoord.v);

      array_push(texcoords, texcoord);
    }

    line = nextLine + 1;
  }

  array_free(texcoords);

  free(text);
}

void load_mesh_png_data(mesh* mesh, char* png_filename) {
    upng_t* png_image = upng_new_from_file(png_filename);

    if (png_image != NULL) {
        upng_decode(png_image);
        if (upng_get_error(png_image) == UPNG_EOK) {
           mesh->texture = png_image;
        }
    }
}

int get_num_meshes() {
  return num_meshes;
}

mesh* get_mesh(int index) {
  return &meshes[index];
}

void free_meshes() {
  for (int i = 0; i < num_meshes; i++) {
    array_free(meshes[i].vertices);
    array_free(meshes[i].faces);
    upng_free(meshes[i].texture);
  }
}

