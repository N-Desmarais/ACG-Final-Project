//
// Created by njdesmarais on 4/18/23.
//

#include <vector>


#include "fracture.h"

using namespace std;


fractureMesh::fractureMesh(MeshData * data, float timestep) {
  this->timestep = timestep;

  for(int i = 0; i < data->positionCount; i+=3) {
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

  for(int i = 0; i < data->tetIndCount; i+=4) {
    auto p1 = &nodes[data->tetIndices[i]],
         p2 = &nodes[data->tetIndices[i+1]],
         p3 = &nodes[data->tetIndices[i+2]],
         p4 = &nodes[data->tetIndices[i+3]];

    auto e = fractureElement(p1,p2,p3,p4);
    elements.push_back(e);
  }

  numNodes = nodes.size();
  numElements = elements.size();
  numTriangles = numElements * 4;
}


void fractureMesh::animate() {
  auto accelerations = vector<Vec3f>();
  auto velocities = vector<Vec3f>();
  auto positions = vector<Vec3f>();

  for(int i = 0; i < numNodes; i++) {
    auto particle = nodes[i];
    auto force = Vec3f(0,0,0); // TODO basic forces
    auto acceleration = force * (1/particle.getMass());
    auto new_vel = particle.getVelocity() + (acceleration * timestep);
    auto new_pos = particle.getPosition() + (new_vel * timestep);

    accelerations.push_back(acceleration);
    velocities.push_back(new_vel);
    positions.push_back(new_pos);
  }

  for(int i = 0; i < numNodes; i++) {
    auto particle = nodes[i];
    particle.setAcceleration(accelerations[i]);
    particle.setVelocity(velocities[i]);
    particle.setPosition(positions[i]);
  }

}

void fractureMesh::packMesh() {

}
