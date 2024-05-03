#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#define MAX_PITCH 89.0f
#define epsilon 0.1f

//// Fonction statique pour la gestion des contrôles de la caméra

// Pour les contrôles "fluides" se faisant en même temps que le rendu (ex : WASD/ZQSD, etc.)
static void processInput(GLFWwindow* window);

// Callback pour les input plus précis (Toggle un truc, etc.)
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Callback pour la position du cursor (ex : rotation de la caméra, etc.)
static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

class Camera
{
public: 

    void init();
    void update(float _deltaTime, GLFWwindow* _window);

    // ImGui (Faire mieux)
    void updateInterface(float _deltaTime);
    // ImGui onglet caméra
    void updateCameraSettingsInterface(float _deltaTime);
    // Réinitialiser la pos de la camera
    void resetCameraPos();
    // Appel les trois fonction de contrôles de la caméra
    // et bloque si conflit avec ImGui
    void allInputs(GLFWwindow* _window);
    void updateFreeInput(float _deltaTime, GLFWwindow* _window);
    void computeFinalView();

    glm::quat getRotation() const {return m_rotation;}
    glm::mat4 getViewMatrix() const {return m_viewMatrix;}
    glm::mat4 getProjectionMatrix() const {return m_projectionMatrix;}

    glm::vec3 pathReset();

    // Reference getters (À mieux faire)
    glm::vec3 & getPosition() {return m_position;}
    glm::vec3 & getEulerAngle() {return m_eulerAngle;}
    glm::vec3 & getFirstPositionReset() {return m_first_pos_reset;}
    glm::vec3 & getInitPosition() {return m_init_position;}

    bool & getDefaultMovementMode() {return m_defaultMovementMode;}
    bool & getImGuiFocus() {return m_imguifocus;}
    bool & getFirstInput() {return m_firstInput;}
    bool & getResetting() {return m_resetting;}
    bool & getFirstReset() {return m_firstReset;}

    float & getTranslationSpeed() {return m_translationSpeed;}
    float & getRotationSpeed() {return m_rotationSpeed;}
    float & getDeltaTime() {return m_deltaTime;}

    float & getLastX() {return m_lastX;}
    float & getLastY() {return m_lastY;}


private:

    //Camera parameters 
    float       m_fovDegree{ 45.0f };
    glm::vec3	m_position{ glm::vec3(0.f, 40.f, 0.f) };
    glm::vec3	m_eulerAngle{ glm::vec3(0.f, 0.f, 0.f) };
    glm::quat	m_rotation{};

    //View Projection
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    // Rotation and translation parameters
    float m_deltaTime;
    float m_translationSpeed = 1.0f;
    float m_rotationSpeed = 1.0f;

    // Mouse mouvements parameters
    bool m_firstInput = true;
    float m_lastX;
    float m_lastY;

    // Reset position parameters
    glm::vec3 m_init_position;
    glm::vec3 m_first_pos_reset;
    glm::vec3 m_path_reset;
    float m_init_time;
    float m_current_time;
    float m_duration = 1.0f;
    bool m_resetting = false;
    bool m_firstReset = true;

    // Others parameters
    bool m_defaultMovementMode = true;
    bool m_imguifocus = false;

};
