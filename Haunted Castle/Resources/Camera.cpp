#include "Camera.hpp"

using namespace cgue;
using namespace glm;

Camera::Camera(){
	//							CamPos			BallonPos			UP
	modelMatrix = mat4x4(1.0);// = glm::lookAt(vec3(0, 0, 15), vec3(0, 0, 0), vec3(0, 1, 0));
	//modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -15));
}

Camera::~Camera(){}

mat4x4 Camera::getCameraModel()
{
	//return modelMatrix;
	//return glm::translate(mat4x4(1.0f), glm::vec3(0, 0, -15)) * modelMatrix;
	return lookAt(getCameraPos(), getCameraLookAt(), getCameraUp());
}

vec3 Camera::getCameraPos()
{
	vec4 v = vec4(0, 0, 0.1, 1) * modelMatrix;
	return vec3(v.x, v.y, v.z);
}

vec3 Camera::getCameraLookAt()
{
	return vec3(0, 0, 0);
}


vec3 Camera::getCameraUp()
{
	vec4 v = vec4(0, -1, 0, 1) * modelMatrix;
	return vec3(v.x, v.y, v.z);
}