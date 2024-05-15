

#include "glm/detail/func_geometric.hpp"
#include "glm/fwd.hpp"
#include <reactphysics3d/body/RigidBody.h>
#include <reactphysics3d/components/RigidBodyComponents.h>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <reactphysics3d/engine/PhysicsWorld.h>
#include <reactphysics3d/mathematics/Quaternion.h>
#include <reactphysics3d/mathematics/Transform.h>
#include <reactphysics3d/mathematics/Vector3.h>
#include <src/entity/destructibleEntity.hpp>
#include <src/entity/fractureGenerator.hpp>


FractureGenerator::FractureGenerator()
{
    m_seed = 0;
    m_maxFratureDepth = 0;

    // Fracture settings
    m_voronoiType = VoronoiType::FORTUNESWEEP;
    m_circlePattern = true;
    m_bulletHoles = false;
    m_pointCount = 100;
    m_circleRadius = 8.0f;
    m_forceAll_mod = 100.0f;
}

FractureGenerator::~FractureGenerator()
{
}

bool FractureGenerator::Fracture(DestructibleEntity* object,
    const glm::vec2 hitPosition, const glm::vec3 hitDirection,
    std::vector<DestructibleEntity*>& objectList,
    reactphysics3d::PhysicsWorld* world,
    reactphysics3d::PhysicsCommon* physicCommon)
{
    const int fractureDepth = object->GetFractureDepth();
    if (object->IsDestroyed() || fractureDepth > m_maxFratureDepth)
        return false;

    // Get object data
    const std::vector<glm::vec2>& hitPolygon = object->GetPolygon();
    const int vertexAmount = (int)hitPolygon.size();
    const float& scale = object->getScale();
    const glm::vec2 min = object->GetMin() * 1.15f;
    const glm::vec2 max = object->GetMax() * 1.15f;

    //float pointRadius = (max.x + max.y);
    //m_circlePattern = true;
    const float pointDiamater	= m_circleRadius;	//	/ (1 + fractureDepth);		// Reduce diameter based on fracture depth
    const int numpoints			= m_pointCount;		//	/ (1 + fractureDepth);				// Reduce amount of points based on fracture depth

    if (vertexAmount >= 3)
    {
        
        jcv_point* j_points = (jcv_point*)malloc(sizeof(jcv_point) * numpoints);
        RandomizePoints(numpoints, pointDiamater, j_points, hitPosition);

        // Clip polygon
        jcv_clipping_polygon jcv_clippingPolygon;
        jcv_clippingPolygon.num_points = vertexAmount;
        jcv_clippingPolygon.points = (jcv_point*)malloc(sizeof(jcv_point) * (size_t)vertexAmount);

        // Polygon received from object
        for (int i = 0; i < (size_t)jcv_clippingPolygon.num_points; i++)
        {
            jcv_clippingPolygon.points[i].x = hitPolygon[i].x;
            jcv_clippingPolygon.points[i].y = hitPolygon[i].y;
        }

        jcv_clipper jcv_polygonclipper;
        jcv_polygonclipper.test_fn = jcv_clip_polygon_test_point;
        jcv_polygonclipper.clip_fn = jcv_clip_polygon_clip_edge;
        jcv_polygonclipper.fill_fn = jcv_clip_polygon_fill_gaps;
        jcv_polygonclipper.ctx = &jcv_clippingPolygon;
        jcv_clipper* clipper = &jcv_polygonclipper;


        jcv_rect rect;
        rect.min.x = min.x;
        rect.min.y = min.y;

        rect.max.x = max.x;
        rect.max.y = max.y;


        // Generate voronoi diagram
        jcv_diagram voronoiDiagram;
        memset(&voronoiDiagram, 0, sizeof(jcv_diagram));
        jcv_diagram_generate(numpoints, j_points, &rect, clipper, &voronoiDiagram);
        //jcv_diagram_generate(numpoints, j_points, 0, 0, &voronoiDiagram);

        // Sites and polygons
        const jcv_site* sites = jcv_diagram_get_sites(&voronoiDiagram);

        // If it fails
        if (voronoiDiagram.numsites == 0)
        {
            RandomizePoints(numpoints, pointDiamater, j_points, hitPosition);

            // Clip polygon
            jcv_clippingPolygon.num_points = vertexAmount;
            free(jcv_clippingPolygon.points);
            jcv_clippingPolygon.points = (jcv_point*)malloc(sizeof(jcv_point) * (size_t)vertexAmount);
            // Polygon received from object
            for (int i = 0; i < (size_t)jcv_clippingPolygon.num_points; i++)
            {
                jcv_clippingPolygon.points[i].x = hitPolygon[i].x;
                jcv_clippingPolygon.points[i].y = hitPolygon[i].y;
            }
            jcv_polygonclipper.test_fn = jcv_clip_polygon_test_point;
            jcv_polygonclipper.clip_fn = jcv_clip_polygon_clip_edge;
            jcv_polygonclipper.fill_fn = jcv_clip_polygon_fill_gaps;
            jcv_polygonclipper.ctx = &jcv_clippingPolygon;
            clipper = &jcv_polygonclipper;

            // Generate voronoi diagram
            jcv_diagram_free(&voronoiDiagram);
            memset(&voronoiDiagram, 0, sizeof(jcv_diagram));
            jcv_diagram_generate(numpoints, j_points, 0, clipper, &voronoiDiagram);

            sites = jcv_diagram_get_sites(&voronoiDiagram);
            if (voronoiDiagram.numsites == 0)
            {
                return false;
            }
        }


        // For each site
        for (int i = 0; i < voronoiDiagram.numsites; ++i)
        {
            std::vector<glm::vec2> siteVertices;
            const jcv_graphedge* j_graphedge = sites[i].edges;
            // For each vertex in site
            while (j_graphedge)
            {
                // Get first vertex of edge
                float edgeX = j_graphedge->pos[0].x;
                float edgeY = j_graphedge->pos[0].y;
                glm::vec2 edgePos = glm::vec2(edgeX, edgeY);

                if (siteVertices.size() == 0)
                {
                    siteVertices.emplace_back(edgePos);
                }
                else if (glm::length(edgePos - siteVertices[0]) > 0.0001f)
                {
                    if (glm::length(edgePos - siteVertices.back()) > 0.0001f)
                    {
                        siteVertices.emplace_back(edgePos);
                    }
                }
                else
                {
                    break;
                }


                // Next edge in site
                j_graphedge = j_graphedge->next;
            }


            glm::vec2 min = siteVertices[0];
            glm::vec2 max = siteVertices[0];
            for (size_t i = 0; i < siteVertices.size(); i++)
            {
                min.x = fminf(siteVertices[i].x, min.x);
                min.y = fminf(siteVertices[i].y, min.y);

                max.x = fmaxf(siteVertices[i].x, max.x);
                max.y = fmaxf(siteVertices[i].y, max.y);
            }

            glm::vec2 s_center = glm::vec2((min + max) * 0.5f);
            for (size_t i = 0; i < siteVertices.size(); i++)
            {
                siteVertices[i] -= s_center;
            }

            Mesh* newMesh = nullptr;
            if (MeshFromSite(newMesh, scale, siteVertices))
            {

                DestructibleEntity* newObject = new DestructibleEntity(this);
                newObject->SetPolygon(siteVertices);
                newObject->SetScale(scale);
                newObject->SetFractureDepth(fractureDepth + 1);
                if(newObject->GetFractureDepth() > m_maxFratureDepth)
                    newObject->SetDestroyed(true);

                newObject->currentMesh() = *newMesh;

                reactphysics3d::Transform newTransform = object->physicalEntity()->getTransform();

                // Create mesh
                reactphysics3d::Quaternion orientation = newTransform.getOrientation().getInverse();
                glm::vec3 center = glm::vec3(s_center, 0.0f) *
                    glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);

                // Fix placement
                // glm::vec3 sitePosition = glm::vec3(sites[i].p.x, sites[i].p.y, 0.0f);
                reactphysics3d::Vector3 currentPosition = newTransform.getPosition();
                // newTransform.setPosition(currentPosition + reactphysics3d::Vector3(center.x, center.y, center.z));

                glm::vec3 curPos = glm::vec3(currentPosition.x, currentPosition.y, currentPosition.z);
                newObject->movement().position = curPos + (center * object->currentTransform().scale);
                newObject->currentTransform().scale = object->currentTransform().scale;
                newObject->updateSelfAndChild();
                newObject->move();
                newObject->currentMesh().color(object->currentMesh().color());
                newObject->currentMesh().hasTexture(false);

                newObject->loadEntity(world);

                newObject->createCollider(/* reactphysics3d::Vector3(), */ physicCommon);

                // Inherit forces
                reactphysics3d::RigidBody* body = object->physicalEntity();
                reactphysics3d::RigidBody* body2 = newObject->physicalEntity();
                body2->setLinearVelocity(body->getLinearVelocity());
                body2->setAngularVelocity(body->getAngularVelocity());

                // Apply new forces
                //ApplyPhysicsFracture(newObject, hitPosition, center, hitDirection);
                float oldForce = m_forceAll_mod;

                if (scale > 0.3f)
                {
                    m_forceAll_mod *= 10.0f;
                    m_forceAll_mod *= std::pow(1.2f, scale);
                }

                ApplyPhysicsFracture(newObject, hitPosition, glm::vec3(s_center, 0.0f), hitDirection);
                m_forceAll_mod = oldForce;

                // newObject->SetDestroyed(true);

                objectList.push_back(newObject);
            }
        }

        // jcv free diagram memory
        jcv_diagram_free(&voronoiDiagram);

        //free(clipper);
        free(jcv_clippingPolygon.points);
        free(j_points);

        object->SetDestroyed(true);


        return true;
    }

    return false;
}

void FractureGenerator::RandomizePoints(const int& numpoints, const float& pointRadius, jcv_point* j_points, const glm::vec2& hitPosition)
{
    // Randomize points and move to hitlocation

    for (int i = 0; i < numpoints; i++)
    {
        if (m_circlePattern)
        {
            float radius = pointRadius;
            
            float range = 1.0f;
            // float rnd = 1.0f + (float)(rand() / (1.0f + RAND_MAX) * range) - (range / 2);
            // radius *= rnd; 
            float rnd = (float)(rand() / (1.0f + RAND_MAX));
            radius *= glm::sqrt(rnd);
            
            rnd = (float)(rand() / (1.0f + RAND_MAX));
            
            float angle = 2.0f * glm::pi<float>() * rnd;
            
            j_points[i].x = radius * glm::cos(angle);
            j_points[i].y = radius * glm::sin(angle);
        }
        else
        {
            j_points[i].x = (float)(rand() / (1.0f + RAND_MAX) * pointRadius) - (pointRadius / 2);
            j_points[i].y = (float)(rand() / (1.0f + RAND_MAX) * pointRadius) - (pointRadius / 2);
        }

        // Move to impact location
        j_points[i].x += hitPosition.x;
        j_points[i].y += hitPosition.y;

        j_points[0].x = hitPosition.x;
        j_points[0].y = hitPosition.y;
    }
}

void FractureGenerator::SetPointAmount(int amount)
{
    m_pointCount = amount;
}

void FractureGenerator::SetRadiusSize(float radius)
{
    m_circleRadius = radius;
}

void FractureGenerator::SetForceStrength(float str)
{
    m_forceAll_mod = str;
}

void FractureGenerator::SetBulletHoles(bool arg)
{
    m_bulletHoles = arg;
}

void FractureGenerator::SetCirclePattern(bool arg)
{
    m_circlePattern = arg;
}

void FractureGenerator::ApplyPhysicsFracture(DestructibleEntity* object, const glm::vec2& hitPosition, const glm::vec3& center, const glm::vec3& hitDirection)
{
    const reactphysics3d::Transform& newTransform = object->physicalEntity()->getTransform();
	float forceAll_mod = 10.0;
	float forceAway_str = 1.0;
	float forceHit_str = 10;
	float distance = 1.0f + glm::length(center - glm::vec3(hitPosition, 0.0f));

	float distanceMod = 1.0;
	if (distanceMod > 0)
	{
		//forceHit_str *= 1 /  ((1 - distanceMod) * 0.0f + distanceMod * 1.0f);// glm::lerp(0.0f, 1.0f, distanceMod);
		//forceHit_str *= 1 / std::powf(distance / 8, 1.4f);
		//forceAway_str += std::powf(distance / 8, 1.5f);

		forceAway_str *= std::fminf((1.0f + (distance / 12)), 10.0f);

		forceHit_str /= distance;

	}

    reactphysics3d::Quaternion orientation = newTransform.getOrientation().getInverse();

	glm::vec3 forceAway_dir = glm::normalize(center - glm::vec3(hitPosition, 0.0f)) *
        glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);
	forceAway_dir *= forceAway_str;


	//float dirRnd = (float)(rand() / (1.0f + RAND_MAX) * spinForce) - (spinForce / 2);
	//float spinForce_mod = 0.2f * (forceAll_mod / 4);
	// Randomize force
	float randRange = 0.5f;
	float forceRand = 1.0f + (float)(rand() / (1.0f + RAND_MAX) * randRange) - (randRange / 2);

	glm::vec3 spinVector = glm::normalize(glm::cross(center - glm::vec3(hitPosition, 0.0f), -hitDirection));
	spinVector.x = (2 * (float)rand() / (float)RAND_MAX - 1) * 1.0f; // * (distance / 2);
	spinVector.y = (2 * (float)rand() / (float)RAND_MAX - 1) * 1.0f; // * (distance / 2);
	spinVector.z = (2 * (float)rand() / (float)RAND_MAX - 1) * 1.0f; // * (distance / 2);
	spinVector.x *= 1.0f + (float)(rand() / (1.0f + RAND_MAX) * randRange) - (randRange / 2);
	spinVector.y *= 1.0f + (float)(rand() / (1.0f + RAND_MAX) * randRange) - (randRange / 2);
	spinVector.z *= 1.0f + (float)(rand() / (1.0f + RAND_MAX) * randRange) - (randRange / 2);

	glm::vec3 forceSpin = spinVector * (m_forceAll_mod / 40);
	forceSpin *= (distance * 5);

	glm::vec3 force = (forceAway_dir + (hitDirection * forceHit_str)) * m_forceAll_mod;
	force.x *= 1 + (float)(rand() / (1.0f + RAND_MAX) * randRange) - (randRange / 2);
	force.y *= 1 + (float)(rand() / (1.0f + RAND_MAX) * randRange) - (randRange / 2);
	force.z *= 1 + (float)(rand() / (1.0f + RAND_MAX) * randRange) - (randRange / 2);

	reactphysics3d::RigidBody* body = object->physicalEntity();
	if (body)
	{	
		body->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(force.x, force.y, force.z));
		//body->setLinearVelocity(btVector3(force.x * 0.2, force.y * 0.2, force.z * 0.2));
		//body->setAngularVelocity(body->getAngularVelocity() + btVector3(forceSpin.x, forceSpin.y, forceSpin.z));
		//body->setAngularVelocity(btVector3(forceSpin.x, forceSpin.y, forceSpin.z));
		//body->applyTorqueImpulse(btVector3(spinVector.x, spinVector.y, spinVector.z));
		body->applyLocalTorque(reactphysics3d::Vector3(forceSpin.x, forceSpin.y, forceSpin.z));
	}
}

bool FractureGenerator::MeshFromSite(Mesh*& mesh, const float& scale, const std::vector<glm::vec2>& siteVertices)
{
    int vertexCount = siteVertices.size();
    if (vertexCount >= 3)
    {
        if (mesh)
        {
            mesh->clear();
            delete mesh;
        }
        mesh = new Mesh();


        std::vector<glm::vec3> newVertices;
        std::vector<glm::vec3> newNormals;

        std::vector<unsigned short> newFaces;
        glm::vec3 normal;

        newVertices.resize(6 * (size_t)vertexCount);
        newNormals.resize(6 * (size_t)vertexCount);
        newFaces.resize(3 * (4 * (size_t)vertexCount - 4));

        int vi = 0;		// Vertex index
        int uvi = 0;	// UV index
        int ni = 0;		// Normal index
        int fi = 0;		// Face index

        // 0 - Bottom Left
        // 1 - Bottom Right
        // 2 - Top Right
        // 3 - Top Left
        //float uvmin_u = uv[0].x;
        //float uvmax_u = uv[2].x;
        //
        //float uvmin_v = uv[0].y;
        //float uvmax_v = uv[2].y;
        // TODO: Check and confirm this -> UV.u is inversed

        // Front
        for (int i = 0; i < vertexCount; i++)
        {
            newVertices[vi++] = glm::vec3(siteVertices[i].x, siteVertices[i].y, scale);

            //newVertices[uvi++].UV = glm::vec2(
            //	(uvmax_u - uvmin_u) * (siteVertices[i].x - polygon[0].x) / (polygon[2].x - polygon[0].x) + uvmin_u,
            //	(uvmax_v - uvmin_v) * (siteVertices[i].y - polygon[0].y) / (polygon[2].y - polygon[0].y) + uvmin_v
            //);

            newNormals[ni++] = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        // Back
        for (int i = 0; i < vertexCount; i++)
        {
            newVertices[vi++] = glm::vec3(siteVertices[i].x, siteVertices[i].y, -scale);

            //newVertices[uvi++].UV = glm::vec2(
            //	(uvmax_u - uvmin_u) * (siteVertices[i].x - polygon[0].x) / (polygon[2].x - polygon[0].x) + uvmin_u,
            //	(uvmax_v - uvmin_v) * (siteVertices[i].y - polygon[0].y) / (polygon[2].y - polygon[0].y) + uvmin_v
            //);

            newNormals[ni++] = glm::vec3(0.0f, 0.0f, -1.0f);
        }

        // Sides
        for (int i = 0; i < vertexCount; i++)
        {
            int iNext = i == vertexCount - 1 ? 0 : i + 1;

            newVertices[vi++] = glm::vec3(siteVertices[i].x, siteVertices[i].y, scale);
            newVertices[vi++] = glm::vec3(siteVertices[i].x, siteVertices[i].y, -scale);

            newVertices[vi++] = glm::vec3(siteVertices[iNext].x, siteVertices[iNext].y, -scale);
            newVertices[vi++] = glm::vec3(siteVertices[iNext].x, siteVertices[iNext].y, scale);

            normal = glm::normalize(glm::cross(glm::vec3(siteVertices[iNext] - siteVertices[i], 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

            newNormals[ni++] = normal;
            newNormals[ni++] = normal;
            newNormals[ni++] = normal;
            newNormals[ni++] = normal;
        }

        for (int vert = 2; vert < vertexCount; vert++)
        {
            newFaces[fi] = 0;
            newFaces[fi+1] = vert - 1;
            newFaces[fi+2] = vert;
            fi+=3;
        }

        for (int vert = 2; vert < vertexCount; vert++)
        {
            newFaces[fi] = vertexCount;
            newFaces[fi+1] = vertexCount + vert;
            newFaces[fi+2] = vertexCount + vert - 1;
            fi+=3;
        }

        for (int vert = 0; vert < vertexCount; vert++)
        {
            int si = 2 * vertexCount + 4 * vert;

            newFaces[fi] = si;
            newFaces[fi+1] = si + 1;
            newFaces[fi+2] = si + 2;
            fi+=3;

            newFaces[fi] = si;
            newFaces[fi+1] = si + 2;
            newFaces[fi+2] = si + 3;
            fi+=3;
        }




        // Apply the vertices and faces to the mesh
        *mesh = Mesh();
        mesh->vertexPosition() = newVertices;
        mesh->vertexNormals() = newNormals;
        mesh->triangleIndices() = newFaces;

        return true;
    }
    else
    {
        return false;
    }


    return false;
}

const unsigned int FractureGenerator::seedRand(int seed)
{
    if (seed == -1)
    {
        if (m_seed == int(time(NULL)))
        {
            m_seed = int(time(NULL));
            srand(m_seed);
        }
        else
        {
            return m_seed;
        }
    }
    else
    {
        srand(seed);
        return seed;
    }

    return m_seed;
}





