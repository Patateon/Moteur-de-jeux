#pragma once

#include "src/entity/scene.hpp"
#include "src/camera/camera.hpp"

struct Context
{
    Scene* scene;
    Camera* camera;
};

//// Fonction statique pour la gestion des contrôles de la caméra

// Pour les contrôles "fluides" se faisant en même temps que le rendu (ex : WASD/ZQSD, etc.)
void processInput(GLFWwindow* window);

// Callback pour les inputs souris
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

// Callback pour les input plus précis (Toggle un truc, etc.)
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Callback pour la position du cursor (ex : rotation de la caméra, etc.)
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
