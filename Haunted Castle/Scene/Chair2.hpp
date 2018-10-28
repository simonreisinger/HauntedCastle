#include <glm/gtc/matrix_transform.hpp>
#include "Geometry.hpp"
#include "../Resources/Shader.hpp"

using namespace cgue;
using namespace std;

namespace cgue
{
	class Chair2 : public Geometry
	{
	public:
		// Constructor
		Chair2(Shader* shader, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scale);
		// Destructor
		virtual ~Chair2();
		mat4x4 getGlobalPose();

		vec3 defaultColor;
		vec3 wonColor;

	private:

	};
};