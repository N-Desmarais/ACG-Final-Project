#include <random>
#include <fstream>

#include "meshdata.h"
#include "argparser.h"
#include "CGAL/boost/graph/copy_face_graph.h"

void INIT_MeshData(MeshData *mesh_data) {
  mesh_data->width = 400;
  mesh_data->height = 400;
  mesh_data->perspective = true;
  mesh_data->triCount = 0;
  mesh_data->animate = false;
  mesh_data->timestep = 0.1;
}

// NOTE: These functions are called by the Objective-C code, so we
// need this extern to allow C code to call C++ functions (without
// function name mangling confusion).

// Also, they use global variables...

extern "C" {
  void PackMesh() {
    printf ("Pack Mesh\n");
    GLOBAL_args->mesh->packMesh();
  }

  void Step() {
    GLOBAL_args->mesh->animate();
  }

  void Animate() {
    if(GLOBAL_args->mesh_data->animate){
      for (int i = 0; i < 10; i++) {
        Step();
      }
      PackMesh();
    }
  }

  void Load() {
    GLOBAL_args->Load();
  }
}

void MeshData::Load(std::string input_file) {

  vertexCount = 0;
  positionCount = 0;
  triCount = 0;
  triIndCount = 0;
  tetCount = 0;
  tetIndCount = 0;
  min_y = 0;

  auto nodes = input_file + ".node",
       tris = input_file + ".face",
       tets = input_file + ".ele";

  // nodes
  std::ifstream infile(nodes);
  infile >> vertexCount;
  positionCount = vertexCount * 3;

  uint32_t dim,attr,bound;

  infile >> dim >> attr >> bound;
  assert(dim == 3);   // 3 dimensions
  assert(attr == 0);  // no attributes
  assert(bound == 0); // no boundary marker

  uint32_t ind;
  float x,y,z;

  while (infile >> ind >> x >> y >> z) {
    vertexPositions.push_back(x);
    vertexPositions.push_back(y);
    vertexPositions.push_back(z);

    if(y < min_y) min_y = y;
  }
  infile.close();

  min_y -= 5; // floor y

  // triangles
  infile = std::ifstream(tris);
  infile >> triCount;
  triIndCount = triCount * 3;

  infile >> bound;
  assert(bound == 0); // no boundary marker

  uint32_t v1,v2,v3;

  while (infile >> ind >> v1 >> v2 >> v3) {
    triIndices.push_back(v1-1);
    triIndices.push_back(v2-1);
    triIndices.push_back(v3-1);
  }
  infile.close();

  // tetrahedron
  infile = std::ifstream(tets);
  infile >> tetCount;
  tetIndCount = tetCount * 4;

  infile >> dim >> attr;
  assert(dim == 4);  // 4 nodes per tet
  assert(attr == 0); // no attributes

  uint32_t v4;

  while(infile >> ind >> v1 >> v2 >> v3 >> v4) {
    tetIndices.push_back(v1-1);
    tetIndices.push_back(v2-1);
    tetIndices.push_back(v3-1);
    tetIndices.push_back(v4-1);
  }
  infile.close();

  assert(positionCount == vertexPositions.size());
  assert(triIndCount == triIndices.size());
  assert(tetIndCount == tetIndices.size());
}
