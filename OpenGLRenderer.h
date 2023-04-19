// ==================================================================
// OpenGL Rendering of the Mesh Data
// ==================================================================

#include "OpenGLCanvas.h"
#include "argparser.h"
#include "meshdata.h"

class OpenGLRenderer {

 public:

  OpenGLRenderer(MeshData *_mesh_data, ArgParser *args);

  // HELPER FUNCTIONS FOR RENDERING
  void setupVBOs();
  void updateVBOs();
  void drawVBOs(const glm::mat4 &MVP,const glm::mat4 &M,const glm::mat4 &V);
  void cleanupVBOs();

 private:

  // private helper functions for VBOs
  void setupMesh();
  void drawMesh() const;
  void cleanupMesh();

  // REPRESENTATION
  MeshData *mesh_data;

  GLuint VaoId;
  GLuint VboId;

  GLuint MatrixID;
  GLuint LightID;
  GLuint ViewMatrixID;
  GLuint ModelMatrixID;
  GLuint wireframeID;

};


class VertexPosColor {
 public:
  // by default nodes are set to black
  VertexPosColor(const glm::vec4 &pos=glm::vec4(0,0,0,1),
                 const glm::vec4 &color=glm::vec4(0,0,0,1)) :
      XYZW(pos),RGBA(color) {}
  glm::vec4 XYZW;
  glm::vec4 RGBA;
};
