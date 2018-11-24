#include "BezierEndPoint.hpp"

using namespace cgue;
using namespace glm;

BezierEndPoint::BezierEndPoint(vec3 point, vec3 direction, float distPointBefore, float distPointAfter)
{
	this->point = point;
	this->direction = direction;
	this->distPointBefore = distPointBefore;
	this->distPointAfter = distPointAfter;
}

BezierEndPoint::~BezierEndPoint() {}

vec3 BezierEndPoint::getPointBefore()
{
	return point - distPointBefore * direction;
}

vec3 BezierEndPoint::getPointAfter()
{
	return point + distPointAfter * direction;
}

vec3 BezierEndPoint::getPoint()
{
	return point;
}

vec3 BezierEndPoint::getDir()
{
	return direction;
}