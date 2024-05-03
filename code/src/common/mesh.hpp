#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <src/common/texture.hpp>

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
    bool m_has_texture = false;

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

    //// Getter par référence des attributs de notre mesh
    // Renvoie le pointeur et la valeur (Donc pas besoin de Setter)
    std::vector<glm::vec3> & vertexPosition() {return m_vertexPositions;}
    std::vector<glm::vec3> & vertexNormals() {return m_vertexNormals;}
    std::vector<glm::vec2> & vertexTexCoords() {return m_vertexTexCoords;}
    std::vector<unsigned short> & triangleIndices() {return m_triangleIndices;}

    // Recompute normals and texture coordinates from vertex
    void recomputeNormals();
    void recomputeTextureCoordinates();

    // Renvoie le barycentre des sommets du maillage
    glm::vec3 getBarycentre();

    // Permet d'attributer une texture à un maillage
    bool hasTexture() const { return m_has_texture; }
    void hasTexture(bool has_texture) { m_has_texture = has_texture; }

    // Permet d'attributer une couleur à un maillage
    glm::vec3 color() const { return m_color; }
    void color(glm::vec3 color) { m_color = color; }

    //// Indique si le maillage doit utiliser la texture ou la couleu
    // Par défaut, le maillage utilise une couleur (m_has_texture = false)
    void toggleTexture() {m_has_texture = ! m_has_texture;}

    // Textures
    GLuint texture() const {return m_texture;}
    void texture(const std::string& path) {m_texture = loadTexture2DFromFilePath(path);}

    //// Fill buffers, renders with thoses buffers, clear them all
    // Init before de mainloop
    void init();
    // Render each loop
    void render(GLuint _colorID, GLuint _hasTextureID);
    // Clear after the mainloop
    void clear();
};
