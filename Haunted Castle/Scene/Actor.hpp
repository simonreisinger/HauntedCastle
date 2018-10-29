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
		Actor(Shader* shader, float sposx, float sposy, float sposz, float sroty);
		glm::vec3 getPosition();
		Actor(Shader* shader);
		virtual ~Actor();
		void resetPosition();
		void initActor();
		PxRigidDynamic* actor;
		mat4x4 getGlobalPose();

		float extraSpeed = 1.0f;

		// if factor > 1 actor excelerate
		// if factor = 1 speed of actor stays the same
		// if factor < 1 and > 0 actor slows down
		// negativ values are illegal
		void excelerateSpeed(float factor);

		// returns the extraspeed of the Object
		// return value > 0 
		float getExtraSpeed();

	private:
	};
};