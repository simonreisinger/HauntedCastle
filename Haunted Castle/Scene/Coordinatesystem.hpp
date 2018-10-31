#include <glm/gtc/matrix_transform.hpp>
#include "../Resources/Geometry.hpp"
#include "../Resources/Shader.hpp"

using namespace cgue;
using namespace std;

namespace cgue
{
	class Coordinatesystem : public Geometry
	{
	public:
		// Constructor
		Coordinatesystem(Shader* shader);
		// Destructor
		virtual ~Coordinatesystem();

	private:

	};
};