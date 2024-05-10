#include <reactphysics3d/engine/PhysicsCommon.h>
#include <src/common/mesh.hpp>
#include <src/entity/heightmap.hpp>
#include <src/common/stb_image.h>
#include <iostream>
#include <vector>
using namespace std;
using namespace glm;

HeightMap::HeightMap(){}

HeightMap::HeightMap(Rectangle map){
    m_map = map;
}

HeightMap::HeightMap(Rectangle map, int hRes, int vRes, const std::string & filename){
    m_map = map;
    m_heightMapTexture = filename;

    build(hRes, vRes);
}

void HeightMap::build(int hRes, int vRes){
    m_res.h = hRes;
    m_res.v = vRes;

    vector<unsigned short> triangles;
    vector<vec3> vertices;
    vector<vec2> texCoords;

    generateSurface(&triangles, &vertices, &texCoords, m_map, hRes, vRes);
    currentMesh() = Mesh(vertices, triangles);
    currentMesh().vertexTexCoords() = texCoords;
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

reactphysics3d::Collider* HeightMap::createCollider(reactphysics3d::PhysicsCommon* _physicsCommon){

    using namespace reactphysics3d;

    float heightValue[m_res.h * m_res.v];
    std::cout<<m_res.h * m_res.v<<std::endl;

    float minHeight = FLT_MAX;
    float maxHeight = FLT_MIN;

    
    for(uint i = 0; i < currentMesh().vertexPosition().size(); i++){
        heightValue[i] = currentMesh().vertexPosition()[i].y;
        minHeight = minHeight < heightValue[i] ? minHeight : heightValue[i]; 
        maxHeight = maxHeight > heightValue[i] ? maxHeight : heightValue[i]; 
    }

    std::vector<reactphysics3d::Message> logHeightMap;
    HeightField* heightField = _physicsCommon->createHeightField(
        m_res.h, m_res.v, heightValue,
        reactphysics3d::HeightField::HeightDataType::HEIGHT_FLOAT_TYPE,
        logHeightMap);

    

    // Display the messages (info, warning and errors)
    if (logHeightMap.size() > 0) {
    
        for (const rp3d::Message& message: logHeightMap) {
    
            std::string messageType;
    
            switch(message.type) {
                case rp3d::Message::Type::Information:
                    messageType = "info";
                    break;
                case rp3d::Message::Type::Warning:
                    messageType = "warning";
                    break;
                case rp3d::Message::Type::Error:
                    messageType = "error";
                    break;
            }
    
            std::cout << "Message (" << messageType << "): " << message.text << std::endl;
            return nullptr;
        }
    }
    
    // Make sure there was no errors during the height field creation
    assert(heightField != nullptr);

    HeightFieldShape* heightMapShape = _physicsCommon->createHeightFieldShape(heightField, Vector3(1.0f, 1.f, 1.0f));
    AABB heightMapBound = heightMapShape->getLocalBounds();
    float heightMapScaleX = ((this->map().right.length()) / 2.0f) / heightMapBound.getMax().x;
    float heightMapScaleZ = ((this->map().up.length()) / 2.0f) / heightMapBound.getMax().z;
    Vector3 heightMapScale = Vector3(heightMapScaleX, 1.0f, heightMapScaleZ);
    heightMapShape->setScale(heightMapScale);
    Transform heightMapTransform = Transform(Vector3(0.0f, heightMapBound.getMax().y, 0.0f), Quaternion::identity());

    Collider* collider = this->physicalEntity()->addCollider(heightMapShape, heightMapTransform);
    return collider;
}

// À partir d'un rectangle et d'une résolution, 
// génère les sommets et les triangles
// d'une surface plane correspondante
void HeightMap::generateSurface(
    vector<unsigned short> * indices_surface,
    vector<vec3> * vertices_surface,
    vector<vec2> * tex_coords,
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
            vec2 new_texCoords = vec2(1.0f - ((float) i / (float) (vertical_res-1)), (float) j / (float) (horizontal_res-1));
            vertices_surface->push_back(new_points);
            tex_coords->push_back(new_texCoords);
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
