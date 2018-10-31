#include <glm/gtc/matrix_transform.hpp>
#include "../Resources/Geometry.hpp"
#include "../Resources/Shader.hpp"

using namespace cgue;
using namespace std;

namespace cgue
{
	class Desk : public Geometry
	{
	public:
		// Constructor
		Desk(Shader* shader);
		// Destructor
		virtual ~Desk();

	private:

	};
};