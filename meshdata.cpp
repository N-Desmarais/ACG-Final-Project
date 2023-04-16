#include <random>
#include "meshdata.h"
#include "argparser.h"


#include <CGAL/boost/graph/copy_face_graph.h>

// ====================================================================


// default values for the MeshData variables
void INIT_MeshData(MeshData *mesh_data) {
  mesh_data->width = 400;
  mesh_data->height = 400;
  mesh_data->perspective = true;
  mesh_data->triCount = 0;
}


// ====================================================================
// ====================================================================

// NOTE: These functions are called by the Objective-C code, so we
// need this extern to allow C code to call C++ functions (without
// function name mangling confusion).

// Also, they use global variables...

extern "C" {
  void PackMesh() {
    printf ("Pack Mesh\n");
    GLOBAL_args->mesh->packMesh(GLOBAL_args->mesh_data);
  }
}

void MeshData::Load(std::string input_file) {
  CGAL_MESH m;
  std::ifstream input;
  input.open(input_file);
  if(!CGAL::IO::read_PLY(input,m)) {
      exit(1);
  }

  vertexCount = 0;
  positionCount = 0;
  triCount = 0;
  indexCount = 0;

  //Get vertices ...
  for (CGAL_MESH::Vertex_index vi : m.vertices()) {
    K::Point_3 pt = m.point(vi);
    vertexPositions.push_back((float)pt.x());
    vertexPositions.push_back((float)pt.y());
    vertexPositions.push_back((float)pt.z());
    vertexCount += 1;
    positionCount += 3;
  }

  //Get face indices ...
  for (CGAL_MESH::Face_index face_index : m.faces()) {
    CGAL::Vertex_around_face_circulator<CGAL_MESH> vcirc(m.halfedge(face_index), m), done(vcirc);
    do {
      faceIndices.push_back(*vcirc++);
      indexCount += 1;
    }
    while (vcirc != done);
    triCount += 1;
  }

  CGAL::copy_face_graph(m, polyhedron);

  return;
}
