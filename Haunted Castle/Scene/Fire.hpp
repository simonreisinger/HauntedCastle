#include <glm/gtc/matrix_transform.hpp>

#include "../Resources/Geometry.hpp"
#include "../Resources/Shader.hpp"

#include <vector>

using namespace cgue;
using namespace std;

namespace cgue
{
	class Fire
	{
	public:
		// Constructor
		Fire(Shader* shader, float posX, float posY, float posZ, float srotX, float srotY, float srotZ);
		// Destructor
		virtual ~Fire();

		void drawParticles(float time_delta, mat4x4 view, mat4x4 proj);
	private:

		Shader* particleShader;
		Shader* computeShader;

		Texture* texture;

		int iSSBO = 0;

		GLuint particles_position_buffer, VAO;
			
		GLuint ssbo_pos[2];
		GLuint ssbo_vel[2];
		GLuint atomic_counter;
		GLuint temp_buffer;

		GLuint vaos[2]; // one VAO for each SSBO

		int particle_count;

		void calculate(double deltaTime);

		void draw(mat4x4 view, mat4x4 proj);

		const float TTL = 1.0f; // time-to-live in seconds
		const float spawnRatePerSecond = 100.0f;
		float particles_to_spawn = 0.0f;
	};
}