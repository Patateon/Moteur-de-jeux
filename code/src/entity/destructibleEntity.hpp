#pragma once

#include <glm/glm.hpp>

#include <src/entity/entity.hpp>
#include <src/entity/fractureGenerator.hpp>

class FractureGenerator;

class DestructibleEntity : public Entity
{
private:
	std::vector<glm::vec2> m_polygonFace;
	std::vector<glm::vec2> m_polygonUV;
	glm::vec2 min;
	glm::vec2 max;

	float m_lifetime = 0.0f;
	float m_scale = 0.0f;
	bool m_destroyed = false;
	int m_fractureDepth = 0;

	FractureGenerator* fractureGenRef = nullptr;

public:
	DestructibleEntity(FractureGenerator* fractureGen);
	~DestructibleEntity();

	void LoadBasic(float size);
	void LoadBasic2(float size);
	void SetPolygon(std::vector <glm::vec2> vertices);
	void SetScale(float scale);

	void setLifetime(float time) { m_lifetime = time; }
	void SetDestroyed(bool state) { m_destroyed = state; }
	void SetFractureDepth(int depth) { m_fractureDepth = depth; }

	void CalculateMinMax();
	void meshFromPolygon();

	reactphysics3d::Collider* createCollider(reactphysics3d::PhysicsCommon* physicsCommon);

	// Get
	const std::vector<glm::vec2>& GetPolygon() const { return m_polygonFace; }
	const glm::vec2& GetMin() const { return min; }
	const glm::vec2& GetMax() const { return max; }
	const float& getScale() const { return m_scale; }
	const float& getLifetime() const { return m_lifetime; }
	const int& GetFractureDepth() const { return m_fractureDepth; }
	const bool& IsDestroyed() const { return m_destroyed; }

	const std::vector<glm::vec2>& getUv() const { return m_polygonUV; }

	FractureGenerator* GetFractureGenerator() { return fractureGenRef; }
};