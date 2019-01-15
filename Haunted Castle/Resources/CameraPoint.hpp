#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "Const.hpp"

using namespace glm;

namespace cgue {
	class CameraPoint
	{
	public:
		CameraPoint(vec3 point, vec3 derivative);
		CameraPoint(vec3 point, vec3 derivative, vec3 curveDerivative);
		CameraPoint(vec3 point, vec3 derivative, float pause);
		CameraPoint(vec3 point, vec3 derivative, vec3 curveDerivative, float pause);
		virtual ~CameraPoint();

		vec3 getPoint();
		vec3 getDerivative();
		vec3 getCurveDerivative();
		float getPause();

	private:
		vec3 point;
		vec3 derivative;
		vec3 curveDerivative;
		float pause;
	};
}