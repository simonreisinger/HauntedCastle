#pragma once

#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Mesh.hpp"

using namespace glm;

namespace cgue
{
	class SceneNode
	{
	public:
		SceneNode(aiNode* aiNode, const aiScene* scene, string modelDir, Shader* shader);
		virtual ~SceneNode();

	private:

		int countVertices(aiNode* node, const aiScene* scene);

		mat4 convertMatrix(const aiMatrix4x4 m);

		int size;
		float* positions;
		float* normals;
		int* indices;
		float* uvs;

		int childNodeCount;
		SceneNode** childNode;

		int meshCount;
		Mesh** mesh;

		string name;
		mat4x4 transform;
	};
}