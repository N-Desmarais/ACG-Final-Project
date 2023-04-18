//
// Created by njdesmarais on 4/17/23.
//

#ifndef PROJECT__TRIANGULATOR_H_
#define PROJECT__TRIANGULATOR_H_

#include <string>

#include "CGAL/Surface_mesh.h"
#include "CGAL/Exact_predicates_inexact_constructions_kernel.h"
#include "CGAL/Mesh_triangulation_3.h"
#include "CGAL/Mesh_complex_3_in_triangulation_3.h"
#include "CGAL/Polyhedron_3.h"
#include "CGAL/Polyhedral_mesh_domain_3.h"
#include "CGAL/Mesh_criteria_3.h"
#include "CGAL/make_mesh_3.h"
#include "CGAL/boost/graph/helpers.h"
#include "CGAL/IO/File_tetgen.h"




// Kernel
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// Surface Mesh
typedef CGAL::Surface_mesh<K::Point_3> CGAL_MESH;
// Polyhedron
typedef CGAL::Polyhedron_3<K> Polyhedron;
typedef CGAL::Polyhedral_mesh_domain_3<Polyhedron, K> Mesh_domain;
// Concurrency tag
typedef CGAL::Sequential_tag Concurrency_tag;
// Triangulation
typedef CGAL::Mesh_triangulation_3<Mesh_domain,CGAL::Default,Concurrency_tag>::type Tr;
typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;
typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;

using namespace CGAL::parameters;

class Triangulator {
 public:
  static void Triangulate(std::string ply_file, std::string output_name) {
    CGAL_MESH mesh;
    std::ifstream input;
    input.open(ply_file);
    if (!CGAL::IO::read_PLY(input, mesh)) {
      exit(1);
    }

    Polyhedron polyhedron;

    CGAL::copy_face_graph(mesh, polyhedron);

    Mesh_domain domain(polyhedron);
    // Mesh criteria (no cell_size set)
    Mesh_criteria criteria(facet_angle=30, facet_size=10, facet_distance=1,
                           cell_radius_edge_ratio=2, cell_size=10);
    // Mesh generation
    C3t3 delauney = CGAL::make_mesh_3<C3t3>(domain, criteria, no_perturb(), no_exude(), manifold());

    CGAL::IO::output_to_tetgen(output_name, delauney, false, true);
  }


};

#endif //PROJECT__TRIANGULATOR_H_
