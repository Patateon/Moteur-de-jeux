#pragma once

// Standard lib
#include <vector>

// Others
#include <common/transform.hpp>
#include <common/mesh.hpp>


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
    Transform m_transform;

    // Mouvement de l'objet (À tester l'impact selon le repère local ou monde ?)
    Movement m_movement;

    // Maillage de l'objet
    Mesh* m_currentMesh;
public:
    Entity(){}
    Entity(Mesh* mesh);

    // Attribue un mmailge à une entity, possibilité de le modifier runtime
    // (L'appel à init est supposé ainsi que les appels à recomputeNormals et TexCoord)
    void setMesh(Mesh* mesh) {this->m_currentMesh = mesh;}

    // Renvoie le maillage par référence (Donc effet de bord donc équivalent Setter)
    Mesh* getMesh() {return this->m_currentMesh;}

    // Actualiser le mouvement, (Modifie la matrice model en conséquence)
    void move();

    // Ajoute un enfant relatif à l'objet
    void addChild(Entity * child);

    // Met à jour les matrices models global de chaque enfant de l'entité
    void updateSelfAndChild();

    // Rendering function overload
    void init();
    void render();
    void clear();
};
