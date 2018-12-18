#include <PhysX\PxPhysicsAPI.h>		//Single header file to include all features of PhysX API 
#include <glm/gtc/type_ptr.hpp>

#include "Fire.hpp"
#include "../Resources/Const.hpp"

unsigned int VBO, VAO;

float points[] = {
	-0.5f, 5.5f, 10.0f, 0.0f, 0.0f, // top-left
	 0.5f, 5.5f, 0.0f, 1.0f, 0.0f, // top-right
	 0.5f, 4.5f, 0.0f, 0.0f, 1.0f, // bottom-right
	-0.5f, 4.5f, 1.0f, 1.0f, 0.0f  // bottom-left
};

Fire::Fire(Shader* shader, float sposX, float sposY, float sposZ,
							   float srotX, float srotY, float srotZ) {
	

	particleShader = new Shader("Shader/FireParticle.vert", "Shader/FireParticle.frag", "Shader/FireParticle.geom");

	


	texture = new Texture("fire", "fire-gray.jpg");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STREAM_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

}

Fire::~Fire()
{
	delete particleShader; particleShader = nullptr;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Fire::drawParticles(float delta, mat4x4 view, mat4x4 proj)
{

	GLuint programID = particleShader->programHandle;
	glUseProgram(programID);

	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");

	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view[0][0]);



	GLuint ViewProjectionMatrixID = glGetUniformLocation(programID, "VP");

	glm::mat4 ViewProjectionMatrix = proj * view;

	glUniformMatrix4fv(ViewProjectionMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);



	texture->bind(4);

	auto fireTexture_location = glGetUniformLocation(programID, "fireTexture");
	glUniform1i(fireTexture_location, 4);


	glm::mat4 ViewMatrix = view;
	glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);


	GLuint CameraRight_worldspace_ID = glGetUniformLocation(programID, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(programID, "CameraUp_worldspace");


	glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);


	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 4);

}