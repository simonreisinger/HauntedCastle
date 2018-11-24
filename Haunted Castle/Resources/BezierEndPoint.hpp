#pragma once

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace cgue {
	class BezierEndPoint
	{
	public:
		BezierEndPoint(vec3 point, vec3 direction, float distPointBefore, float distPointAfter);
		virtual ~BezierEndPoint();

		vec3 getPointBefore();
		vec3 getPointAfter();
		vec3 getPoint();
		vec3 getDir();

	private:
		vec3 point;
		vec3 direction;
		float distPointBefore;
		float distPointAfter;
	};
}