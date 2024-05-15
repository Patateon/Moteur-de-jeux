#pragma once

// Standard libs
#include <deque>
#include <reactphysics3d/engine/EventListener.h>
#include <reactphysics3d/engine/PhysicsWorld.h>
#include <vector>

// Personal libs
#include <src/camera/skybox.hpp>
#include <src/entity/heightmap.hpp>
#include <src/entity/destructibleEntity.hpp>

// ReactPhysics3D
#include <reactphysics3d/reactphysics3d.h>
#include <reactphysics3d/body/RigidBody.h>
#include <reactphysics3d/collision/Collider.h>
#include <reactphysics3d/engine/PhysicsCommon.h>

class Scene : reactphysics3d::EventListener
{
private:
    HeightMap m_heightMap;
    //SkyBox m_skybox;
    std::deque<Entity> m_entities;
    std::deque<DestructibleEntity> m_destructibles;

    FractureGenerator m_fractureGenerator;

    reactphysics3d::PhysicsCommon m_physicsCommon;
    reactphysics3d::PhysicsWorld* m_world;
    std::vector<reactphysics3d::RigidBody*> m_bodies;
    std::vector<reactphysics3d::Collider*> m_colliders;
public:
    Scene();
    ~Scene();

    void init();
    void load();
    //void initSkybox();
    void update(float _deltatime, const Camera& _camera, GLuint _matrixID, GLuint _modelMatrixID, GLuint _colorID, GLuint _hasTextureID);
    void clear();

    void setupTestScene();

    int getIndexEntity(unsigned int id);
    int getIndexDestructible(unsigned int id);

    virtual void onContact(const CollisionCallback::CallbackData& callbackData) override;
    // virtual void onTrigger(const CollisionCallback::CallbackData& callbackData) override {}

    // Getter
    reactphysics3d::PhysicsCommon * physicsCommon() { return &m_physicsCommon; }
    reactphysics3d::PhysicsWorld * world() { return m_world; }

    std::deque<Entity> & entities() { return m_entities; }
    const std::deque<Entity> & entities() const { return m_entities; }

    std::deque<DestructibleEntity> & destructibles() { return m_destructibles; }
    const std::deque<DestructibleEntity> & destructibles() const { return m_destructibles; }

    FractureGenerator & fractureGenerator() { return m_fractureGenerator; }
    const FractureGenerator & fractureGenerator() const { return m_fractureGenerator; }
};