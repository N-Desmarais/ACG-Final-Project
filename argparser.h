// ================================================================
// Parse the command line arguments and the input file
// ================================================================

#ifndef __ARG_PARSER_H__
#define __ARG_PARSER_H__

#include <string>
#include <random>
#include "mesh.h"
#include "meshdata.h"

class BoundingBox;

// ======================================================================
// Class to collect all the high-level rendering parameters controlled
// by the command line or the keyboard input
// ======================================================================

class ArgParser {

 public:

  ArgParser(int argc, const char *argv[], MeshData *_mesh_data);

  // helper functions
  void separatePathAndFile(const std::string &input, std::string &path, std::string &file);

  void DefaultValues();

  // ==============
  // REPRESENTATION
  // all public! (no accessors)

  MeshData *mesh_data;
  Mesh *mesh;

  std::string input_file;
  std::string path;
};

extern ArgParser *GLOBAL_args;

#endif
