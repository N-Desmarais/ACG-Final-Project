#ifndef _OPEN_GL_CANVAS_H_
#define _OPEN_GL_CANVAS_H_

// Graphics Library Includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>

class ArgParser;
class MeshData;
class OpenGLCamera;
class OpenGLRenderer;

// ====================================================================
// NOTE:  All the methods and variables of this class are static
// ====================================================================

class OpenGLCanvas {

 public:

  // various static variables
  static ArgParser *args;
  static MeshData *mesh_data;
  static OpenGLCamera* camera;
  static OpenGLRenderer* renderer;
  static GLFWwindow* window;

  // mouse position
  static int mouseX;
  static int mouseY;
  // which mouse button
  static bool leftMousePressed;
  static bool middleMousePressed;
  static bool rightMousePressed;
  // current state of modifier keys
  static bool shiftKeyPressed;
  static bool controlKeyPressed;
  static bool altKeyPressed;
  static bool superKeyPressed;

  static void initialize(ArgParser *_args, MeshData *_mesh_data, OpenGLRenderer *_renderer);

  // Callback functions for mouse and keyboard events
  static void mousebuttonCB(GLFWwindow *window, int which_button, int action, int mods);
  static void mousemotionCB(GLFWwindow *window, double x, double y);
  static void keyboardCB(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void error_callback(int error, const char* description);
};

// ====================================================================

// helper functions
GLuint LoadShaders(const std::string &vertex_file_path,const std::string &fragment_file_path);
std::string WhichGLError(GLenum &error);
int HandleGLError(const std::string &message = "", bool ignore = false);

#endif
