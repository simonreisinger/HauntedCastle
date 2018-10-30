#include <glm/gtc/type_ptr.hpp>

#include "Coordinatesystem.hpp"
#include "../Resources/Const.hpp"

using namespace glm;


Coordinatesystem::Coordinatesystem(Shader* shader, float sposX, float sposY, float sposZ, float srotX, float srotY, float srotZ, float sscale)
{
	this->posx = sposX;
	this->posy = sposY;
	this->posz = sposZ;
	this->rotx = srotX;
	this->roty = srotY;
	this->rotz = srotZ;
	this->scale = sscale;

	const std::string& displayFile = "Coordinatesystem/Coordinatesystem.dae";

	glm::mat4& matrix = glm::mat4(1.0f);

	glm::mat4& initTrans = glm::mat4(1.0f);

	Geometry::init(displayFile, matrix, shader, initTrans);

	// Shader
	shader->useShader();
}

Coordinatesystem::~Coordinatesystem()
{
	// TODO delete data??
}

mat4x4 Coordinatesystem::getGlobalPose()
{
	return mat4x4(1);
}

