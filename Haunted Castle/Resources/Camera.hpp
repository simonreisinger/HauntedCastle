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

		mat4x4 getCameraModel();
		mat4x4 getInverseCameraModel();
		vec3 getCameraPos();
		vec3 getCameraUp();
		vec3 getCameraLookAt();
	private:
	};
}