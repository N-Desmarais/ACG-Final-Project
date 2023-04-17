//
// Created by njdesmarais on 3/28/23.
//

#include <string>
#include "vertex.h"
#include "triangle.h"
#include "memory"
#include "meshdata.h"
#include "boundingbox.h"


#include "CGAL/Exact_predicates_inexact_constructions_kernel.h"
#include "CGAL/Mesh_triangulation_3.h"
#include "CGAL/Mesh_complex_3_in_triangulation_3.h"
#include "CGAL/Polyhedron_3.h"
#include "CGAL/Polyhedral_mesh_domain_3.h"
#include "CGAL/Mesh_criteria_3.h"
#include "CGAL/make_mesh_3.h"
#include "CGAL/boost/graph/helpers.h"
#include "CGAL/IO/File_tetgen.h"


#ifndef PROJECT__MESH_H_
#define PROJECT__MESH_H_


#ifdef CGAL_CONCURRENT_MESH_3
typedef CGAL::Parallel_tag Concurrency_tag;
#else
typedef CGAL::Sequential_tag Concurrency_tag;
#endif

// Domain
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polyhedron_3<K> Polyhedron;
typedef CGAL::Polyhedral_mesh_domain_3<Polyhedron, K> Mesh_domain;
// Triangulation
typedef CGAL::Mesh_triangulation_3<Mesh_domain,CGAL::Default,Concurrency_tag>::type Tr;
typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;


class ArgParser;

class Mesh {
 public:


  Mesh(ArgParser *a) { args = a; }
  ~Mesh();
  
  std::vector<Vertex> vertices;
  std::vector<Triangle> triangles;
  std::unique_ptr<float> tri_data;

  int num_vertices;
  int num_triangles;
  int num_data;

  BoundingBox bbox;
  ArgParser *args;

  void packMesh(MeshData * data);
};

#endif //PROJECT__MESH_H_
