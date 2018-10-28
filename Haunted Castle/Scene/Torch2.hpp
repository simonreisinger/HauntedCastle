#include <glm/gtc/matrix_transform.hpp>
#include "Geometry.hpp"
#include "../Resources/Shader.hpp"

using namespace cgue;
using namespace std;

namespace cgue
{
	class Torch2 : public Geometry
	{
	public:
		// Constructor
		Torch2(Shader* shader, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scale);
		// Destructor
		virtual ~Torch2();
		mat4x4 getGlobalPose();

		vec3 defaultColor;
		vec3 wonColor;

	private:

	};
};