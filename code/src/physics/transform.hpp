#pragma once

// GLM library
#include <glm/glm.hpp>

#include <src/common/mesh.hpp>

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
