#include <iostream>
#include "argparser.h"
#include "meshdata.h"
#include "OpenGLRenderer.h"

MeshData *mesh_data;

int main(int argc, const char * argv[]) {

  MeshData mymesh_data;
  mesh_data = &mymesh_data;
  INIT_MeshData(mesh_data);
  ArgParser args(argc, argv, mesh_data);
  OpenGLRenderer opengl_renderer(mesh_data,&args);

}
