#include <glm/gtc/matrix_transform.hpp>
#include "../Resources/Geometry.hpp"
#include "../Resources/Shader.hpp"

using namespace cgue;
using namespace std;

namespace cgue
{
	class Chess : public Geometry
	{
	public:
		// Constructor
		Chess(Shader* shader);
		// Destructor
		virtual ~Chess();

	private:

	};
};