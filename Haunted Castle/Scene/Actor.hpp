#pragma once

#include "../Resources/Geometry.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../Resources/Shader.hpp"

using namespace std;

namespace cgue
{
	class Actor : public Geometry
	{
	public:
		Actor(Shader* shader);
		virtual ~Actor();
		void resetPosition();
		void initActor();
		PxRigidDynamic* actor;
		mat4x4 getGlobalPose();

		void PxRotate(float rx, float ry, float rz);
		void PxTranslate(float tx, float ty, float tz);

	private:
	};
};