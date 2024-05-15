
#include <src/entity/scene.hpp>
#include <src/camera/camera.hpp>
#include <src/camera/camerahelper.hpp>
#include <src/common/control.hpp>

// Include GLM
#include <external/imgui/imgui.h>
#include <external/imgui/imgui_impl_glfw.h>
#include <external/imgui/imgui_impl_opengl3.h>


using namespace glm;

void Camera::init()
{
    m_fovDegree = 45.0f;
    m_position = vec3(0.f, 40.f, -40.f);
    getInitPosition() = m_position;
    m_eulerAngle = vec3(0.f, 0.f, 0.f);
    m_rotation = quat{};
    m_defaultMovementMode = true;
    m_translationSpeed = 30.0f;
    m_rotationSpeed = 0.15f;
}


void Camera::updateInterface(float _deltaTime)
{
    // ImGUI window creation
    if(ImGui::Begin("Settings"))
    {
        ImGui::Separator();
        ImGui::Text("Welcome to this TP about Cameras! Press escape to close the exe");
        ImGui::Text("Long live to the cameras");
        ImGui::Separator();
        ImGui::Text("FPS: %f", 1.f /_deltaTime);
        ImGui::Separator();
        if(ImGui::CollapsingHeader("Camera settings")){
            updateCameraSettingsInterface(_deltaTime);
        }
    }
    ImGui::End();
}


void Camera::updateCameraSettingsInterface(float _deltaTime)
{
    ImGui::Text("Delta Time %f", _deltaTime);
    ImGui::Separator();

    ImGui::Text("Camera Position (%f, %f, %f)", m_position.x, m_position.y, m_position.z);
    ImGui::SliderFloat("X", &m_position.x, -100.f, 100.f);
    ImGui::SliderFloat("Y", &m_position.y, -100.f, 100.f);
    ImGui::SliderFloat("Z", &m_position.z, -100.f, 100.f);
    ImGui::Separator();
    
    ImGui::Text("Euler Angle (%f, %f, %f)", m_eulerAngle.x, m_eulerAngle.y, m_eulerAngle.z);
    ImGui::SliderFloat("Pitch", &m_eulerAngle.x, -90.f, 90.f);
    ImGui::SliderFloat("Yaw", &m_eulerAngle.y, -180.f, 180.f);
    ImGui::SliderFloat("Roll", &m_eulerAngle.z, -180.f, 180.f);
    ImGui::Separator();

    ImGui::Text("Field of View (%f)", m_fovDegree);
    ImGui::SliderFloat("FOV", &m_fovDegree, 15.f, 180.f);
    ImGui::Separator();

    ImGui::Text("Translation Speed (%f)", m_translationSpeed);
    ImGui::SliderFloat("Translation speed", &m_translationSpeed, 0.f, 200.f);
    ImGui::Separator();

    ImGui::Text("Rotation Speed (%f)", m_rotationSpeed);
    ImGui::SliderFloat("Rotation speed", &m_rotationSpeed, 0.f, 50.f);
    ImGui::Separator();

    ImGui::Text("Camera Mode (%d)", !m_defaultMovementMode);
    if(ImGui::Button("Togle")){
        m_defaultMovementMode = !m_defaultMovementMode;
    }
    ImGui::Separator();

    if(ImGui::Button("Restore default value")){
        init();
    }
    ImGui::Separator();
}


vec3 Camera::pathReset() {
    if (m_path_reset == glm::vec3())
        return getInitPosition() - getFirstPositionReset();
    return m_path_reset;
}


void Camera::resetCameraPos(){
    if (m_firstReset){
        m_first_pos_reset = m_position;
        m_init_time = glfwGetTime();
        m_firstReset = false;
    }else{
        m_current_time = glfwGetTime(); 
        float u = (m_current_time - m_init_time) / m_duration;
        vec3 path_reset = pathReset();
        if (1 - u < epsilon_camera){
            m_resetting = false;
            m_firstReset = false;
        }else{
            m_position = mix(m_first_pos_reset, m_init_position, Camera_Helper::interpolate_smooth(u));
        }
    }
}


void Camera::allInputs(GLFWwindow* _window){
    ImGuiIO& io = ImGui::GetIO();

    if(!io.WantCaptureKeyboard)
    {
        processInput(_window);
        glfwSetKeyCallback(_window, keyCallback);
    }

    if(!io.WantCaptureMouse)
    {
        if(! getImGuiFocus() && m_defaultMovementMode){
            glfwSetMouseButtonCallback(_window, mouseButtonCallback);
            glfwSetCursorPosCallback(_window, cursorPositionCallback);
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }else{
            getFirstInput() = true;
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}


void Camera::updateFreeInput(float _deltaTime, GLFWwindow* _window)
{
    this->getDeltaTime() = _deltaTime;
    // glfwSetWindowUserPointer(_window, this);
    
    ImGuiIO& io = ImGui::GetIO();

    if(!m_resetting){
        if(getFirstReset()){getFirstReset() = true;}
        
        allInputs(_window);
        getEulerAngle().y = Camera_Helper::clipAngle180(getEulerAngle().y);
    }else{
        resetCameraPos();
    }
}


void Camera::update(float _deltaTime, GLFWwindow* _window)
{
    updateInterface(_deltaTime);
    updateFreeInput(_deltaTime, _window);

    m_rotation = quat(radians(m_eulerAngle));

    Camera_Helper::computeFinalView(m_projectionMatrix, m_viewMatrix, m_position, m_rotation, m_fovDegree);
}

