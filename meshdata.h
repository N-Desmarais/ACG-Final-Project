#ifndef _MESH_DATA_H_
#define _MESH_DATA_H_

// ====================================================================
// ====================================================================

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
  int wireframe;

  int triCount;
  float* triData;

  float3 bb_center;
  float bb_scale;

} MeshData;


void INIT_MeshData(MeshData *mesh_data);
void loadOBJ(MeshData *mesh_data);

// ====================================================================
// ====================================================================

#endif
