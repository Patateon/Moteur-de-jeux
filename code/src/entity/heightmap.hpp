#pragma once

// Standard library
#include <src/entity/entity.hpp>
#include <string>

// GLM
#include <glm/glm.hpp>

#include <src/common/mesh.hpp>
#include <src/camera/camera.hpp>

#define MAX_HEIGHT 30.0f

struct Rectangle{
    glm::vec3 bottomLeft;
    glm::vec3 up;
    glm::vec3 right;
};

struct Resolution{
    int h;
    int v;
};

class HeightMap : public Entity
{
private:
    float m_maxHeight = 10.f;
    Rectangle m_map;
    std::string m_heightMapTexture;
    Resolution m_res;

public:
    HeightMap();
    HeightMap(Rectangle map);
    HeightMap(Rectangle map, int hRes, int vRes, const std::string & filename);

    static void generateSurface(
        std::vector<unsigned short> * indices_surface,
        std::vector<glm::vec3> * vertices_surface,
        std::vector<glm::vec2> * tex_coords,
        Rectangle rectangle,
        int horizontal_res, int vertical_res
    );

    void build(int hRes, int vRes);
    
    void applyHeightMap();

    reactphysics3d::Collider* createCollider(reactphysics3d::PhysicsCommon* _physicsCommon);

    float maxHeight() const { return m_maxHeight; }
    void maxHeight(float maxH) { m_maxHeight = maxH; }
 
    Rectangle map() const { return m_map; }
    void map(Rectangle map) { m_map = map; }

    Resolution res() const { return m_res; }

    std::string heightMapTexture() const { return m_heightMapTexture; }
    void heightMapTexture(const std::string path) { m_heightMapTexture = path;}

};