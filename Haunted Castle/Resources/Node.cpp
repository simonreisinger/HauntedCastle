#include "Node.hpp"

using namespace cgue;
using namespace std;

void printMatrix(string name, mat4x4 mat);

Node::Node(aiNode* aiNode, const aiScene* scene) {

	aiString name = aiNode->mName;
	mat4x4 transform = convertMatrix(aiNode->mTransformation);

	printMatrix(name.data, transform);

	size = countVertices(aiNode, scene);
	positions = new float[size * 3];
	indices = new int[size];
	normals = new float[size * 3];
	uvs = new float[size * 2];

	cout << "aiNode->mNumMeshes:" << aiNode->mNumMeshes << endl;
}

Node::~Node() {

	delete positions; positions = nullptr;
	delete normals; normals = nullptr;
	delete indices; indices = nullptr;
	delete uvs; uvs = nullptr;
}

int Node::countVertices(aiNode* node, const aiScene* scene)
{
	int count = 0;
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		int iMesh = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[iMesh];
		int iMeshFaces = mesh->mNumFaces;
		for (int j = 0; j < iMeshFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];
			for (int k = 0; k < face.mNumIndices; k++)
			{
				count++;
			}
		}
	}
	return count;
}

mat4 Node::convertMatrix(const aiMatrix4x4 m)
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

void printMatrix(string name, mat4x4 mat)
{
	cout << name << endl;
	cout << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << " " << mat[0][3] << endl;
	cout << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << " " << mat[1][3] << endl;
	cout << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << " " << mat[2][3] << endl;
	cout << mat[3][0] << " " << mat[3][1] << " " << mat[3][2] << " " << mat[3][3] << endl;
}