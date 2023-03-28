//
// Created by njdesmarais on 3/28/23.
//

#include <string>

#ifndef PROJECT__MESH_H_
#define PROJECT__MESH_H_

#endif //PROJECT__MESH_H_

struct MeshData;
class ArgParser;

class Mesh {
 public:


  Mesh(ArgParser *a) { args = a; }
  ~Mesh();

  int num_triangles;
  float * tri_data;
  ArgParser *args;

  void Load(std::string input_file) {
    // TODO implement
    num_triangles = 0;
    tri_data = nullptr;
  }

  void packMesh(MeshData * data) {
    // TODO implement
  }
};
