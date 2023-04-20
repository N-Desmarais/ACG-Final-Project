//
// Created by njdesmarais on 4/18/23.
//

#ifndef PROJECT_FRACTURE_H
#define PROJECT_FRACTURE_H

#include <vector>

#include "vectors.h"
#include "meshdata.h"
#include "boundingbox.h"

class fractureElement;

// pretty much the clothNode class from hw2 + tetrahedral data
class fractureNode {
public:
    // Accessors
    const Vec3f& getOriginalPosition() const{ return original_position; }
    const Vec3f& getPosition() const{ return position; }
    const Vec3f& getVelocity() const{ return velocity; }
    const Vec3f& getAcceleration() const { return acceleration; }
    const Vec3f& getCollisionForce() const { return collision_force; }
    double getMass() const { return mass; }
    std::vector<fractureElement *> getElements() { return adjacent_elements; }
    // Modifiers
    void setOriginalPosition(const Vec3f &p) { original_position = p; }
    void setPosition(const Vec3f &p) { position = p; }
    void setVelocity(const Vec3f &v) { velocity = v; }
    void setAcceleration(const Vec3f &a) { acceleration = a; }
    void setCollisionForce(const Vec3f &f) { collision_force = f; }
    void setMass(double m) { mass = m; }
    void addElement(fractureElement * element) { adjacent_elements.push_back(element); }

    bool checkElement(fractureElement * element) {
        for (uint32_t i = 0; i < adjacent_elements.size(); i++) {
            if (element == adjacent_elements[i]) {
                return true;
            }
        }
    }
private:
    // Rep
    Vec3f original_position;
    Vec3f position;
    Vec3f velocity;
    Vec3f acceleration;
    Vec3f collision_force;
    std::vector<fractureElement *> adjacent_elements;
    double mass;
};

class fractureElement {
public:
    fractureElement(fractureNode * p1,fractureNode * p2,fractureNode * p3,fractureNode * p4) {
        nodes = std::vector<fractureNode *> {p1,p2,p3,p4};
    }
    // Accessors
    fractureNode * operator[](unsigned int i) {
        assert(i < 4);
        return nodes[i];
    }
    // Volume calculation taken from O'Brien
    double computeVolume() {
        fractureNode n1 = *nodes[0],
                     n2 = *nodes[1],
                     n3 = *nodes[2],
                     n4 = *nodes[3];

        Vec3f diff1 = n2.getPosition() - n1.getPosition();
        Vec3f diff2 = n3.getPosition() - n1.getPosition();
        Vec3f diff3 = n4.getPosition() - n1.getPosition();

        return (diff1 * diff2).Dot3(diff3) / 6;
    }
    //Find if a point is on the same side as the remaining vertex of a tetrahedron plane
    bool SameSide(Vec3f v1, Vec3f v2, Vec3f v3, Vec3f v4, Vec3f p)
    {
        Vec3f normal;
        Vec3f::Cross3(normal, v2 - v1, v3 - v1);
        normal.Normalize();
        double dotV4 = normal.Dot3(v4 - v1);
        double dotP = normal.Dot3(p - v1);
        return (dotV4 * dotP) > 0;
    }
    //Check if a point is intersection the fracture node(tetrahedron)
    bool Intersection(Vec3f p) {
        Vec3f v1 = nodes[0]->getPosition();
        Vec3f v2 = nodes[1]->getPosition();
        Vec3f v3 = nodes[2]->getPosition();
        Vec3f v4 = nodes[3]->getPosition();

        return SameSide(v1, v2, v3, v4, p) &&
            SameSide(v2, v3, v4, v1, p) &&
            SameSide(v3, v4, v1, v2, p) &&
            SameSide(v4, v1, v2, v3, p);
    }

    double planeDistance(Vec3f v1, Vec3f v2, Vec3f v3, Vec3f p, Vec3f &direction) {
        Vec3f normal;
        Vec3f::Cross3(normal, v2 - v1, v3 - v1);
        normal.Normalize();
        direction = normal;
        return normal.Dot3(p - v1);
    }

    double penetrationDepth(Vec3f p, Vec3f &direction) {
        Vec3f v1 = nodes[0]->getPosition();
        Vec3f v2 = nodes[1]->getPosition();
        Vec3f v3 = nodes[2]->getPosition();
        Vec3f v4 = nodes[3]->getPosition();
        Vec3f potential_direction;
        double min_dist = planeDistance(v1, v2, v3, p, potential_direction);
        direction = potential_direction;
        double check = planeDistance(v2, v3, v4, p, potential_direction);
        if (check < min_dist) {
            min_dist = check;
            direction = potential_direction;;
        }
        check = planeDistance(v3, v4, v1, p, potential_direction);
        if (check < min_dist) {
            min_dist = check;
            direction = potential_direction;;
        }
        check = planeDistance(v4, v1, v2, p, potential_direction);
        if (check < min_dist) {
            min_dist = check;
            direction = potential_direction;;
        }
        direction *= -1.0f;
        return min_dist;
    }

private:
    std::vector<fractureNode *> nodes;
};

class fractureMesh {
 public:
    fractureMesh(MeshData * data);

    void animate();
    void packMesh(MeshData *data);
    void computeForces();

    // public so it can be seen by renderer
    std::unique_ptr<float> tri_data;
    uint32_t numTriangles;
 private:
    // animation constants
    float timestep;
    // FEM representation
    std::vector<fractureNode> nodes;
    std::vector<fractureElement> elements;
    // Mesh representation
    uint32_t numNodes;
    uint32_t numElements;
    BoundingBox bbox;
};

#endif //PROJECT_FRACTURE_H
