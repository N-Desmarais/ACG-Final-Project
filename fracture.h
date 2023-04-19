//
// Created by njdesmarais on 4/18/23.
//

#ifndef PROJECT_FRACTURE_H
#define PROJECT_FRACTURE_H

#include <vector>

#include "vectors.h"
#include "meshdata.h"

class fractureElement;

// pretty much the clothNode class from hw2 + tetrahedral data
class fractureNode {
public:
    // Accessors
    const Vec3f& getOriginalPosition() const{ return original_position; }
    const Vec3f& getPosition() const{ return position; }
    const Vec3f& getVelocity() const{ return velocity; }
    const Vec3f& getAcceleration() const { return acceleration; }
    double getMass() const { return mass; }
    std::vector<fractureElement *> getElements() { return adjacent_elements; }
    // Modifiers
    void setOriginalPosition(const Vec3f &p) { original_position = p; }
    void setPosition(const Vec3f &p) { position = p; }
    void setVelocity(const Vec3f &v) { velocity = v; }
    void setAcceleration(const Vec3f &a) { acceleration = a; }
    void setMass(double m) { mass = m; }
    void addElement(fractureElement * element) { adjacent_elements.push_back(element); }
private:
    // Rep
    Vec3f original_position;
    Vec3f position;
    Vec3f velocity;
    Vec3f acceleration;
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
private:
    std::vector<fractureNode *> nodes;
};

class fractureMesh {
    fractureMesh(MeshData * data, float timestep);
    // animation constants
    float timestep;
    // FEM representation
    std::vector<fractureNode> nodes;
    std::vector<fractureElement> elements;
    // Mesh representation
    uint32_t numNodes;
    uint32_t numElements;
    uint32_t numTriangles;

    void animate();
    void packMesh();


};

#endif //PROJECT_FRACTURE_H