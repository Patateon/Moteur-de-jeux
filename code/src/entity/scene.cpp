#include "src/entity/entity.hpp"
#include <iostream>
#include <src/entity/scene.hpp>
#include <src/entity/heightmap.hpp>

#include <string>

Scene::Scene(){}

Scene::~Scene(){
    clear();
}

void Scene::init(){

    m_world = m_physicsCommon.createPhysicsWorld();

    // Load every entities
    m_heightMap.loadEntity();
    for(Entity & entity : m_entities){
        entity.loadEntity();
    }
}

void Scene::update(float _deltatime, const Camera& _camera, GLuint _matrixID, GLuint _modelMatrixID, GLuint _colorID, GLuint _hasTextureID){
    // Update physics
    /* using namespace reactphysics3d;
    m_world->update(_deltatime);
    const Transform& sphereTransform = sphereBody->getTransform();
    const Vector3& spherePosition = sphereTransform.getPosition();
    sphere.movement().position = glm::vec3(spherePosition.x, spherePosition.y, spherePosition.z);
    sphere.move();
    std::cout<<spherePosition.x<<", "<<spherePosition.y<<", "<<spherePosition.z<<std::endl; */
    
    

    // Render
    m_heightMap.updateViewAndDraw(_camera, _matrixID, _modelMatrixID, _colorID, _hasTextureID);
    for(Entity & entity : m_entities){
        entity.updateViewAndDraw(_camera, _matrixID, _modelMatrixID, _colorID, _hasTextureID);
    }

}

void Scene::clear(){
    m_heightMap.clear();
    for(Entity & entity : m_entities){
        entity.clear();
    }

    // m_physicsCommon.destroyPhysicsWorld(m_world);
}

void Scene::setupTestScene(){

    // Height Map
    Rectangle rec;
    rec.bottomLeft = glm::vec3(-75.0f, 0.0, -75.0f);
    rec.right = glm::vec3(150.0f, 0.0f, 0.0f);
    rec.up = glm::vec3(0.0f, 0.0f, 150.0f);
    m_heightMap = HeightMap(rec, 30, 30, "../assets/map/heightmap-1024x1024.png");
    m_heightMap.build(30, 30);
    m_heightMap.currentMesh().hasTexture(false);
    m_heightMap.currentMesh().color(glm::vec3(0.91f, 0.91f, 0.91f));
    // m_heightMap.currentMesh().texture("../assets/map/grass.png");


    // Simple sphere entity
    Entity sphere = Entity("../assets/entities/sphere.off");
    sphere.currentMesh().recomputeNormalsAndTexCoords();
    sphere.currentMesh().hasTexture(false);
    sphere.currentMesh().color(glm::vec3(0.6f, 0.1f, 0.1f));
    sphere.movement().position = glm::vec3(0.0f, 40.f, 5.0f);
    sphere.move();
    m_entities.push_back(sphere);

    sphere.currentMesh().hasTexture(true);
    sphere.currentMesh().texture("../assets/map/rock.png");
    sphere.movement().position = glm::vec3(0.0f, 30.f, 5.0f);
    sphere.move();
    m_entities.push_back(sphere);

    sphere.currentMesh().hasTexture(false);
    sphere.movement().position = glm::vec3(0.0f, 20.f, 5.0f);
    sphere.move();
    m_entities.push_back(sphere);

    sphere.currentMesh().hasTexture(true);
    sphere.movement().position = glm::vec3(0.0f, 10.f, 5.0f);
    sphere.move();
    m_entities.push_back(sphere);

    // using namespace reactphysics3d;
    // Vector3 spherePosition = Vector3(sphere.movement().position);
    // // Vector3 spherePosition = Vector3(0.0, 40.0, 5.0);
    // Quaternion sphereOrientation = Quaternion::identity();
    // Transform sphereTransform(spherePosition, sphereOrientation);
    // RigidBody* sphereBody = m_world->createRigidBody(sphereTransform);
}