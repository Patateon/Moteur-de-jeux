#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
// #include <GL/glew.h>

#include <vector>
#include "mesh.hpp"

// Structure pour la gestion du mouvement avec les vecteurs positions, vitesse
// et accélération, ainsi que la masse. À modifier selon les besoins
struct Movement{
    float masse = 1.0f;

    glm::vec3 position = glm::vec3();
    glm::vec3 velocity = glm::vec3();
    glm::vec3 acceleration = glm::vec3();
};

//// Class Transform
// 3 vecteurs publiques pour la translation, la rotation et la mise à l'échelle.
// Angle d'eulers en entrée pour la rotation puis rotation par matrice dans
// l'ordre Pitch, Yaw, Roll en degrés.
// Pour calculer la matrice model local après avoir modifier les 3 vecteurs, getLocalModel()
// Pour calculer la matrice model dans le repère monde il faut passer la class
// Entity décrite plus loin et sa méthode updateSelfAndChild()
class Transform
{
public:
    // Local Transform
    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 eulerAngle = glm::vec3(0.0f, 0.0f, 0.0f); // Degrees
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    // Global Transfrom
    glm::mat4 model = glm::mat4(1.0f);

    // Compute the model matrix in the local space
    glm::mat4 getLocalModel();
};

//// Class Entity
// Stocke une entity de la scène et permet l'attribution d'enfant 
// ayant une position relatif à celle-ci (Scène graphe mais aussi pour de simples objets)
// Une entité se définit par une maillage, une transformation et une mouvement.
class Entity
{
public:
    Entity(){}
    // Scene graph
    std::vector<Entity*> children;
    Entity* parent = nullptr;

    // Global and local space informations
    Transform transform;

    // Mouvement de l'objet (À tester l'impact selon le repère local ou monde ?)
    Movement movement;

    // Maillage de l'objet
    Mesh* mesh;

    // Attribue un mmailge à une entity, possibilité de le modifier runtime
    // (L'appel à init est supposé ainsi que les appels à recomputeNormals et TexCoord)
    void setMesh(Mesh* mesh) {this->mesh = mesh;}

    // Renvoie le maillage par référence (Donc effet de bord donc équivalent Setter)
    Mesh* getMesh() {return this->mesh;}

    // Actualiser le mouvement, (Modifie la matrice model en conséquence)
    void move();

    // Ajoute un enfant relatif à l'objet
    void addChild(Entity * child);

    // Met à jour les matrices models global de chaque enfant de l'entité
    void updateSelfAndChild();
};

#endif