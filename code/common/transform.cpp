
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>

// Include GLM
#include <glm/gtc/matrix_transform.hpp>

// Others
#include "transform.hpp"


/*===============
Transform methods
===============*/

glm::mat4 Transform::getLocalModel(){

    glm::mat4 rotationX = glm::rotate(
        glm::mat4(1.0f),
        glm::radians(eulerAngle.x),
        glm::vec3(1.0f, 0.0f, 0.0f)
    );
    glm::mat4 rotationY = glm::rotate(
        glm::mat4(1.0f),
        glm::radians(eulerAngle.y),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    glm::mat4 rotationZ = glm::rotate(
        glm::mat4(1.0f),
        glm::radians(eulerAngle.z),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    glm::mat4 rotation = rotationX * rotationY * rotationZ;

    // Scale -> Rotate -> Translate
    return glm::translate(glm::mat4(1.0f), translation) *
        rotation * glm::scale(glm::mat4(1.0f), scale);
}


/*============
Entity methods
============*/

void Entity::move(){
    this->transform.translation = this->movement.position;
    this->updateSelfAndChild();
}


void Entity::addChild(Entity * child){
    children.emplace_back(child);
    children.back()->parent = this;
}


void Entity::updateSelfAndChild()
{
    if (parent)
        transform.model = parent->transform.model * 
            transform.getLocalModel();
    else
        transform.model = transform.getLocalModel();

    for (auto&& child : children)
    {
        child->updateSelfAndChild();
    }
}
