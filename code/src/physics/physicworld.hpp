#pragma once

// Standard libraries
#include <vector>

// ReactPhysics3D
#include <reactphysics3d/reactphysics3d.h>

// Personnal libraries
#include "src/entity/entity.hpp"


class PhysicWorld
{
private:
    std::vector<Entity*> m_entities;

public:
    PhysicWorld();
    ~PhysicWorld();

    void test();

    void addEntity();
    void deleteEntity();

    void initWorld();

    void updateWorld();

    void clearWorld();

    // Toute une tripotée de méthodes
};