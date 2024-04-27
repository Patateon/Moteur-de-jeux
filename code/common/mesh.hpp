#ifndef MESH_H
#define MESH_H

#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.hpp"

class Mesh
{
private:
    // Vertices buffers
	GLuint m_vao = 0;
	GLuint m_posVbo = 0;
	GLuint m_normalVbo = 0;
	GLuint m_texCoordVbo = 0;
	GLuint m_ebo = 0;

    // Texture buffers
	GLuint m_texture = 0;

    // Simple check to tell the fragment shader if the mesh has a texture
    bool has_texture = false;

    // Vertices attributs arrays
	std::vector<glm::vec3> m_vertexPositions;
	std::vector<glm::vec3> m_vertexNormals;
	std::vector<glm::vec2> m_vertexTexCoords;
	std::vector<unsigned short> m_triangleIndices;
    glm::vec3 m_color;

public:
    // Constructor
    Mesh(){}
    Mesh(
        std::vector<glm::vec3> vertices,
        std::vector<unsigned short> indices
    );
    // Destructor
    ~Mesh();

    std::vector<glm::vec3> & vertexPosition() {return m_vertexPositions;}
    std::vector<glm::vec3> & vertexNormals() {return m_vertexNormals;}
    std::vector<glm::vec2> & vertexTexCoords() {return m_vertexTexCoords;}
    std::vector<unsigned short> & triangleIndices() {return m_triangleIndices;}

    // Recompute normals and texture coordinates from vertex
    void recomputeNormals();
    void recomputeTextureCoordinates();

    // For mouvement computation
    glm::vec3 getBarycentre();

    bool getHasTexture() {return this->has_texture;}
    void setHasTexture(bool has_texture) {this->has_texture = has_texture;}
    void toggleTexture() {this->has_texture = ! this->has_texture;}

    // Colors
    glm::vec3 getColor() {return this->m_color;}
    void setColor(glm::vec3 color_in) {this->m_color = color_in;}

    // Textures
    GLuint getTexture() {return this->m_texture;}
    void setTexture(const std::string& path) {this->m_texture = loadTexture2DFromFilePath(path);}

    // Fill buffers, renders with thoses buffers, clear them all
    void init();
    void render();
    void clear();
};

#endif