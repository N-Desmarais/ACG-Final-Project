#include <random>
#include "meshdata.h"
#include "argparser.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>

typedef CGAL::Simple_cartesian<double> K;
typedef CGAL::Surface_mesh<K::Point_3> CGAL_MESH;



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

  miniply::PLYReader reader(input_file.c_str());

  if (!reader.valid()) {
    exit(1);
  }

  uint32_t faceIdxs[3];
  uint32_t posIdxs[3];
  bool gotVerts = false, gotFaces = false;

  miniply::PLYElement* faceElem = reader.get_element(reader.find_element(miniply::kPLYFaceElement));
  if (faceElem == nullptr) {
    exit(1);
  }
  faceElem->convert_list_to_fixed_size(faceElem->find_property("vertex_indices"), 3, faceIdxs);

  while (reader.has_element() && (!gotVerts || !gotFaces)) {
    if (reader.element_is(miniply::kPLYVertexElement) && reader.load_element() && reader.find_pos(posIdxs)) {
      vertexCount = reader.num_rows();
      positionCount = vertexCount * 3; // 3 = x y z for each vert
      vertexPositions = std::unique_ptr<float>(new float[positionCount]);
      reader.extract_properties(posIdxs, 3, miniply::PLYPropertyType::Float, vertexPositions.get());
      gotVerts = true;
    }
    else if (!gotFaces && reader.element_is(miniply::kPLYFaceElement) && reader.load_element()) {
      triCount = reader.num_rows();
      indexCount = triCount * 3; // 3 = p1 p2 p3 for each tri
      faceIndices = std::unique_ptr<int>(new int[indexCount]);
      reader.extract_properties(faceIdxs, 3, miniply::PLYPropertyType::Int, faceIndices.get());
      gotFaces = true;
    }
    if (gotVerts && gotFaces) {
      break;
    }
    reader.next_element();
  }

  if (!gotVerts || !gotFaces) {
    exit(1);
  }

  CGAL_MESH m;
  std::ifstream input;
  input.open(input_file);
  if(!CGAL::IO::read_PLY(input,m)) {
      exit(1);
  }

  return;
}
