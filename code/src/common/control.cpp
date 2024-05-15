
#include <X11/X.h>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <reactphysics3d/engine/PhysicsWorld.h>
#include <reactphysics3d/mathematics/Vector3.h>
#include <src/common/control.hpp>
#include <src/camera/camerahelper.hpp>

// Include GLM
#include <glm/glm.hpp>

// Include IMGui
#include <external/imgui/imgui.h>
#include <external/imgui/imgui_impl_glfw.h>
#include <external/imgui/imgui_impl_opengl3.h>


using namespace glm;


void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    void *data = glfwGetWindowUserPointer(window);
    Context * context = static_cast<Context *> (data);
    Camera * camera = context->camera;
    Scene * scene = context->scene;

    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

    if(!camera->getImGuiFocus()){
        if(camera->getFirstInput()){
            camera->getLastX() = xpos;
            camera->getLastY() = ypos;
            camera->getFirstInput() = false;
        }

        float xOffset = xpos - camera->getLastX();
        float yOffset = ypos - camera->getLastY();
        camera->getLastX() = xpos;
        camera->getLastY() = ypos;

        xOffset *= camera->getRotationSpeed();
        yOffset *= camera->getRotationSpeed();

        camera->getEulerAngle().x += yOffset;
        camera->getEulerAngle().y -= xOffset;

        if(camera->getEulerAngle().x > MAX_PITCH)
            camera->getEulerAngle().x = MAX_PITCH;
        if(camera->getEulerAngle().x < -MAX_PITCH)
            camera->getEulerAngle().x = -MAX_PITCH;
    }else{
        camera->getFirstInput() = true;
    }
}


void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){

    void *data = glfwGetWindowUserPointer(window);
    Context * context = static_cast<Context *> (data);
    Camera * camera = context->camera;
    Scene * scene = context->scene;

    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

    vec3 front = normalize(camera->getRotation() * VEC_FRONT);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        Entity sphere = Entity("../assets/entities/sphere.off");
        sphere.currentMesh().recomputeNormalsAndTexCoords();
        sphere.currentMesh().hasTexture(false);
        sphere.currentMesh().color(glm::vec3(0.6f, 0.1f, 0.1f));
        sphere.movement().position = camera->getPosition() + front;
        sphere.move();
        sphere.shouldRender(true);
        using namespace reactphysics3d;

        PhysicsCommon * physicalCommon = scene->physicsCommon();
        PhysicsWorld * world = scene->world();

        scene->entities().push_back(sphere);
        scene->entities().back().loadEntity(world);
        
        float radius = 0.5f;
        float strength = 5000.f;
        glm::vec3 weightedFront = strength * front;
        SphereShape* sphereShape = physicalCommon->createSphereShape(radius);
        Collider* entityCollider = scene->entities().back().physicalEntity()->addCollider(sphereShape, Transform::identity());
        scene->entities().back().physicalEntity()->applyLocalForceAtCenterOfMass(Vector3(weightedFront.x, weightedFront.y, weightedFront.z));
    }       
}


void processInput(GLFWwindow* window)
{
    void *data = glfwGetWindowUserPointer(window);
    Context * context = static_cast<Context *> (data);
    Camera * camera = context->camera;
    Scene * scene = context->scene;
    
    int state;

    vec3 front;
    vec3 right = normalize(camera->getRotation() * VEC_RIGHT);

    int front_key;
    int back_key;
    int right_key;
    int left_key;
    int pitch_up_key;
    int pitch_down_key;
    int yaw_up_key;
    int yaw_down_key;
    int height_up_key;
    int height_down_key;

    front_key = GLFW_KEY_W;
    back_key = GLFW_KEY_S;
    right_key = GLFW_KEY_D;
    left_key = GLFW_KEY_A;
    pitch_up_key = GLFW_KEY_UP;
    pitch_down_key = GLFW_KEY_DOWN;
    yaw_up_key = GLFW_KEY_RIGHT;
    yaw_down_key = GLFW_KEY_LEFT;
    height_down_key = GLFW_KEY_Q;
    height_up_key = GLFW_KEY_E;


    if (!camera->getDefaultMovementMode()){
        front = normalize(camera->getRotation() * VEC_FRONT);
        front = vec3(front.x, 0.0, front.z);
        front = normalize(front);

        state = glfwGetKey(window, pitch_up_key);
        if (state == GLFW_PRESS){
            camera->getEulerAngle().x -= 1.0f * camera->getRotationSpeed();
        }

        state = glfwGetKey(window, pitch_down_key);
        if (state == GLFW_PRESS){
            camera->getEulerAngle().x += 1.0f * camera->getRotationSpeed();
        }

        state = glfwGetKey(window, yaw_up_key);
        if (state == GLFW_PRESS){
            camera->getEulerAngle().y -= 1.0f * camera->getRotationSpeed();
        }

        state = glfwGetKey(window, yaw_down_key);
        if (state == GLFW_PRESS){
            camera->getEulerAngle().y += 1.0f * camera->getRotationSpeed();
        }

        state = glfwGetKey(window, height_up_key);
        if (state == GLFW_PRESS){
            camera->getPosition().y += 1.0f * camera->getDeltaTime()
                * camera->getTranslationSpeed();
        }

        state = glfwGetKey(window, height_down_key);
        if (state == GLFW_PRESS){
            camera->getPosition().y -= 1.0f * camera->getDeltaTime()
                * camera->getTranslationSpeed();
        }

        if(camera->getEulerAngle().x > MAX_PITCH)
            camera->getEulerAngle().x = MAX_PITCH;
        if(camera->getEulerAngle().x < -MAX_PITCH)
            camera->getEulerAngle().x = -MAX_PITCH;

    }else{
        front = normalize(camera->getRotation() * VEC_FRONT);
    }

    // Move forward
    state = glfwGetKey(window, front_key);
    if (state == GLFW_PRESS){
        camera->getPosition() += front * camera->getDeltaTime() 
            * camera->getTranslationSpeed();
    }
    // Move backward
    state = glfwGetKey(window, back_key);
    if (state == GLFW_PRESS){
        camera->getPosition() -= front * camera->getDeltaTime() 
            * camera->getTranslationSpeed();
    }
    // Strafe right
    state = glfwGetKey(window, right_key);
    if (state == GLFW_PRESS){
        camera->getPosition() -= right * camera->getDeltaTime() 
            * camera->getTranslationSpeed();
    }
    // Strafe left
    state = glfwGetKey(window, left_key);
    if (state == GLFW_PRESS){
        camera->getPosition() += right * camera->getDeltaTime() 
            * camera->getTranslationSpeed();
    }
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    void *data = glfwGetWindowUserPointer(window);
    Context * context = static_cast<Context *> (data);
    Camera * camera = context->camera;
    Scene * scene = context->scene;

    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

    // Switch movement mode    
    if(key == GLFW_KEY_TAB && action == GLFW_PRESS){
        camera->getDefaultMovementMode() = !camera->getDefaultMovementMode();
    }

    if(key == GLFW_KEY_F && action == GLFW_PRESS){
        camera->getImGuiFocus() = !camera->getImGuiFocus();
    }

    if(key == GLFW_KEY_T && action == GLFW_PRESS){
        camera->getFirstReset() = true;
        camera->getResetting() = ! camera->getResetting();
    }
}