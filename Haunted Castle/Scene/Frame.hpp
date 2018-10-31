#include <glm/gtc/matrix_transform.hpp>
#include "../Resources/Geometry.hpp"
#include "../Resources/Shader.hpp"

using namespace cgue;
using namespace std;

namespace cgue
{
	class Frame : public Geometry
	{
	public:
		// Constructor
		Frame(Shader* shader);
		// Destructor
		virtual ~Frame();

	private:

	};
};