
// Others
#include "glm/detail/type_vec.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <reactphysics3d/engine/PhysicsWorld.h>
#include <reactphysics3d/mathematics/Quaternion.h>
#include <reactphysics3d/mathematics/Transform.h>
#include <reactphysics3d/mathematics/Vector3.h>
#include <src/actor/objcontroller.hpp>
#include <src/physics/transform.hpp>
#include <src/entity/entity.hpp>
#include <vector>
//#include <reactphysics3d/reactphysics3d.h> 


Entity::Entity(Mesh mesh){
    m_currentMesh = mesh;
}

Entity::Entity(const std::string & filename){
    m_currentMesh = Mesh(filename);
}

Entity::Entity(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices){
    m_currentMesh = Mesh(vertices, indices);
}

void Entity::move(){
    this->m_currentTransform.translation = this->m_movement.position;
    this->updateSelfAndChild();
}


void Entity::addChild(Entity * child){
    m_children.emplace_back(child);
    m_children.back()->m_parent = this;
}


void Entity::updateSelfAndChild()
{
    if (m_parent)
        m_currentTransform.model = m_parent->m_currentTransform.model * 
            m_currentTransform.getLocalModel();
    else
        m_currentTransform.model = m_currentTransform.getLocalModel();

    for (auto&& child : m_children)
    {
        child->updateSelfAndChild();
    }
}

void Entity::initBody(reactphysics3d::PhysicsWorld* _world){
    glm::vec3 position = currentTransform().translation;
    glm::quat orientation = glm::quat(currentTransform().eulerAngle);
    reactphysics3d::Quaternion rOrientation = 
    reactphysics3d::Quaternion(
        orientation.x,
        orientation.y,
        orientation.z,
        orientation.w);

    reactphysics3d::Transform transform = reactphysics3d::Transform(
        reactphysics3d::Vector3(position.x, position.y , position.z),
        rOrientation
    );

    m_physicalEntity = _world->createRigidBody(transform);
}

void Entity::loadEntity(reactphysics3d::PhysicsWorld* _world){
    currentMesh().init();
    initBody(_world);
}

void Entity::updateViewAndDraw(const Camera& _camera, reactphysics3d::PhysicsWorld* _world, GLuint _matrixID, GLuint _modelMatrixID, GLuint _colorID, GLuint _hasTextureID){
    glm::mat4 projectionMatrix = _camera.getProjectionMatrix();
    glm::mat4 viewMatrix = _camera.getViewMatrix();
    const reactphysics3d::Transform transform = m_physicalEntity->getTransform();
    float tmpModelMatrix[16];
    transform.getOpenGLMatrix(tmpModelMatrix);
    glm::mat4 modelMatrix = glm::make_mat4(tmpModelMatrix);
    modelMatrix = modelMatrix * glm::scale(glm::mat4(), currentTransform().scale);

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
