#ifndef _MESH_DATA_H_
#define _MESH_DATA_H_

#include <memory>

#include "vectors.h"
#include <CGAL/Surface_mesh.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Surface_mesh<K::Point_3> CGAL_MESH;
typedef CGAL::Polyhedron_3<K> Polyhedron;

// ====================================================================
// ====================================================================

struct faceIndex {
  int data[3];
};

// a homogeneous 3D point or a color with alpha
typedef struct float3 {
  float data[3];
} float3;

// a homogenous 3D point or a color with alpha
typedef struct float4 {
  float data[4];
} float4;

// a vertex with position, normal, and color
typedef struct float12 {
  float data[12];
} float12;

// a 4x4 matrix
typedef struct float16 {
  float data[16];
} float16;


typedef struct MeshData {

  // REPRESENTATION
  int width;
  int height;
  bool perspective;

  std::vector<float> vertexPositions;
  std::vector<uint32_t> triIndices;
  std::vector<uint32_t> tetIndices;

  uint32_t vertexCount;
  uint32_t positionCount;
  uint32_t triCount;
  uint32_t triIndCount;
  uint32_t tetCount;
  uint32_t tetIndCount;

  float3 bb_center;
  float bb_scale;

  void Load(std::string input_file);

  bool animate;
  float timestep;
  float min_y;

} MeshData;

void INIT_MeshData(MeshData *mesh_data);

// ====================================================================
// ====================================================================

#endif
