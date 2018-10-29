#pragma once

#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

namespace cgue
{
	class Node
	{
	public:
		Node(aiNode* aiNode, const aiScene* scene);
		virtual ~Node();

	private:

		int countVertices(aiNode* node, const aiScene* scene);

		mat4 convertMatrix(const aiMatrix4x4 m);

		int size;
		float* positions;
		float* normals;
		int* indices;
		float* uvs;
	};
}