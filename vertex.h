#ifndef _VERTEX_H
#define _VERTEX_H

#include "vectors.h"

// ==========================================================

// TODO this class doesn't currently serve any purpose
// should be used for shading later on, work around it for now

class Vertex {

 public:


  Vertex() : position(Vec3f(0,0,0)) {}
  Vertex(const Vec3f &pos) : position(pos) {}

  double x() const { return position.x(); }
  double y() const { return position.y(); }
  double z() const { return position.z(); }
  const Vec3f& getPos() const { return position; }

  void setPos(const Vec3f &v) { position = v; }

 private:
  Vec3f position;

};

// ==========================================================

#endif

