#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "Const.hpp"
#include "vector"

using namespace glm;

namespace cgue {
	class BSpline
	{
	public:
		BSpline();
		virtual ~BSpline();

		void addPoint(vec3 point, float tDiff);
		void addLastPoint(vec3 point, float tDiff1, float tDiff2, float tDiff3);

		vec3 calcPoint(float t);
		vec3 calcDerivative(float t);

	private:

		//float T[countT] = { 0, 0, 0, 3, 4, 5, 5, 5 };
		vector<float> times;

		// P_0 to P_{n-p-2}
		//vec3 P[countT - polyGrad - 1] = { vec3(1, 0, 0), vec3(2, 0, 0), vec3(3, 1, 0), vec3(4, 0, 0), vec3(5, 0, 0) };
		vector<vec3> points;

		const int polyGrad = 2;
		int countT = 0;

		float T(int i);
		vec3 BSpline::P(int i);

		float calcDerivativeN(int i, int p, float u);
		float calcN(int i, int p, float u);
	};
}