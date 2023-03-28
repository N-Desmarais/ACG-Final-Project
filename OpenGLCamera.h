#ifndef _OPEN_GL_CAMERA_H_
#define _OPEN_GL_CAMERA_H_

#include <cassert>
#include <iostream>
#include <fstream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ====================================================================
// ====================================================================

class OpenGLCamera {
 public:
  // CONSTRUCTOR & DESTRUCTOR
  OpenGLCamera(const glm::vec3 &c, const glm::vec3 &poi, const glm::vec3 &u);
  virtual ~OpenGLCamera() {}

  // GL NAVIGATION
  virtual void glPlaceCamera() = 0;
  void dollyCamera(float dist);
  virtual void zoomCamera(float dist) = 0;
  void truckCamera(float dx, float dy);
  void rotateCamera(float rx, float ry);

  const glm::mat4& getViewMatrix() const { return ViewMatrix; }
  const glm::mat4& getProjectionMatrix() const { return ProjectionMatrix; }

  friend std::ostream& operator<< (std::ostream& ostr, const OpenGLCamera &c);
  friend std::istream& operator>> (std::istream& istr, OpenGLCamera &c);

 public:
  //protected:
  OpenGLCamera() { assert(0); } // don't use

  // HELPER FUNCTIONS
  glm::vec3 getHorizontal() const {
    return glm::normalize(glm::cross(getDirection(),up));
  }
  glm::vec3 getScreenUp() const {
    return glm::normalize(glm::cross(getHorizontal(),getDirection()));
  }
  glm::vec3 getDirection() const {
    return glm::normalize(point_of_interest - camera_position);
  }

  // REPRESENTATION
  glm::vec3 point_of_interest;
  glm::vec3 camera_position;
  glm::vec3 up;
  int width;
  int height;
  glm::mat4 ViewMatrix;
  glm::mat4 ProjectionMatrix;
};

// ====================================================================

class OrthographicOpenGLCamera : public OpenGLCamera {
 public:
  // CONSTRUCTOR & DESTRUCTOR
  OrthographicOpenGLCamera(const glm::vec3 &c = glm::vec3(0,0,1),
                           const glm::vec3 &poi = glm::vec3(0,0,0),
                           const glm::vec3 &u = glm::vec3(0,1,0),
                           float s=100);

  // GL NAVIGATION
  void glPlaceCamera();
  void zoomCamera(float factor);

  friend std::ostream& operator<< (std::ostream& ostr, const OrthographicOpenGLCamera &c);
  friend std::istream& operator>> (std::istream& istr, OrthographicOpenGLCamera &c);

 private:
  // REPRESENTATION
  float size;
};

// ====================================================================

class PerspectiveOpenGLCamera : public OpenGLCamera {
 public:
  // CONSTRUCTOR & DESTRUCTOR
  PerspectiveOpenGLCamera(const glm::vec3 &c = glm::vec3(0,0,1),
                          const glm::vec3 &poi = glm::vec3(0,0,0),
                          const glm::vec3 &u = glm::vec3(0,1,0),
                          float a = 45);

  // GL NAVIGATION
  void glPlaceCamera();
  void zoomCamera(float dist);

  friend std::ostream& operator<< (std::ostream& ostr, const PerspectiveOpenGLCamera &c);
  friend std::istream& operator>> (std::istream& istr, PerspectiveOpenGLCamera &c);

 private:
  // REPRESENTATION
  float angle;
};

// ====================================================================

#endif

