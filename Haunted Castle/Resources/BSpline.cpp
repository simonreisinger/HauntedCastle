#include "BSpline.hpp"

using namespace cgue;
using namespace glm;

BSpline::BSpline()
{

}

BSpline::~BSpline() {}

void BSpline::addPoint(vec3 point, float tDiff)
{
	points.push_back(point);
	if (times.size() == 0) {
		times.push_back(tDiff);
	}
	else {
		times.push_back(times.back() + tDiff);
	}
	countT++;
}

void BSpline::addLastPoint(vec3 point, float tDiff1, float tDiff2, float tDiff3)
{
	points.push_back(points.back());
	//points.push_back(point);
	times.push_back(times.back() + tDiff1);
	times.push_back(times.back() + tDiff2);
	times.push_back(times.back() + tDiff3);
	countT += 3;
	/*
	cout << "times:" << endl;
	for (int i = 0; i < times.size(); i++) {
		cout << times.at(i) << endl;
	}
	cout << endl;
	*/
}

float BSpline::T(int i)
{
	return times.at(i);
}

vec3 BSpline::P(int i)
{
	return points.at(i);
}

vec3 BSpline::calcPoint(float t)
{
	float u = t + (float)T(polyGrad);
	//cout << "u = " << u << " ";
	//cout << "T(countT - polyGrad - 1) = " << T(countT - polyGrad - 1) << " ";
	if (u >= T(countT - polyGrad - 1))
	{
		u = T(countT - polyGrad - 1) - 0.001f;
		//cout << "Overflow";
	}

	vec3 C = vec3(0);
	for (int i = 0; i <= countT - polyGrad - 2; i++) {
		float PN = calcN(i, polyGrad, u);

		C += P(i) * PN;
	}

	//cout << "C: " << C.x << ", " << C.y << ", " << C.z << " " << endl;
	return C;
}

vec3 BSpline::calcDerivative(float t)
{
	float u = t + T(polyGrad);
	if (u >= T(countT - polyGrad - 1))
	{
		u = T(countT - polyGrad - 1) - 0.001f;
	}

	//cout << "u = " << u << " ";
	vec3 D = vec3(0);
	for (int i = 0; i <= countT - polyGrad - 2; i++) {
		float DN = calcDerivativeN(i, polyGrad, u);

		D += P(i) * DN;
	}
	if (D.x != 0 || D.y != 0 || D.z != 0) {
		D = normalize(D);
	}
	else {
		D = vec3(99, 0, 0);
	}

	//cout << "D: " << D.x << ", " << D.y << ", " << D.z << endl;
	return D;
}

float BSpline::calcN(int i, int p, float u) {
	if (p <= 0) {
		if (u >= T(i) && u < T(i + 1)) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		float N = 0;

		if (T(i + p) != T(i)) {
			N += (u - T(i)) / (T(i + p) - T(i)) * calcN(i, p - 1, u);
		}

		if (T(i + p + 1) != T(i + 1)) {
			N += (T(i + p + 1) - u) / (T(i + p + 1) - T(i + 1)) * calcN(i + 1, p - 1, u);
		}

		return N;
	}
}

float BSpline::calcDerivativeN(int i, int p, float u) {
	float D = 0;

	if (T(i + p) != T(i)) {
		D += (p / (T(i + p) - T(i))) * calcN(i, p - 1, u);
	}

	if (T(i + p + 1) != T(i + 1)) {
		D -= (p / (T(i + p + 1) - T(i + 1))) * calcN(i + 1, p - 1, u);
	}

	return D;
}