#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "Const.hpp"

using namespace glm;

namespace cgue {
	class CameraPoint
	{
	public:
		CameraPoint(vec3 point, vec3 derivative);
		virtual ~CameraPoint();

		vec3 getPoint();
		vec3 getDerivative();

	private:
		vec3 point;
		vec3 derivative;
	};
}