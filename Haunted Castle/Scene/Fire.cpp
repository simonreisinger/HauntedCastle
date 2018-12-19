#include <PhysX\PxPhysicsAPI.h>		//Single header file to include all features of PhysX API 
#include <glm/gtc/type_ptr.hpp>

#include "Fire.hpp"
#include "../Resources/Const.hpp"

unsigned int particles_position_buffer, VAO;

// CPU representation of a particle
struct Particle {
	glm::vec3 pos, dir;
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f
	float life; // Remaining life of the particle. if <0 : dead and unused.

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

const int MaxParticles = 1000;
Particle ParticlesContainer[MaxParticles];
int LastUsedParticle = 0;

vec3 flameTop = vec3(0, 5, 0);

// Finds a Particle in ParticlesContainer which isn't used yet.
// (i.e. life < 0);
int FindUnusedParticle() {

	for (int i = LastUsedParticle; i < MaxParticles; i++) {
		if (ParticlesContainer[i].life < 0.0f) {
			LastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < LastUsedParticle; i++) {
		if (ParticlesContainer[i].life < 0.0f) {
			LastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void SortParticles() {
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}

float particlePos[MaxParticles*3];

float randFloat(float LO, float HI)
{
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}

float randPos()
{
	return (rand() % 20 - 10) / 100.0f;
}

void initParticle(int particleIndex)
{
	float u1 = randFloat(0.0f, 1.0f);
	float u2 = randFloat(0.0f, 1.0f);

	float rand_x = sqrt(-2 * log(u1)) * cos(2 * PI * u2) / 10.0f;
	float rand_z = sqrt(-2 * log(u1)) * sin(2 * PI * u2) / 10.0f;
	float rand_y = 1;// ((rand() % 10) / 10.0f) + 0.5f;
	ParticlesContainer[particleIndex].pos = vec3(rand_x, 0, rand_z) + vec3(0, 3, 0);
	ParticlesContainer[particleIndex].dir = normalize(vec3(rand_x, rand_y, rand_z));
	ParticlesContainer[particleIndex].life = randFloat(0.5f, 1.0f);
}

Fire::Fire(Shader* shader, float sposX, float sposY, float sposZ,
							   float srotX, float srotY, float srotZ) {
	

	
	for (int i = 0; i < MaxParticles; i++) {
		int particleIndex = i;
		ParticlesContainer[particleIndex].life = -1.0f;
	}
	

	particleShader = new Shader("Shader/FireParticle.vert", "Shader/FireParticle.frag", "Shader/FireParticle.geom");

	texture = new Texture("fire", "particle.png");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particlePos), NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindVertexArray(0);


}

Fire::~Fire()
{
	delete particleShader; particleShader = nullptr;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &particles_position_buffer);
}

int ParticlesCount = 0;

float deltaSum = 0.0f;
float creationThreshold = 0.001f;

void updateParticles(float delta, vec3 CameraPosition)
{
	deltaSum += delta;


	while (deltaSum > creationThreshold) {
		int particleIndex = FindUnusedParticle();
		initParticle(particleIndex);

		deltaSum -= creationThreshold;
	}

	ParticlesCount = 0;
	for (int i = 0; i < MaxParticles; i++) {
		Particle& p = ParticlesContainer[i]; // shortcut

		if (p.life > 0.0f) {

			// Decrease life
			p.life -= delta;
			if (p.life > 0.0f) {

				vec3 dirCorr = normalize(flameTop - p.pos);

				p.dir = normalize(p.dir + 0.05f * dirCorr);

				p.pos += p.dir * delta;

				p.cameradistance = glm::length(p.pos - CameraPosition);

				particlePos[3 * ParticlesCount + 0] = p.pos.x;
				particlePos[3 * ParticlesCount + 1] = p.pos.y;
				particlePos[3 * ParticlesCount + 2] = p.pos.z;

				ParticlesCount++;
			}
			else {
				// Died particles
				p.cameradistance = -1.0f;
			}
		}
	}

	SortParticles();
}

void Fire::drawParticles(float delta, mat4x4 view, mat4x4 proj)
{

	glEnable(GL_BLEND); // activate blending
	glDepthMask(GL_FALSE); // disable writing to depth buffer
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendEquation(GL_MAX);

	GLuint programID = particleShader->programHandle;
	glUseProgram(programID);

	// Transform
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view[0][0]);

	GLuint ViewProjectionMatrixID = glGetUniformLocation(programID, "VP");
	glm::mat4 ViewProjectionMatrix = proj * view;

	glUniformMatrix4fv(ViewProjectionMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

	glm::mat4 ViewMatrix = view;
	glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);

	GLuint CameraRight_worldspace_ID = glGetUniformLocation(programID, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(programID, "CameraUp_worldspace");

	// Texture
	texture->bind(4);
	auto fireTexture_location = glGetUniformLocation(programID, "fireTexture");
	glUniform1i(fireTexture_location, 4);


	glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);



	updateParticles(delta, CameraPosition);



	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particlePos), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(particlePos), particlePos);

	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, ParticlesCount);


	//glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}