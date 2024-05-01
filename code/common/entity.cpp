
// Others
#include "main/Actor/ObjController.hpp"
#include <common/transform.hpp>
#include <common/entity.hpp>

Entity::Entity(Mesh mesh){
    m_currentMesh = mesh;
}

void Entity::move(){
    this->m_transform.translation = this->m_movement.position;
    this->updateSelfAndChild();
}


void Entity::addChild(Entity * child){
    m_children.emplace_back(child);
    m_children.back()->m_parent = this;
}


void Entity::updateSelfAndChild()
{
    if (m_parent)
        m_transform.model = m_parent->m_transform.model * 
            m_transform.getLocalModel();
    else
        m_transform.model = m_transform.getLocalModel();

    for (auto&& child : m_children)
    {
        child->updateSelfAndChild();
    }
}

void Entity::loadEntity(){
    currentMesh().init();
}

void Entity::updateViewAndDraw(const Camera& _camera, GLuint _matrixID, GLuint _modelMatrixID, GLuint _colorID, GLuint _hasTextureID){
    glm::mat4 projectionMatrix = _camera.getProjectionMatrix();
    glm::mat4 viewMatrix = _camera.getViewMatrix();
    glm::mat4 modelMatrix = m_transform.getLocalModel();
    glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(_modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    currentMesh().render(_colorID, _hasTextureID);
}

void Entity::clear(){
    m_children.clear();
    currentMesh().clear();
}
