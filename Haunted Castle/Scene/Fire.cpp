#include <PhysX\PxPhysicsAPI.h>		//Single header file to include all features of PhysX API 
#include <glm/gtc/type_ptr.hpp>

#include "Fire.hpp"
#include "../Resources/Const.hpp"


Fire::Fire(Shader* shader, vec3 flame_pos, vec3 flame_dir) {
	

	flamePos = flame_pos;
	flameDir = flame_dir;

	computeShader = new Shader("Shader/FireParticle.comp");

	computeShader->useShader();

	// i = [0,1]
	for (int i = 0; i <= 1; i++)
	{
		glGenBuffers(1, &ssbo_pos[i]);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pos[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES *
			sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &ssbo_vel[i]);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vel[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES *
			sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

	}

	glGenBuffers(1, &atomic_counter);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL,
		GL_DYNAMIC_DRAW);
	GLuint value = 0;
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &value);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

	// Because of a performance warning when reading the atomic counter, we
	// create a buffer to move-to and read-from instead.
	glGenBuffers(1, &temp_buffer);
	glBindBuffer(GL_COPY_WRITE_BUFFER, temp_buffer);
	glBufferData(GL_COPY_WRITE_BUFFER, sizeof(GLuint), NULL,
		GL_DYNAMIC_READ);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);



	vector<vec4> positions;
	vector<vec4> velocities;
	positions.push_back(vec4(0, 0, 0, 1));
	velocities.push_back(vec4(0, 1, 0, 1));
	particle_count = positions.size();

	// copy the data to the SSBO:
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pos[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
		particle_count * sizeof(positions[0]), &positions[0]);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vel[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
		particle_count * sizeof(velocities[0]), &velocities[0]);



	const GLuint position_layout = 0;
	glGenVertexArrays(2, vaos);
	// i = [0,1]
	for (int i = 0; i <= 1; i++)
	{
		glBindVertexArray(vaos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, ssbo_pos[i]);
		glEnableVertexAttribArray(position_layout);
		glVertexAttribPointer(position_layout, 4, GL_FLOAT, GL_FALSE, 0, 0);
	}



	particleShader = new Shader("Shader/FireParticle.vert", "Shader/FireParticle.frag", "Shader/FireParticle.geom");

	particleShader->useShader();

	texture = new Texture("fire", "sprite-explosion-3.jpg");

	

	// Unbind VAO
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

void Fire::calculate(double deltaTime)
{
	//cout << "Start calculate" << endl;

	GLuint programID = computeShader->programHandle;
	glUseProgram(programID);
	
	// set all uniforms, like deltaTime, current particle count, ...
	auto LastCount_location = glGetUniformLocation(programID, "LastCount");
	glUniform1ui(LastCount_location, particle_count);

	auto MaximumCount_location = glGetUniformLocation(programID, "MaximumCount");
	glUniform1ui(MaximumCount_location, MAX_PARTICLES);
	
	auto DeltaT_location = glGetUniformLocation(programID, "DeltaT");
	glUniform1f(DeltaT_location, deltaTime);

	auto flameTop_location = glGetUniformLocation(programID, "flameTop");
	glUniform3f(flameTop_location, flameTop.x, flameTop.y, flameTop.z);

	auto flamePos_location = glGetUniformLocation(programID, "flamePos");
	glUniform3f(flamePos_location, flamePos.x, flamePos.y, flamePos.z);

	auto flameDir_location = glGetUniformLocation(programID, "flameDir");
	glUniform3f(flameDir_location, flameDir.x, flameDir.y, flameDir.z);


	particles_to_spawn += spawnRatePerSecond * deltaTime;
	GLuint spawnCount = 0;
	if (particles_to_spawn > 0) {
		spawnCount += (GLuint)particles_to_spawn;
		particles_to_spawn -= spawnCount;
	}
	auto spawnCount_location = glGetUniformLocation(programID, "spawnCount");
	glUniform1ui(spawnCount_location, spawnCount);
	
	
	// set SSBO and atomic counters...
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_pos[iSSBO]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo_vel[iSSBO]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo_pos[!iSSBO]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo_vel[!iSSBO]);

	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, atomic_counter);
	
	iSSBO = !iSSBO; // ping-pong between buffers
	// Execute compute shader with a 16 x 16 work group size
	GLuint groups = (particle_count / (16 * 16)) + 1;
	glDispatchCompute(groups, 1, 1);
	
	
	glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);

	// Read atomic counter through a temporary buffer
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter);
	glBindBuffer(GL_COPY_WRITE_BUFFER, temp_buffer);
	// from atomic counter to temp buffer:
	glCopyBufferSubData(GL_ATOMIC_COUNTER_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
		sizeof(GLuint));
	GLuint *counterValue = (GLuint*)glMapBufferRange(GL_COPY_WRITE_BUFFER, 0,
		sizeof(GLuint), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
	particle_count = counterValue[0];
	//cout << "Particles: " << particle_count << endl;
	counterValue[0] = 0; // reset atomic counter in temp buffer
	glUnmapBuffer(GL_COPY_WRITE_BUFFER); // stop writing to temp buffer
	// copy temp buffer to atomic counter:
	glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_ATOMIC_COUNTER_BUFFER, 0, 0,
		sizeof(GLuint));
	// memory barrier, to make sure everything from the compute shader is written
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);


	

	//cout << "particle_count: " << particle_count << endl;

	//cout << "End calculate" << endl;
}

void Fire::draw(mat4x4 view, mat4x4 proj)
{
	glEnable(GL_BLEND); // activate blending
	glDepthMask(GL_FALSE); // disable writing to depth buffer
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	particleShader->useShader(); // program with VS -> GS -> FS



	GLuint programID = particleShader->programHandle;
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

	glBindVertexArray(vaos[iSSBO]); // bind VAO
	glDrawArrays(GL_POINTS, 0, particle_count);
	glBindVertexArray(0);
	glUseProgram(0);

	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Fire::drawParticles(float delta, mat4x4 view, mat4x4 proj)
{

	calculate(delta);

	draw(view, proj);

}