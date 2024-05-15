
#include "glm/fwd.hpp"
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
#include <src/camera//skybox.hpp>

#include <string>

Scene::Scene(){
    m_fractureGenerator = FractureGenerator();
}

Scene::~Scene(){
    clear();
}

void Scene::init(){

    m_world = m_physicsCommon.createPhysicsWorld();
    // m_world->setGravity(reactphysics3d::Vector3(0.0, -9.81, 0.0));
    
    // Load every entities
    load();
    //initSkybox();
}

void Scene::load(){
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
    
    float current_time = glfwGetTime();

    /* if (current_time > 2.0f) {
        std::vector<DestructibleEntity*> listObject;
        m_fractureGenerator.Fracture(
            &m_destructibles[0], glm::vec2(0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f), listObject,
            m_world, &m_physicsCommon);

        m_destructibles[0].disable();
        for(uint i = 0; i < listObject.size(); i++){
            m_destructibles.push_back(*listObject[i]);
        }
    } */

    m_world->update(_deltatime);

    // Render
    m_heightMap.updateViewAndDraw(_camera, m_world, _matrixID, _modelMatrixID, _colorID, _hasTextureID);
    for(Entity & entity : m_entities){
        entity.updateViewAndDraw(_camera, m_world, _matrixID, _modelMatrixID, _colorID, _hasTextureID);
    }

    for(DestructibleEntity & dstEntity : m_destructibles){
        if (dstEntity.IsDestroyed()){
            dstEntity.checkIfAlive(_deltatime);
        }
        dstEntity.updateViewAndDraw(_camera, m_world, _matrixID, _modelMatrixID, _colorID, _hasTextureID);
    }

    //m_skybox.renderSkyBox(_camera,_matrixID);
}

int Scene::getIndexEntity(unsigned int id){
    for (uint i = 0; i < m_destructibles.size(); i++){
        if (id == m_destructibles[i].physicalEntity()->getEntity().id){
            return i;
        }
    }

    return -1;
}

int Scene::getIndexDestructible(unsigned int id){
    for (uint i = 0; i < m_destructibles.size(); i++){
        if (id == m_destructibles[i].physicalEntity()->getEntity().id){
            return i;
        }
    }

    return -1;
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
    m_heightMap = HeightMap(rec, 30, 30, "../assets/map/heightmap-1024x1024.png");
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
        m_entities[i].shouldRender(true);
    }

    DestructibleEntity test = DestructibleEntity(&m_fractureGenerator);

    test.LoadBasic(0.1f);
    test.movement().position = glm::vec3(0.0f, 40.f, 50.0f);
    // test.currentTransform().scale = glm::vec3(0.1f, 0.1f, 0.1f);
    test.updateSelfAndChild();
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
    m_destructibles[0].physicalEntity()->setType(BodyType::STATIC);


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
    glm::vec3 scale = m_destructibles[0].currentTransform().scale;
    Vector3 vScale = Vector3(scale.x, scale.y, scale.z);
    BoxShape* boxShape = m_physicsCommon.createBoxShape(Vector3(xLength, yLenght, m_destructibles[0].getScale()+0.1f) * vScale);
    
    Collider* testEcranCollider = m_destructibles[0].physicalEntity()->addCollider(boxShape, Transform::identity());

    for (uint i = 0; i < m_entities.size(); i++){
        m_entities[i].disable();
    }

    m_world->setEventListener(this);

    //// Initial forces settings
    m_entities[0].physicalEntity()->applyLocalForceAtCenterOfMass(Vector3(0.0, 0.0, 150.0));
}
/*
void Scene::initSkybox() {
    // Charger les faces de la skybox
    std::vector<std::string> faces {
        "../assets/skyboxes/Meadow/posx.jpg",
        "../assets/skyboxes/Meadow/negx.jpg",
        "../assets/skyboxes/Meadow/posy.jpg",
        "../assets/skyboxes/Meadow/negy.jpg",
        "../assets/skyboxes/Meadow/posz.jpg",
        "../assets/skyboxes/Meadow/negz.jpg"
    };

    // Charger la skybox
    if (!m_skybox.loadSkyBox(faces)) {
        // Gérer les erreurs si le chargement de la skybox échoue
        std::cerr << "Failed to load skybox." << std::endl;
    }
}*/

void Scene::onContact(const CollisionCallback::CallbackData& callbackData){
    // For each contact pair
    for (uint p = 0; p < callbackData.getNbContactPairs(); p++) {

        // Get the contact pair
        CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);
        rp3d::Body * body1 = contactPair.getBody1();
        rp3d::Body * body2 = contactPair.getBody2();

        int fstBody = 0;

        int indexDestructible = getIndexDestructible(body1->getEntity().id);
        if (indexDestructible < 0){
            indexDestructible = getIndexDestructible(body2->getEntity().id);

            if (indexDestructible < 0)
                break;
        }else{
            fstBody = 1;
        }

        if (contactPair.getEventType() == ContactPair::EventType::ContactStart){
            // For each contact point of the contact pair
            rp3d::Vector3 contactBarycentre = rp3d::Vector3::zero();
            for (uint c = 0; c < contactPair.getNbContactPoints(); c++) {
    
                // Get the contact point
                CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(c);
    
                // Get the contact point on the first collider and convert it in world-space
                rp3d::Vector3 worldPoint;
                if (fstBody){
                    worldPoint = contactPair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();
                }else{
                    worldPoint = contactPair.getCollider2()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider2();
                }
                
                contactBarycentre = worldPoint;
            }

            // contactBarycentre /= contactPair.getNbContactPoints();

            std::vector<DestructibleEntity*> listObjects;

            // fractureGenerator().seedRand();

            // Calculate hitpoint
            reactphysics3d::Vector3 contact = contactBarycentre - destructibles()[indexDestructible].physicalEntity()->getTransform().getPosition();
            // Adjust for object rotation
            reactphysics3d::Quaternion orientation = destructibles()[indexDestructible].physicalEntity()->getTransform().getOrientation();
            glm::vec3 glmContact = glm::vec3(contact.x, contact.y, contact.z) *
                glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);
            
            if (fractureGenerator().Fracture(&(destructibles()[indexDestructible]),
                glm::vec2(glmContact.x, glmContact.y), glm::vec3(0.0f, 0.0f, 0.0f), listObjects,
                world(), physicsCommon())){
                destructibles()[indexDestructible].disable();
                for(uint i = 0; i < listObjects.size(); i++){
                    destructibles().push_back(*listObjects[i]);
                }
            }
        }      
    }
}