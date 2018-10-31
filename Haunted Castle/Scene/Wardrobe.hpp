#include <glm/gtc/matrix_transform.hpp>
#include "../Resources/Geometry.hpp"
#include "../Resources/Shader.hpp"

using namespace cgue;
using namespace std;

namespace cgue
{
	class Wardrobe : public Geometry
	{
	public:
		// Constructor
		Wardrobe(Shader* shader);
		// Destructor
		virtual ~Wardrobe();

	private:

	};
};