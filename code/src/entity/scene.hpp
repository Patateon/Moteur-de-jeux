#pragma once

// Standard libs
#include "src/entity/destructibleEntity.hpp"
#include <reactphysics3d/body/RigidBody.h>
#include <reactphysics3d/collision/Collider.h>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <vector>

// Personal libs
#include <src/entity/heightmap.hpp>
#include <src/camera//skybox.hpp>

// ReactPhysics3D
#include <reactphysics3d/reactphysics3d.h>

class Scene
{
private:
    HeightMap m_heightMap;
    //SkyBox m_skybox;
    std::vector<Entity> m_entities;
    std::vector<DestructibleEntity> m_destructibles;

    FractureGenerator m_fractureGenerator;

    reactphysics3d::PhysicsCommon m_physicsCommon;
    reactphysics3d::PhysicsWorld* m_world;
    std::vector<reactphysics3d::RigidBody*> m_bodies;
    std::vector<reactphysics3d::Collider*> m_colliders;
public:
    Scene();
    ~Scene();

    void init();
    //void initSkybox();
    void update(float _deltatime, const Camera& _camera, GLuint _matrixID, GLuint _modelMatrixID, GLuint _colorID, GLuint _hasTextureID);
    void clear();

    void setupTestScene();
};