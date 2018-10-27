#pragma once

#include "SceneObject.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace cgue {
	class Camera : public SceneObject
	{
	public:
		Camera();
		virtual ~Camera();

		vec3 p;
		vec3 l;
		vec3 u;

		mat4x4 Camera::getCameraModel();
		vec3 Camera::getCameraPos();
		vec3 Camera::getCameraUp();
		vec3 Camera::getCameraLookAt();
	private:
	};
}