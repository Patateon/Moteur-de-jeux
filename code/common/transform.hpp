#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
// #include <GL/glew.h>

#include <vector>
#include "mesh.hpp"

// Class Transform

struct Movement{
    float masse = 1.0f;

    glm::vec3 position = glm::vec3();
    glm::vec3 velocity = glm::vec3();
    glm::vec3 acceleration = glm::vec3();
};

class Transform
{
public:
    // Local Transform
    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 eulerAngle = glm::vec3(0.0f, 0.0f, 0.0f); // Degrees
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    // Global Transfrom
    glm::mat4 model = glm::mat4(1.0f);

    // Compute the model matrix in the local space
    glm::mat4 getLocalModel();
};

class Node
{
public:
    Node(){}
    // Scene graph
    std::vector<Node*> children;
    Node* parent = nullptr;

    // Global and local space informations
    Transform transform;

    Movement movement;

    // Objects informations
    Mesh * mesh;

    void setMesh(Mesh * mesh) {this->mesh = mesh;}
    Mesh * getMesh() {return this->mesh;}

    void move();

    void addChild(Node * child);

    void updateSelfAndChild();
};

#endif