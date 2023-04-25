//
// Created by njdesmarais on 4/18/23.
//

#include <vector>


#include "fracture.h"
#include "triangle.h"
#include "vertex.h"
#include "argparser.h"

using namespace std;


fractureMesh::fractureMesh(MeshData * data) {

  for(uint32_t i = 0; i < data->positionCount; i+=3) {
    auto x = data->vertexPositions[i],
         y = data->vertexPositions[i+1],
         z = data->vertexPositions[i+2];

    auto pos = Vec3f(x,y,z),
         empty = Vec3f(0,0,0);
    auto n = fractureNode();
    n.setOriginalPosition(pos);
    n.setPosition(pos);
    n.setAcceleration(empty);
    n.setVelocity(empty);
    n.setMass(0.1);
    nodes.push_back(n);
  }

  for(uint32_t i = 0; i < data->tetIndCount; i+=4) {
    auto p1 = &nodes[data->tetIndices[i]],
         p2 = &nodes[data->tetIndices[i+1]],
         p3 = &nodes[data->tetIndices[i+2]],
         p4 = &nodes[data->tetIndices[i+3]];

    auto e = fractureElement(p1,p2,p3,p4);

    p1->addElement(&e);

    elements.push_back(e);
  }

  for(uint32_t i = 0; i < data->triIndCount; i += 3) {
    auto p1 = &nodes[data->triIndices[i]],
         p2 = &nodes[data->triIndices[i+1]],
         p3 = &nodes[data->triIndices[i+2]];

    auto t = fractureSurfaceTri(p1,p2,p3);

    surfaceTris.push_back(t);
  }

  numNodes = nodes.size();
  numElements = elements.size();
  numSurfaceTris = surfaceTris.size();
  numTriangles = numElements * 4 + numSurfaceTris;

  updateBBox();
}


void fractureMesh::animate() {
  auto accelerations = vector<Vec3f>();
  auto velocities = vector<Vec3f>();
  auto positions = vector<Vec3f>();
  auto timestep = GLOBAL_args->mesh_data->timestep;

  for(uint32_t i = 0; i < numNodes; i++) {
    auto particle = nodes[i];
    auto force = Vec3f(0,-9.8,0); // TODO basic forces
    auto acceleration = force * (1/particle.getMass());
    auto new_vel = particle.getVelocity() + (acceleration * timestep);
    auto new_pos = particle.getPosition() + (new_vel * timestep);

    accelerations.push_back(acceleration);
    velocities.push_back(new_vel);
    positions.push_back(new_pos);
  }

  for(uint32_t i = 0; i < numNodes; i++) {
    auto particle = &nodes[i];
    particle->setAcceleration(accelerations[i]);
    particle->setVelocity(velocities[i]);
    particle->setPosition(positions[i]);
  }

  updateBBox();
}

void fractureMesh::packMesh() {
  auto num_data = 12 * 3 * numTriangles;
  tri_data = unique_ptr<float>(new float[num_data]);

  auto triangles = vector<Triangle>();
  auto volume_deltas = vector<double>();

  for(uint32_t i = 0; i < numSurfaceTris; i++) {
      auto surfaceTri = surfaceTris[i];
      auto p1 = new Vertex(surfaceTri[0]->getPosition()),
           p2 = new Vertex(surfaceTri[1]->getPosition()),
           p3 = new Vertex(surfaceTri[2]->getPosition());

      auto t = Triangle(p1,p2,p3);

      triangles.push_back(t);
  }

  for(uint32_t i = 0; i < numElements; i++) {
    auto element = elements[i];
    auto p1 = new Vertex(element[0]->getPosition()),
         p2 = new Vertex(element[1]->getPosition()),
         p3 = new Vertex(element[2]->getPosition()),
         p4 = new Vertex(element[3]->getPosition());

    auto t1 = Triangle(p1,p2,p3),
         t2 = Triangle(p1,p2,p4),
         t3 = Triangle(p1,p3,p4),
         t4 = Triangle(p2,p3,p4);

    triangles.push_back(t1);
    triangles.push_back(t2);
    triangles.push_back(t3);
    triangles.push_back(t4);

    auto delta = fabs(element.getOriginalVolume() - element.computeVolume());

    volume_deltas.push_back(delta);
    volume_deltas.push_back(delta);
    volume_deltas.push_back(delta);
    volume_deltas.push_back(delta);
  }



  float * current = tri_data.get();

  for(uint32_t tri = 0; tri < numTriangles; tri++) {
    Triangle t = triangles[tri];

    Vec3f a = t[0]->getPos();
    Vec3f b = t[1]->getPos();
    Vec3f c = t[2]->getPos();

    auto center = Vec3f(0,0,0);
    bbox.getCenter(center);

    if(GLOBAL_args->cross_section && a.x() < center.x() && b.x() < center.x() && c.x() < center.x()) {
      continue;
    }

    float color_r,color_g,color_b,color_a;

    // color the surface white and the interior red based on volume delta
    color_r = tri < numSurfaceTris ? 1.0 : 0.0;
    color_g = tri < numSurfaceTris ? 1.0 : 0.0;
    color_b = tri < numSurfaceTris ? 1.0 : 1.0;
    color_a = tri < numSurfaceTris ? 1.0 : 0.5;

    // for flat shading
    Vec3f normal = t.getNormal();
    Vec3f na = normal;
    Vec3f nb = normal;
    Vec3f nc = normal;

    float12 ta = { float(a.x()),float(a.y()),float(a.z()),1, float(na.x()),float(na.y()),float(na.z()),0, color_r,color_g,color_b,color_a };
    float12 tb = { float(b.x()),float(b.y()),float(b.z()),1, float(nb.x()),float(nb.y()),float(nb.z()),0, color_r,color_g,color_b,color_a };
    float12 tc = { float(c.x()),float(c.y()),float(c.z()),1, float(nc.x()),float(nc.y()),float(nc.z()),0, color_r,color_g,color_b,color_a };
    memcpy(current, &ta, sizeof(float)*12); current += 12;
    memcpy(current, &tb, sizeof(float)*12); current += 12;
    memcpy(current, &tc, sizeof(float)*12); current += 12;
  }

}

void fractureMesh::updateBBox() {
  bbox = BoundingBox();

  for(uint32_t i = 0; i < numNodes; i++) {
    bbox.Extend(nodes[i].getPosition());
  }

  Vec3f center;
  bbox.getCenter(center);

  GLOBAL_args->mesh_data->bb_center.data[0] = center.x();
  GLOBAL_args->mesh_data->bb_center.data[1] = center.y();
  GLOBAL_args->mesh_data->bb_center.data[2] = center.z();

  GLOBAL_args->mesh_data->bb_scale = 1.8 / float(bbox.maxDim());
}
