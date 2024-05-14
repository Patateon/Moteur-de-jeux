#include <reactphysics3d/collision/shapes/BoxShape.h>
#include <src/entity/destructibleEntity.hpp>
#include <src/entity/fractureGenerator.hpp>

#include <reactphysics3d/body/RigidBody.h>
#include <reactphysics3d/collision/Collider.h>
#include <reactphysics3d/collision/CollisionCallback.h>
#include <reactphysics3d/collision/HeightField.h>
#include <reactphysics3d/collision/shapes/AABB.h>
#include <reactphysics3d/collision/shapes/HeightFieldShape.h>
#include <reactphysics3d/collision/shapes/SphereShape.h>
#include <reactphysics3d/components/RigidBodyComponents.h>
#include <reactphysics3d/engine/Entity.h>
#include <reactphysics3d/mathematics/Quaternion.h>
#include <reactphysics3d/mathematics/Transform.h>
#include <reactphysics3d/mathematics/Vector3.h>
#include <reactphysics3d/utils/DebugRenderer.h>
#include <reactphysics3d/utils/Message.h>
#include <src/entity/scene.hpp>
#include <src/entity/entity.hpp>
#include <src/entity/heightmap.hpp>

#include <string>
#include <vector>

Scene::Scene(){}

Scene::~Scene(){
    clear();
}

void Scene::init(){

    m_world = m_physicsCommon.createPhysicsWorld();
    m_world->setGravity(reactphysics3d::Vector3(0.0, -9.81, 0.0));
    
    // Load every entities
    m_heightMap.loadEntity(m_world);
    for(Entity & entity : m_entities){
        entity.loadEntity(m_world);
    }

    for(DestructibleEntity & dstEntity : m_destructibles){
        dstEntity.loadEntity(m_world);
    }
}

void Scene::update(float _deltatime, const Camera& _camera, GLuint _matrixID, GLuint _modelMatrixID, GLuint _colorID, GLuint _hasTextureID){
    // Update physics
    
    m_world->update(_deltatime);

    // Render
    m_heightMap.updateViewAndDraw(_camera, m_world, _matrixID, _modelMatrixID, _colorID, _hasTextureID);
    for(Entity & entity : m_entities){
        entity.updateViewAndDraw(_camera, m_world, _matrixID, _modelMatrixID, _colorID, _hasTextureID);
    }

    for(DestructibleEntity & dstEntity : m_destructibles){
        dstEntity.updateViewAndDraw(_camera, m_world, _matrixID, _modelMatrixID, _colorID, _hasTextureID);
    }

}

void Scene::clear(){
    m_heightMap.clear();
    for(Entity & entity : m_entities){
        entity.clear();
    }

    for(DestructibleEntity & dstEntity : m_destructibles){
        dstEntity.clear();
    }

    m_physicsCommon.destroyPhysicsWorld(m_world);
}

void Scene::setupTestScene(){

    //// Setup for entities of the scene

    // Height Map
    Rectangle rec;
    rec.bottomLeft = glm::vec3(-75.0f, 0.0, -75.0f);
    rec.right = glm::vec3(150.0f, 0.0f, 0.0f);
    rec.up = glm::vec3(0.0f, 0.0f, 150.0f);
    m_heightMap = HeightMap(rec, 30, 30, "../assets/map/default-heightmap-1024x1024.png");
    m_heightMap.maxHeight(40.f);
    m_heightMap.build(30, 30);
    m_heightMap.currentMesh().hasTexture(false);
    m_heightMap.currentMesh().color(glm::vec3(0.30f, 0.30f, 0.30f));
    m_heightMap.currentMesh().texture("../assets/map/rock.png");

    // Simple sphere entity
    Entity sphere = Entity("../assets/entities/sphere.off");
    sphere.currentMesh().recomputeNormalsAndTexCoords();
    sphere.currentMesh().texture("../assets/map/rock.png");
    sphere.currentMesh().hasTexture(true);
    sphere.currentMesh().color(glm::vec3(0.6f, 0.1f, 0.1f));
    sphere.movement().position = glm::vec3(0.0f, 40.f, 5.1f);
    sphere.move();
    m_entities.push_back(sphere);

    sphere.currentMesh().hasTexture(true);
    sphere.movement().position = glm::vec3(0.0f, 30.f, 5.0f);
    sphere.move();
    m_entities.push_back(sphere);

    sphere.currentMesh().hasTexture(true);
    sphere.movement().position = glm::vec3(0.0f, 20.f, 5.1f);
    sphere.move();
    m_entities.push_back(sphere);

    sphere.currentMesh().hasTexture(true);
    sphere.movement().position = glm::vec3(0.0f, 10.f, 5.0f);
    sphere.move();
    m_entities.push_back(sphere);

    m_heightMap.shouldRender(true);
    for (uint i = 0; i < m_entities.size(); i++){
        m_entities[i].shouldRender(false);
    }

    FractureGenerator frtGen = FractureGenerator();
    DestructibleEntity test = DestructibleEntity(&frtGen);

    test.LoadBasic(2.0f);
    test.movement().position = glm::vec3(0.0f, 40.f, 10.0f);
    test.move();
    test.currentMesh().color(glm::vec3(1.0f, 0.0f, 0.0f));
    test.currentMesh().hasTexture(false);
    test.shouldRender(true);
    m_destructibles.push_back(test);

    //// Init each entities and give the possibilities to give options for physics
    init();

    //// Physicals bodies settings
    using namespace reactphysics3d;

    // Height map not affected by physics
    m_heightMap.physicalEntity()->setType(BodyType::STATIC);
    // m_destructibles[0].physicalEntity()->setType(BodyType::STATIC);


    Collider* heightMapCollider = m_heightMap.createCollider(&m_physicsCommon);

    // Describes colliders of each entities
    for (auto& entity : m_entities) {
        float radius = 0.5f;
        SphereShape* sphereShape = m_physicsCommon.createSphereShape(radius);
        Collider* entityCollider = entity.physicalEntity()->addCollider(sphereShape, Transform::identity());
    }

    m_destructibles[0].CalculateMinMax();
    glm::vec2 min = m_destructibles[0].GetMin();
    glm::vec2 max = m_destructibles[0].GetMax();
    float xLength = (max.x - min.x) / 2.0f;
    float yLenght = (max.y - min.y) / 2.0f;
    BoxShape* boxShape = m_physicsCommon.createBoxShape(Vector3(xLength, yLenght, m_destructibles[0].getScale()+0.1f));

    Collider* testEcranCollider = m_destructibles[0].physicalEntity()->addCollider(boxShape, Transform::identity());

    std::vector<DestructibleEntity*> listObject;
    frtGen.Fracture(&m_destructibles[0], glm::vec2(0.3f, 0.6f), glm::vec3(0.0f, 0.0f, 1.0f), listObject, m_world, &m_physicsCommon);

    for(uint i = 0; i < listObject.size(); i++){
        m_destructibles.push_back(*listObject[i]);
    }

    //// Initial forces settings
    m_entities[0].physicalEntity()->applyLocalForceAtCenterOfMass(Vector3(0.0, 0.0, 150.0));
}