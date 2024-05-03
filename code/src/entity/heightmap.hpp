#pragma once

// Standard library
#include <src/entity/entity.hpp>
#include <string>

// GLM
#include <glm/glm.hpp>

#include <src/common/mesh.hpp>

#define MAX_HEIGHT 30.0f

struct Rectangle{
    glm::vec3 bottomLeft;
    glm::vec3 up;
    glm::vec3 right;
};

class HeightMap : public Entity
{
private:
    float m_maxHeight = 10.f;
    Rectangle m_map;
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

    float maxHeight() const { return m_maxHeight; }
    void maxHeight(float maxH) { m_maxHeight = maxH; }
    
 
    Rectangle map() const { return m_map; }
    void map(Rectangle map) { m_map = map; }

    std::string heightMapTexture() const { return m_heightMapTexture; }
    void heightMapTexture(const std::string path) { m_heightMapTexture = path;}

};