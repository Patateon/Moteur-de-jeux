#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "src/camera/camera.hpp"

class SkyBox {
public:
    SkyBox();
    ~SkyBox();

    bool loadSkyBox(std::vector<std::string> faces);
    void renderSkyBox(const Camera& _camera, GLuint _matrixID) ;

private:
    GLuint m_textureID;
    GLuint m_vao, m_vbo;
};

#endif /* SKYBOX_HPP */
