#include <glm/gtc/matrix_transform.hpp>
#include "../Resources/Geometry.hpp"
#include "../Resources/Shader.hpp"

using namespace cgue;
using namespace std;

namespace cgue
{
	class Room : public Geometry
	{
	public:
		// Constructor
		Room(Shader* shader);
		// Destructor
		virtual ~Room();

	private:

	};
};