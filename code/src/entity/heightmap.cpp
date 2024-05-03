#include <src/common/mesh.hpp>
#include <src/entity/heightmap.hpp>
#include <src/common/stb_image.h>
#include <iostream>
#include <vector>
using namespace std;
using namespace glm;


HeightMap::HeightMap(Rectangle map){
    m_map = map;
}

HeightMap::HeightMap(Rectangle map, int hRes, int vRes, std::string path){
    m_map = map;
    m_heightMapTexture = path;

    build(hRes, vRes);
}

void HeightMap::build(int hRes, int vRes){
    vector<unsigned short> triangles;
    vector<vec3> vertices;

    generateSurface(&triangles, &vertices, m_map, hRes, vRes);
    currentMesh() = Mesh(vertices, triangles);
    currentMesh().recomputeTextureCoordinates();
    applyHeightMap();
    currentMesh().recomputeNormals();
}

void HeightMap::applyHeightMap() {
    // Set height from height map
    int width = 0, height = 0, nChannels;
    unsigned char *data = stbi_load(m_heightMapTexture.c_str(),
        &width, &height, &nChannels,
        0);

    if(data == NULL){
        cerr<<"Couldn't read height map from file: "<<m_heightMapTexture<<endl;
        return; 
    }

    for(uint it = 0; it < currentMesh().vertexPosition().size(); it++){
        // retrieve texel for (i,j) tex coord
        int i = currentMesh().vertexTexCoords()[it].x * (height - 1);
        int j = currentMesh().vertexTexCoords()[it].y * (width - 1);

        unsigned char* texel = data + (j + width * i) * nChannels;
        // raw height at coordinate
        float y = texel[0] / 255.f;

        // vertex
        currentMesh().vertexPosition()[it].y = y * m_maxHeight;
    }
}

// À partir d'un rectangle et d'une résolution, 
// génère les sommets et les triangles
// d'une surface plane correspondante
void HeightMap::generateSurface(
    vector<unsigned short> * indices_surface,
    vector<vec3> * vertices_surface,
    Rectangle rectangle,
    int horizontal_res, int vertical_res){
    unsigned short v0, v1, v2, v3;

    indices_surface->clear();
    vertices_surface->clear();

    for (int i = 0; i<vertical_res; i++){
        for (int j = 0; j<horizontal_res; j++){
            float horizontal_step = (float) j / (float) (horizontal_res-1);
            float vertical_step = (float) i / (float) (vertical_res-1);
            vec3 new_points = rectangle.bottomLeft + (rectangle.right*horizontal_step) + (rectangle.up*vertical_step);
            vertices_surface->push_back(new_points);
        }
    }

    for (int i = 0; i<vertical_res - 1; i++){
        for (int j = 0; j<horizontal_res - 1; j++){
            v0 = i * horizontal_res + j;
            v1 = i * horizontal_res + (j+1);
            v2 = (i+1) * horizontal_res + j;
            v3 = (i+1) * horizontal_res + (j+1);

            // Triangle {(i,j), (i,j+1), (i+1,j)}
            indices_surface->push_back(v0);
            indices_surface->push_back(v1);
            indices_surface->push_back(v2);
            
            // Triangle {(i+1,j), (i,j+1), (i+1,j+1)}
            indices_surface->push_back(v2);
            indices_surface->push_back(v1);
            indices_surface->push_back(v3);
        }
    }
}
