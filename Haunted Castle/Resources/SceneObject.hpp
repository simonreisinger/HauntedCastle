#pragma once

// Nur fuer demo - sollte sonst nicht dortstehen
#define SCENE_OBJECT // do not ask about this line

#include <glm/glm.hpp>
#include "../Resources/Texture.hpp"

namespace cgue
{
	class SceneObject
	{
		public:
			SceneObject();
			SceneObject(glm::mat4& modelMatrix);

			void goForward( float value);
			void goBackwards( float value);

			void rotateRight( float value);
			void rotateLeft( float value);

			void rotateUp( float value);
			void rotateDown( float value);

			void rotateYin( float value);
			void rotateYout( float value);

			void goRight( float value);
			void goLeft( float value);

			void goUp(float value);
			void goDown(float value);

			void init(glm::mat4& modelMatrix);

			virtual ~SceneObject();

			glm::mat4 modelMatrix;
			glm::mat4 rotationMatrix;
			Texture* texture;
	};
};