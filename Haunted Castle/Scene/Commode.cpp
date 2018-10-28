#include <glm/gtc/type_ptr.hpp>

#include "Commode.hpp"
#include "../Resources/Const.hpp"

using namespace glm;


Commode::Commode(Shader* shader, float sposX, float sposY, float sposZ, float srotX, float srotY, float srotZ, float sscale)
{
	this->posx = sposX;
	this->posy = sposY;
	this->posz = sposZ;
	this->rotx = srotX;
	this->roty = srotY;
	this->rotz = srotZ;
	this->scale = sscale;

	const std::string& displayFile = "commode/commode.dae";

	glm::mat4& matrix = glm::mat4(1.0f);

	glm::mat4& initTrans = glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(1, 0, 0));
	initTrans = glm::rotate(initTrans, srotY / 180.0f * PI, glm::vec3(0, 0, 1));

	Geometry::init(displayFile, matrix, shader, initTrans);

	// Shader
	shader->useShader();
	enableLighting = 1;
	enableDraw = true;
	alpha = 1;

	defaultColor = vec3(1, 1, 1);
	mesh[0]->diffuse = defaultColor;
	wonColor;
}

Commode::~Commode()
{
	// TODO delete data??
}

mat4x4 Commode::getGlobalPose()
{
	mat4x4 mScale = glm::scale(mat4x4(1), vec3(this->scale));
	mat4x4 mRotX = glm::rotate(mScale, this->rotx, vec3(1, 0, 0));
	mat4x4 mRotY = glm::rotate(mRotX, this->roty, vec3(0, 1, 0));
	mat4x4 mRotZ = glm::rotate(mRotY, this->rotz, vec3(0, 0, 1));
	return translate(mRotZ, vec3(this->posx, this->posy, this->posz));
}

