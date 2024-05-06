#pragma once

// Standard lib
#include "src/actor/objcontroller.hpp"
#include <reactphysics3d/body/RigidBody.h>
#include <reactphysics3d/engine/Entity.h>
#include <reactphysics3d/engine/PhysicsWorld.h>
#include <vector>

// ReactPhysics3D
#include <reactphysics3d/reactphysics3d.h>

// Others
#include <src/common/mesh.hpp>
#include <src/physics/transform.hpp>
#include <src/camera/camera.hpp>


//// Class Entity
// Stocke une entity de la scène et permet l'attribution d'enfant 
// ayant une position relatif à celle-ci (Scène graphe mais aussi pour de simples objets)
// Une entité se définit par une maillage, une transformation et une mouvement.
class Entity
{
private:
    // Scene graph
    std::vector<Entity*> m_children;
    Entity* m_parent = nullptr;

    // Global and local space informations
    transform::Transform m_currentTransform;

    // Mouvement de l'objet (À tester l'impact selon le repère local ou monde ?)
    Movement m_movement;

    // Maillage de l'objet
    Mesh m_currentMesh;

    reactphysics3d::RigidBody* m_physicalEntity;

public:
    Entity(){}
    Entity(Mesh mesh);
    Entity(const std::string & filename);
    Entity(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices);

    Mesh & currentMesh() { return m_currentMesh; }
    const Mesh & currentMesh() const { return m_currentMesh; }

    Movement & movement() { return m_movement; }

    transform::Transform & currentTransform() { return m_currentTransform; }

    reactphysics3d::RigidBody* physicalEntity() { return m_physicalEntity; } 

    // Actualiser le mouvement, (Modifie la matrice model en conséquence)
    void move();

    // Ajoute un enfant relatif à l'objet
    void addChild(Entity * child);

    // Met à jour les matrices models global de chaque enfant de l'entité
    void updateSelfAndChild();

    // Physics init and update function
    void initBody(reactphysics3d::PhysicsWorld* _world);

    // Rendering function overload
    void loadEntity(reactphysics3d::PhysicsWorld* _world);
    void updateViewAndDraw(const Camera& _camera, 
        reactphysics3d::PhysicsWorld* _world,
        GLuint _matrixID, GLuint _modelMatrixID, 
        GLuint _colorID, GLuint _hasTextureID);
    void clear();
};
