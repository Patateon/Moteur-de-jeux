#pragma once

#define JC_VORONOI_IMPLEMENTATION
#define JC_VORONOI_CLIP_IMPLEMENTATION

#include <glm/glm.hpp>

#include <src/common/jc_voronoi.h>
#include <src/common/jc_voronoi_clip.h>
#include <src/common/mesh.hpp>

#include <src/entity/destructibleEntity.hpp>

enum class VoronoiType
{
	FORTUNESWEEP,
	DELAUNAY
};

class DestructibleEntity;

class FractureGenerator
{
public:
	FractureGenerator();
	~FractureGenerator();

	bool Fracture(DestructibleEntity * object, const glm::vec2 hitPosition, const glm::vec3 hitDirection, std::vector<DestructibleEntity*>& objectList, reactphysics3d::PhysicsWorld* world, reactphysics3d::PhysicsCommon* physicCommon);

	void SetPointAmount(int amount);
	void SetRadiusSize(float radius);
	void SetForceStrength(float str);
	void SetBulletHoles(bool arg);
	void SetCirclePattern(bool arg);

	const int& GetPointAmount() { return m_pointCount; };
	const float& GetRadiusSize() { return m_circleRadius; };
	const float& GetForceStrength() { return m_forceAll_mod; };
	const bool& GetBulletHoles() { return m_bulletHoles; };
	const bool& GetCirclePattern() { return m_circlePattern; };
	const unsigned int seedRand(int seed = -1);
	const unsigned int getSeed(unsigned int seed) const { return m_seed; }


private:
	void RandomizePoints(const int& numpoints, const float& pointDiamater, jcv_point* j_points, const glm::vec2& hitPosition);
	bool MeshFromSite(Mesh*& mesh, const float& scale, const std::vector<glm::vec2>& siteVertices);

	VoronoiType m_voronoiType;
	int m_seed;	// Seed for randomizer, used for network consistency


	// Settings
	int m_pointCount;
	int m_maxFratureDepth;
	bool m_circlePattern;
	bool m_bulletHoles;
	float m_circleRadius;
	float m_forceAll_mod;

	//jcv_point* m_j_points;
};
