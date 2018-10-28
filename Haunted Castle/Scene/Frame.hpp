#include <glm/gtc/matrix_transform.hpp>
#include "Geometry.hpp"
#include "../Resources/Shader.hpp"

using namespace cgue;
using namespace std;

namespace cgue
{
	class Frame : public Geometry
	{
	public:
		// Constructor
		Frame(Shader* shader, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scale);
		// Destructor
		virtual ~Frame();
		mat4x4 getGlobalPose();

		vec3 defaultColor;
		vec3 wonColor;

	private:

	};
};