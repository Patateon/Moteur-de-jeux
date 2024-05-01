#pragma once

// Standard library
#include "common/entity.hpp"
#include <string>

// GLM
#include <glm/glm.hpp>

#include <common/mesh.hpp>

struct Rectangle{
    glm::vec3 bottomLeft;
    glm::vec3 up;
    glm::vec3 right;
};

class HeightMap : public Entity
{
private:
    Rectangle m_map;

    // Mesh m_currentMesh;

    std::string m_heightMapTexture;
public:
    
    HeightMap(Rectangle map);
    HeightMap(Rectangle map, int hRes, int vRes, std::string path);

    static void generateSurface(
        std::vector<unsigned short> * indices_surface,
        std::vector<glm::vec3> * vertices_surface,
        Rectangle rectangle,
        int horizontal_res, int vertical_res
    );

    void build(int hRes, int vRes);
    
    void applyHeightMap();
 
    Rectangle map() const { return m_map; }
    void map(Rectangle map) { m_map = map; }

    std::string heightMapTexture() const { return m_heightMapTexture; }
    void heightMapTexture(const std::string path) { m_heightMapTexture = path;}

};