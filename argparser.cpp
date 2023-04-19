// ================================================================
// Parse the command line arguments and the input file
// ================================================================

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "argparser.h"
#include "meshdata.h"
#include "triangulator.h"

ArgParser *GLOBAL_args;

ArgParser::ArgParser(int argc, const char *argv[], MeshData *_mesh_data) {

  mesh_data = _mesh_data;

  // parse the command line arguments
  for (int i = 1; i < argc; i++) {
    if (std::string(argv[i]) == std::string("--input")) {
      i++; assert (i < argc);
      separatePathAndFile(argv[i],path,input_file);
    } else if (std::string(argv[i]) == std::string("--size")) {
      i++; assert (i < argc);
      mesh_data->width = atoi(argv[i]);
      i++; assert (i < argc);
      mesh_data->height = atoi(argv[i]);
    } else if (std::string(argv[i]) == std::string("--triangulate")) {
      i++; assert (i < argc);
      auto input = std::string(argv[i]);
      i++; assert (i < argc);
      auto output = std::string(argv[i]);
      Triangulator::Triangulate(input, output);
      exit(0);
    } else if (std::string(argv[i]) == std::string("--time-step")) {
      i++; assert (i < argc);
      mesh_data->timestep = std::stof(std::string(argv[i]));
    }else {
      std::cout << "ERROR: unknown command line argument "
      << i << ": '" << argv[i] << "'" << std::endl;
      exit(1);
    }
  }

  mesh_data->Load(path+"/"+input_file);

  GLOBAL_args = this;

  mesh = new fractureMesh(mesh_data);
  mesh->packMesh(mesh_data);
}

void ArgParser::Load() {
    delete mesh;
    mesh = new fractureMesh(mesh_data);
    mesh->packMesh(mesh_data);
}

void ArgParser::separatePathAndFile(const std::string &input, std::string &path, std::string &file) {
  // we need to separate the filename from the path
  // (we assume the vertex & fragment shaders are in the same directory)
  // first, locate the last '/' in the filename
  size_t last = std::string::npos;
  while (1) {
    int next = input.find('/',last+1);
    if (next != (int)std::string::npos) {
      last = next;
      continue;
    }
    next = input.find('\\',last+1);
    if (next != (int)std::string::npos) {
      last = next;
      continue;
    }
    break;
  }
  if (last == std::string::npos) {
    // if there is no directory in the filename
    file = input;
    path = ".";
  } else {
    // separate filename & path
    file = input.substr(last+1,input.size()-last-1);
    path = input.substr(0,last);
  }
}






