//
// Created by njdesmarais on 3/28/23.
//

#include <string>
#include "miniply.h"
#include "vertex.h"
#include "triangle.h"
#include "memory"
#include "meshdata.h"

#ifndef PROJECT__MESH_H_
#define PROJECT__MESH_H_

#endif //PROJECT__MESH_H_

class ArgParser;

class Mesh {
 public:


  Mesh(ArgParser *a) { args = a; }
  ~Mesh();

  std::unique_ptr<Vertex> vertices;
  std::unique_ptr<Triangle> triangles;
  std::unique_ptr<float> tri_data;

  int num_vertices;
  int num_triangles;
  int num_data;

  ArgParser *args;

  void packMesh(MeshData * data);
};
