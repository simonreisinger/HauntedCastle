#include <glm/gtc/matrix_transform.hpp>
#include "../Resources/Geometry.hpp"
#include "../Resources/Shader.hpp"

using namespace cgue;
using namespace std;

namespace cgue
{
	class Windows : public Geometry
	{
	public:
		// Constructor
		Windows(Shader* shader);
		// Destructor
		virtual ~Windows();

	private:

	};
};