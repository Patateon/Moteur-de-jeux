
#include <iostream>
#include <reactphysics3d/collision/ConvexMesh.h>
#include <reactphysics3d/collision/PolygonVertexArray.h>
#include <reactphysics3d/collision/VertexArray.h>
#include <reactphysics3d/collision/shapes/ConvexMeshShape.h>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <reactphysics3d/mathematics/Transform.h>
#include <reactphysics3d/mathematics/Vector3.h>
#include <reactphysics3d/utils/Message.h>
#include <src/entity/destructibleEntity.hpp>


DestructibleEntity::DestructibleEntity(FractureGenerator* fractureGen)
{
    fractureGenRef = fractureGen;
    m_scale = 0;
    m_lifetime = 0.0f;
}

DestructibleEntity::~DestructibleEntity()
{
    
    clear();
}


void DestructibleEntity::SetPolygon(std::vector<glm::vec2> vertices)
{
    m_polygonFace = vertices;
    CalculateMinMax();
}

void DestructibleEntity::SetScale(float scale)
{
    m_scale = scale;
}

void DestructibleEntity::CalculateMinMax()
{
    min = m_polygonFace[0];
    max = m_polygonFace[0];
    for (size_t i = 0; i < m_polygonFace.size(); i++)
    {
        min.x = fminf(m_polygonFace[i].x, min.x);
        min.y = fminf(m_polygonFace[i].y, min.y);

        max.x = fmaxf(m_polygonFace[i].x, max.x);
        max.y = fmaxf(m_polygonFace[i].y, max.y);
    }
}

void DestructibleEntity::LoadBasic(float size)
{
    
    m_polygonFace.resize(4);
    m_polygonUV.resize(4);

    float sizeX = 27;
    float sizeY = 18;

    m_polygonFace[0] = glm::vec2(-sizeX, -sizeY);
    m_polygonFace[1] = glm::vec2(sizeX, -sizeY);
    m_polygonFace[2] = glm::vec2(sizeX, sizeY);
    m_polygonFace[3] = glm::vec2(-sizeX, sizeY);

    m_scale = size;

    int count = 0;
    count = m_polygonFace.size();

    meshFromPolygon();
    CalculateMinMax();
}

void DestructibleEntity::LoadBasic2(float size)
{

    m_polygonFace.resize(4);
    m_polygonUV.resize(4);

    float sizeX = 27;
    float sizeY = 18;

    m_polygonFace.resize(12);
    m_polygonUV.resize(12);
    m_polygonFace[0] = glm::vec2(-30.0f, -19.0f);
    m_polygonFace[1] = glm::vec2(-15.0f, -21.0f);
    m_polygonFace[2] = glm::vec2(0.0f, -22.0f);
    m_polygonFace[3] = glm::vec2(15.0f, -21.0f);
    m_polygonFace[4] = glm::vec2(30.0f, -19.0f);
    m_polygonFace[5] = glm::vec2(35.0f, 0.0f);
    m_polygonFace[6] = glm::vec2(30.0f, 19.0f);
    m_polygonFace[7] = glm::vec2(15.0f, 21.0f);
    m_polygonFace[8] = glm::vec2(0.0f, 22.0f);
    m_polygonFace[9] = glm::vec2(-15.0f, 21.0f);
    m_polygonFace[10] = glm::vec2(-30.0f, 19.0f);
    m_polygonFace[11] = glm::vec2(-35.0f, 0.0f);

    m_scale = size;

    int count = 0;
    count = m_polygonFace.size();

    // meshFromPolygon(name);
    CalculateMinMax();
}


void DestructibleEntity::meshFromPolygon() {

    int count = 0;
    glm::vec3 normal = glm::vec3();

    count = m_polygonFace.size();

    std::vector<glm::vec3> newVertices;
    std::vector<glm::vec3> newNormal;
    std::vector<glm::vec2> newUV;
    newVertices.resize(6 * (size_t)count);
    newNormal.resize(6 * (size_t)count);
    newUV.resize(6 * (size_t)count);
    std::vector<unsigned short> newFace;
    newFace.resize((4 * (size_t)count - 4) * 3);

    int vi = 0;
    int uvi = 0;
    int ni = 0;
    int ti = 0;

    // Front
    for (int i = 0; i < count; i++)
    {
        newVertices[vi++] = glm::vec3(m_polygonFace[i].x, m_polygonFace[i].y, m_scale);
        newUV[uvi++] = glm::vec2(m_polygonUV[i].x, m_polygonUV[i].y);
        newNormal[ni++] = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    // Back
    for (int i = 0; i < count; i++)
    {
        newVertices[vi++] = glm::vec3(m_polygonFace[i].x, m_polygonFace[i].y, -m_scale);
        newUV[uvi++] = glm::vec2(m_polygonUV[i].x, m_polygonUV[i].y);
        newNormal[ni++] = glm::vec3(0.0f, 0.0f, -1.0f);
    }

    // Sides
    for (int i = 0; i < count; i++)
    {
        //int iNext = i == count - 1 ? 0 : i + 1;
        int iNext = 0;
        if (i == count - 1)
            iNext = 0;
        else
            iNext = i + 1;

        newVertices[vi++] = glm::vec3(m_polygonFace[i].x, m_polygonFace[i].y, m_scale);
        newVertices[vi++] = glm::vec3(m_polygonFace[i].x, m_polygonFace[i].y, -m_scale);

        newVertices[vi++] = glm::vec3(m_polygonFace[iNext].x, m_polygonFace[iNext].y, -m_scale);
        newVertices[vi++] = glm::vec3(m_polygonFace[iNext].x, m_polygonFace[iNext].y, m_scale);

        normal = glm::normalize(glm::cross(glm::vec3(m_polygonFace[iNext] - m_polygonFace[i], 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

        newNormal[ni++] = normal;
        newNormal[ni++] = normal;
        newNormal[ni++] = normal;
        newNormal[ni++] = normal;
    }

    // Pour chaque faces, (|Sommets du polyone| + 2)
    /* for (int i = 0; i < count + 2; i++)
    {
        newFace[ti] = i;
        newFace[ti+1] = i+1;
        newFace[ti+2] = i+2;
        ti += 3;

        newFace[ti] = i;
        newFace[ti+1] = i+2;
        newFace[ti+2] = i+3;
        ti += 3;
    } */

    // Faces
    for (int vert = 2; vert < count; vert++)
    {
        newFace[ti] = 0;
        newFace[ti+1] = vert - 1;
        newFace[ti+2] = vert;
        ti+=3;
    }

    for (int vert = 2; vert < count; vert++)
    {
        newFace[ti] = count;
        newFace[ti+1] = count + vert;
        newFace[ti+2] = count + vert - 1;
        ti+=3;
    }

    for (int vert = 0; vert < count; vert++)
    {
        int si = 2 * count + 4 * vert;

        newFace[ti] = si;
        newFace[ti+1] = si + 1;
        newFace[ti+2] = si + 2;
        ti+=3;

        newFace[ti] = si;
        newFace[ti+1] = si + 2;
        newFace[ti+2] = si + 3;
        ti+=3;
    }

    currentMesh() = Mesh(newVertices, newFace);
    currentMesh().vertexNormals() = newNormal;
    currentMesh().vertexTexCoords() = newUV;
}

reactphysics3d::Collider* DestructibleEntity::createCollider(reactphysics3d::PhysicsCommon* physicsCommon){
    using namespace reactphysics3d;

    uint32 numberOfVertex = currentMesh().vertexPosition().size();
    std::vector<glm::vec3> vertices = currentMesh().vertexPosition();
    uint32 numberOfIndices = currentMesh().triangleIndices().size();
    std::vector<unsigned short> us_indices = currentMesh().triangleIndices(); 

    VertexArray vertexArray(vertices.data(), 3 * sizeof(float),
        numberOfVertex, VertexArray::DataType::VERTEX_FLOAT_TYPE);


    // Polygon face to specify indices structures
    // PolygonVertexArray::PolygonFace* polygonFace = new PolygonVertexArray::PolygonFace[numberOfIndices / 3];
    // PolygonVertexArray::PolygonFace* face = polygonFace;


    // for(uint i = 0; i < numberOfIndices; i++){
    //     face[i].indexBase = i*3;
    //     face[i].nbVertices = 3;
    // }

    // // Convert indice type from unsigned short to short
    // std::vector<short> indices;
    // indices.resize(numberOfIndices);
    // for(uint i = 0; i < numberOfIndices; i++){
    //     indices[i] = (short) us_indices[i];
    // }
    

    // // Created polygon vertex array
    // PolygonVertexArray polygonVertexArray(numberOfVertex / 3, vertices.data(), sizeof(float),
    //     indices.data(), sizeof(short), (indices.size() / 3) / 3,
    //     polygonFace, PolygonVertexArray::VertexDataType::VERTEX_FLOAT_TYPE, PolygonVertexArray::IndexDataType::INDEX_SHORT_TYPE);



    std::vector<Message> messages;

    ConvexMesh* convexMesh = physicsCommon->createConvexMesh(vertexArray, messages);

    // Display the messages (info, warning and errors)
    if (messages.size() > 0) {
    
        for (const rp3d::Message& message: messages) {
    
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
        }
    }

    // Make sure there was no errors during mesh creation
    assert(convexMesh != nullptr);

    Vector3 scaling = Vector3(1.0f, 1.0f, 1.0f);

    ConvexMeshShape* convexMeshShape = physicsCommon->createConvexMeshShape(convexMesh, scaling);

    return physicalEntity()->addCollider(convexMeshShape, Transform::identity());
}
