#include "SceneObject.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace cgue;
using namespace glm;


SceneObject::SceneObject()
{
	rotationMatrix = glm::mat4x4(1.0);
}

SceneObject::SceneObject(glm::mat4& _modelMatrix)
: modelMatrix(_modelMatrix)
{

}

SceneObject::~SceneObject()
{

}

void SceneObject::init(glm::mat4& _modelMatrix)
{
	modelMatrix = _modelMatrix;
}


void SceneObject::rotateUp(float value){
	glm::vec3 axis = vec3(rotationMatrix * vec4(1, 0, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, 1.0f * value, axis);
	//modelMatrix = glm::rotate(modelMatrix, 1.0f * value, vec3(1, 0, 0));
}
void SceneObject::rotateDown(float value){
	glm::vec3 axis = vec3(rotationMatrix * vec4(1, 0, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, -1.0f * value, axis);
	//modelMatrix = glm::rotate(modelMatrix, -1.0f * value, vec3(1, 0, 0));
}

void SceneObject::rotateRight(float value){
	modelMatrix = glm::rotate(modelMatrix, 1.0f * value, glm::vec3(0, 1, 0));
	rotationMatrix = glm::rotate(rotationMatrix, -1.0f * value, glm::vec3(0, 1, 0));
}
void SceneObject::rotateLeft(float value){
	modelMatrix = glm::rotate(modelMatrix, -1.0f * value, glm::vec3(0, 1, 0));
	rotationMatrix = glm::rotate(rotationMatrix, 1.0f * value, glm::vec3(0, 1, 0));
}
void SceneObject::goForward(float value){
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -1.0f * value));
}

void SceneObject::goBackwards(float value){
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 1.0f * value));
}

void SceneObject::goLeft(float value){
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.0f * value, 0, 0));
}

void SceneObject::goRight(float value){
	modelMatrix = glm::translate(modelMatrix, glm::vec3(1.0f * value,  0, 0));
}

void SceneObject::goUp(float value){
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.0f * value, 0));
}

void SceneObject::goDown(float value){
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -1.0f * value, 0));
}

