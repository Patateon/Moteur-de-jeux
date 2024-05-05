#pragma once

// Standard libs
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <vector>

// Personal libs
#include <src/entity/heightmap.hpp>

// ReactPhysics3D
#include <reactphysics3d/reactphysics3d.h>

class Scene
{
private:
    HeightMap m_heightMap;
    std::vector<Entity> m_entities;

    reactphysics3d::PhysicsCommon m_physicsCommon;
    reactphysics3d::PhysicsWorld* m_world;
public:
    Scene();
    ~Scene();

    void init();
    void update(float _deltatime, const Camera& _camera, GLuint _matrixID, GLuint _modelMatrixID, GLuint _colorID, GLuint _hasTextureID);
    void clear();

    void setupTestScene();
};