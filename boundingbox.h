#ifndef _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_

#include <cassert>
#include <algorithm>
#include "vectors.h"

// Because getting std::max & std::min to work on all platforms is annoying
inline double mymax(double x, double y) { if (x > y) return x; return y; }
inline double mymin(double x, double y) { if (x < y) return x; return y; }

// ====================================================================
// ====================================================================

class BoundingBox {

 public:

  // ========================
  // CONSTRUCTOR & DESTRUCTOR
  BoundingBox() {
    Set(Vec3f(0,0,0),Vec3f(0,0,0)); }
  BoundingBox(const Vec3f &pt) {
    Set(pt,pt); }
  BoundingBox(const Vec3f &_minimum, const Vec3f &_maximum) {
    Set(_minimum,_maximum); }

  // =========
  // ACCESSORS
  void Get(Vec3f &_minimum, Vec3f &_maximum) const {
    _minimum = minimum;
    _maximum = maximum; }
  Vec3f getMin() const { return minimum; }
  Vec3f getMax() const { return maximum; }
  void getCenter(Vec3f &c) const {
    c = maximum;
    c -= minimum;
    c *= 0.5f;
    c += minimum;
  }
  double maxDim() const {
    double x = maximum.x() - minimum.x();
    double y = maximum.y() - minimum.y();
    double z = maximum.z() - minimum.z();
    return mymax(x,mymax(y,z));
  }

  // =========
  // MODIFIERS
  void Set(const BoundingBox &bb) {
    minimum = bb.minimum;
    maximum = bb.maximum; }
  void Set(const Vec3f &_minimum, const Vec3f &_maximum) {
    assert (minimum.x() <= maximum.x() &&
        minimum.y() <= maximum.y() &&
        minimum.z() <= maximum.z());
    minimum = _minimum;
    maximum = _maximum; }
  void Extend(const Vec3f v) {
    minimum = Vec3f(mymin(minimum.x(),v.x()),
                    mymin(minimum.y(),v.y()),
                    mymin(minimum.z(),v.z()));
    maximum = Vec3f(mymax(maximum.x(),v.x()),
                    mymax(maximum.y(),v.y()),
                    mymax(maximum.z(),v.z()));
  }
  void Extend(const BoundingBox &bb) {
    Extend(bb.minimum);
    Extend(bb.maximum);
  }

 private:

  // ==============
  // REPRESENTATION
  Vec3f minimum;
  Vec3f maximum;
};

// ====================================================================
// ====================================================================

#endif
