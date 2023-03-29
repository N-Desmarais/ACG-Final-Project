//
// Created by njdesmarais on 3/28/23.
//

#include <cstring>
#include "mesh.h"

void Mesh::packMesh(MeshData *data) {

  num_vertices = data->vertexCount;
  num_triangles = data->triCount;
  num_data = 12 * 3 * num_triangles;

  vertices = std::unique_ptr<Vertex>(new Vertex[num_vertices]);
  triangles = std::unique_ptr<Triangle>(new Triangle[num_triangles]);
  tri_data = std::unique_ptr<float>(new float[num_data]);

  auto vertices_ptr = vertices.get();
  auto positions_ptr = data->vertexPositions.get();
  auto data_ptr = tri_data.get();

  for(uint32_t ind = 0; ind < data->positionCount; ind += 3) {
    auto pos = Vec3f(positions_ptr[ind],positions_ptr[ind+1],positions_ptr[ind+2]);
    vertices_ptr[ind/3] = Vertex(pos);
  }

  auto triangles_ptr = triangles.get();
  auto indices_ptr = data->faceIndices.get();

  for(uint32_t ind = 0; ind < data->indexCount; ind += 3) {
    auto v1 = &vertices_ptr[indices_ptr[ind]],
         v2 = &vertices_ptr[indices_ptr[ind+1]],
         v3 = &vertices_ptr[indices_ptr[ind+2]];
    triangles_ptr[ind/3] = Triangle(v1,v2,v3);
  }

  float * current = data_ptr;

  for(int tri = 0; tri < num_triangles; tri++) {
    Triangle t = triangles_ptr[tri];

    Vec3f a = t[0]->getPos();
    Vec3f b = t[1]->getPos();
    Vec3f c = t[2]->getPos();

    // for flat shading
    Vec3f normal = t.getNormal();
    Vec3f na = normal;
    Vec3f nb = normal;
    Vec3f nc = normal;

    float12 ta = { float(a.x()),float(a.y()),float(a.z()),1, float(na.x()),float(na.y()),float(na.z()),0, 1.0,1.0,1.0,1 };
    float12 tb = { float(b.x()),float(b.y()),float(b.z()),1, float(nb.x()),float(nb.y()),float(nb.z()),0, 1.0,1.0,1.0,1 };
    float12 tc = { float(c.x()),float(c.y()),float(c.z()),1, float(nc.x()),float(nc.y()),float(nc.z()),0, 1.0,1.0,1.0,1 };
    memcpy(current, &ta, sizeof(float)*12); current += 12;
    memcpy(current, &tb, sizeof(float)*12); current += 12;
    memcpy(current, &tc, sizeof(float)*12); current += 12;
  }

}
