#include "SceneNode.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace cgue;
using namespace std;

void printMatrixOfNode(string name, mat4x4 mat);

SceneNode::SceneNode(aiNode* aiNode, const aiScene* scene, string modelDir, Shader* shader)
{

	name = aiNode->mName.data;
	//cout << "aiNode: " << name << endl;
	transform = convertMatrix(aiNode->mTransformation);
	transformOriginal = transform;

	size = countVertices(aiNode, scene);
	positions = new float[size * 3];
	indices = new int[size];
	normals = new float[size * 3];
	uvs = new float[size * 2];


	meshCount = aiNode->mNumMeshes;
	mesh = new Mesh*[meshCount];

	for (unsigned int i = 0; i < aiNode->mNumMeshes; i++)
	{
		int iMesh = aiNode->mMeshes[i];
		aiMesh* aiMesh = scene->mMeshes[iMesh];

		// MATERIAL
		int iMaterial = aiMesh->mMaterialIndex;
		const aiMaterial* material = scene->mMaterials[iMaterial];

		mesh[i] = new Mesh(modelDir, aiNode->mName.data, aiMesh, material, mat4x4(1), mat4x4(1), shader);
	}

	childNodeCount = aiNode->mNumChildren;
	childNode = new SceneNode*[childNodeCount];

	for (int i = 0; i < childNodeCount; i++)
	{
		childNode[i] = new SceneNode(aiNode->mChildren[i], scene, modelDir, shader);
	}

	this->init = true;
}

SceneNode::~SceneNode() {

	delete positions; positions = nullptr;
	delete normals; normals = nullptr;
	delete indices; indices = nullptr;
	delete uvs; uvs = nullptr;
	delete mesh; mesh = nullptr;
	delete childNode; childNode = nullptr;
	delete mesh; mesh = nullptr;
}

void SceneNode::draw(Shader* drawShader, mat4x4 view, glm::mat4x4 proj, mat4x4 globalPose, bool cull)
{
	if (this->init)
	{
		//cout << "DRAW " << name << endl;

		drawShader->useShader();

		for (int i = 0; i < this->meshCount; i++)
		{
			mesh[i]->draw(drawShader, view, proj, globalPose * transform, cull);
		}

		for (int i = 0; i < childNodeCount; i++)
		{
			childNode[i]->draw(drawShader, view, proj, globalPose * transform, cull);
		}
	}
}

int SceneNode::countVertices(aiNode* node, const aiScene* scene)
{
	int count = 0;
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		int iMesh = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[iMesh];
		int iMeshFaces = mesh->mNumFaces;
		for (int j = 0; j < iMeshFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++)
			{
				count++;
			}
		}
	}
	return count;
}

mat4 SceneNode::convertMatrix(const aiMatrix4x4 m)
{
	mat4 Matri;

	Matri[0][0] = m.a1;
	Matri[0][1] = m.b1;
	Matri[0][2] = m.c1;
	Matri[0][3] = m.d1;
	Matri[1][0] = m.a2;
	Matri[1][1] = m.b2;
	Matri[1][2] = m.c2;
	Matri[1][3] = m.d2;
	Matri[2][0] = m.a3;
	Matri[2][1] = m.b3;
	Matri[2][2] = m.c3;
	Matri[2][3] = m.d3;
	Matri[3][0] = m.a4;
	Matri[3][1] = m.b4;
	Matri[3][2] = m.c4;
	Matri[3][3] = m.d4;

	return Matri;
}

void printMatrixOfNode(string name, mat4x4 mat)
{
	cout << name << endl;
	cout << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << " " << mat[3][0] << endl;
	cout << mat[0][1] << " " << mat[1][1] << " " << mat[2][1] << " " << mat[3][1] << endl;
	cout << mat[0][2] << " " << mat[1][2] << " " << mat[2][2] << " " << mat[3][2] << endl;
	cout << mat[0][3] << " " << mat[1][3] << " " << mat[2][3] << " " << mat[3][3] << endl;
}

void SceneNode::translateLinear(string meshName, vec3 trans, float time_start, float duration, float time, float time_delta) {
	if (meshName.compare(name) == 0 && time >= time_start && time <= time_start + duration) {
		transform = translate(transform, trans * time_delta / duration);
		objectMoved = true;
	}

	for (int i = 0; i < childNodeCount; i++)
	{
		childNode[i]->translateLinear(meshName, trans, time_start, duration, time, time_delta);
	}
}

void SceneNode::translateGravity(string meshName, float trans_y_end, float time_start, float time, float time_delta) {
	if (meshName.compare(name) == 0 && time >= time_start && transform[3][2] > trans_y_end) {
		forces += vec3(0, 0, -5.0) * time_delta;
		transform = translate(transform, forces);
		if (transform[3][2] < trans_y_end) {
			transform[3][2] = trans_y_end;
		}
		objectMoved = true;
	}

	for (int i = 0; i < childNodeCount; i++)
	{
		childNode[i]->translateGravity(meshName, trans_y_end, time_start, time, time_delta);
	}
}

void SceneNode::rotateLinear(string meshName, vec3 rotateAxis, float rotateValue, bool exactEndValue, float time_start, float duration, float time, float time_delta) {
	if (meshName.compare(name) == 0 && time >= time_start) {
		if (time <= time_start + duration) {
			transform = glm::rotate(transform, (float)(rotateValue * M_PI * time_delta / (duration * 180.0)), rotateAxis);
			objectMoved = true;
		}
		else if(exactEndValue && time - time_delta <= time_start + duration) {
			transform = glm::rotate(transformOriginal, (float)(rotateValue * M_PI / 180.0), rotateAxis);
			objectMoved = true;
		}
	}

	for (int i = 0; i < childNodeCount; i++)
	{
		childNode[i]->rotateLinear(meshName, rotateAxis, rotateValue, exactEndValue, time_start, duration, time, time_delta);
	}
}