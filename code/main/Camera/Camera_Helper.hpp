#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include <iostream>

#define SMOOTHSTEP(x) ((x) * (x) * (3 - 2 * (x)))

static glm::vec3 VEC_ZERO{ 0.f,0.f,0.f };
static glm::vec3 VEC_UP{ 0.f,1.f,0.f };
static glm::vec3 VEC_FRONT{ 0.f,0.f,1.f };
static glm::vec3 VEC_RIGHT{ 1.f,0.f,0.f };

class Camera_Helper
{
public: 
	static glm::vec3 quatToEuler(glm::quat _quat);
	static void computeFinalView(glm::mat4& _outProjectionMatrix, glm::mat4& _outviewMatrix, glm::vec3& _position, glm::quat _rotation, float _fovDegree);

	static glm::vec3 projectVectorOnPlan(glm::vec3 vecteur, glm::vec3 normal){
		glm::vec3 tmp = cross(vecteur, normal);
		return cross(normal, vecteur);
	}

	static float clipAngle180(float angle){
		return angle > 180.f ? angle - 360.f : (angle < -180.f ? angle + 360.f : angle);
	}

	static float interpolate_smooth(float ratio){
		return(SMOOTHSTEP(ratio));
	}

};