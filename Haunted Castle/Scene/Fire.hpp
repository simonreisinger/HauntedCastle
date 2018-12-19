#include <glm/gtc/matrix_transform.hpp>

#include "../Resources/Geometry.hpp"
#include "../Resources/Shader.hpp"

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

		Texture* texture;
	};
}