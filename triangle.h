#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <cstdlib>
#include "vector"
#include "vertex.h"


class Triangle {

 public:

  Triangle() {}
  Triangle(Vertex * v1, Vertex * v2, Vertex * v3) {
    vertices = {v1,v2,v3};
    normal = calculateSurfaceNormal();
  }

  Vec3f getNormal() const { return normal; }

  Vertex* operator[](int i) const { return vertices[i]; }

  Vec3f calculateSurfaceNormal() {

    auto p1 = this->operator[](0)->getPos();
    auto p2 = this->operator[](1)->getPos();
    auto p3 = this->operator[](2)->getPos();
    auto U = p2 - p1;
    auto V = p3 - p1;
    auto Nx = (U.y() * V.z()) - (U.z() * V.y());
    auto Ny = (U.z() * V.x()) - (U.x() * V.z());
    auto Nz = (U.x() * V.y()) - (U.y() * V.x());

    return Vec3f { Nx, Ny, Nz };
  }

 protected:
  std::vector<Vertex*> vertices;
  Vec3f normal;
};

// ===========================================================

#endif
