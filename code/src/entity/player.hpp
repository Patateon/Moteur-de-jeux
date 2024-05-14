#pragma once

#include <src/entity/entity.hpp>
#include <src/camera/camera.hpp>
#include <src/entity/scene.hpp>

static void keyUserCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

class Player : public Entity
{
private:
    Scene* m_scene;
    Camera* m_camera;
public:
    Player();
    Player(Scene * scene, Camera * camera);
    ~Player();

    void update();
    void updateInput();
};