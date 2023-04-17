//
// Created by njdesmarais on 3/28/23.
//

#include <cstring>

#include "mesh.h"

typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;
using namespace CGAL::parameters;

void Mesh::packMesh(MeshData *data) {
  num_vertices = data->vertexCount;
  num_triangles = data->triCount;
  num_data = 12 * 3 * num_triangles;
  tri_data = std::unique_ptr<float>(new float[num_data]);

  auto positions = data->vertexPositions;
  auto indices = data->triIndices;

  for(uint32_t ind = 0; ind < data->positionCount; ind += 3) {
    auto pos = Vec3f(positions[ind],positions[ind+1],positions[ind+2]);
    vertices.push_back(Vertex(pos));
  }

  for(uint32_t ind = 0; ind < data->triIndCount; ind += 3) {
    auto v1 = &vertices[indices[ind]-1],
         v2 = &vertices[indices[ind+1]-1],
         v3 = &vertices[indices[ind+2]-1];
    triangles.push_back(Triangle(v1,v2,v3));
  }

  float * current = tri_data.get();

  for(int tri = 0; tri < num_triangles; tri++) {
    Triangle t = triangles[tri];

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

  bbox = BoundingBox();

  for(int i = 0; i < num_vertices; i++) {
    bbox.Extend(vertices[i].getPos());
  }

  Vec3f center;
  bbox.getCenter(center);

  data->bb_center.data[0] = center.x();
  data->bb_center.data[1] = center.y();
  data->bb_center.data[2] = center.z();

  data->bb_scale = 1.8 / float(bbox.maxDim());
}

