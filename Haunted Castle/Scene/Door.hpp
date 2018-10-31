#include <glm/gtc/matrix_transform.hpp>
#include "../Resources/Geometry.hpp"
#include "../Resources/Shader.hpp"

using namespace cgue;
using namespace std;

namespace cgue
{
	class Door : public Geometry
	{
	public:
		// Constructor
		Door(Shader* shader);
		// Destructor
		virtual ~Door();

	private:

	};
};