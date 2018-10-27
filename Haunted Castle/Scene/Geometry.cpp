#include "Geometry.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



using namespace cgue;
using namespace std;
using namespace glm;


void printMatGeometry(string name, mat4x4 mat);
mat4 convertMetrix(const aiMatrix4x4 m);

int iVertices = 0;

void Geometry::searchNodesRecursive(string modelDir, aiNode* node, const aiScene* scene, mat4x4 initTrans, float* positionsPointer, float* normalsPointer, int* indicesPointer, float* uvsPointer, Mesh** meshPointer, Shader* shader)
{
	aiString name = node->mName;
	mat4x4 transform = convertMetrix(node->mTransformation);

	//cout << "NODE: " << name.data << endl;
	//printMatGeometry(name.data, transform);
	
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		int iMesh = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[iMesh];

		// MATERIAL
		int iMaterial = mesh->mMaterialIndex;
		const aiMaterial* material = scene->mMaterials[iMaterial];

		meshPointer[meshIndex++] = new Mesh(modelDir, name.data, mesh, material, initTrans, transform, shader);


	}

	//cout << endl;

	for (int i = 0; i < node->mNumChildren; i++)
	{
		searchNodesRecursive(modelDir, node->mChildren[i], scene, initTrans, positionsPointer, normalsPointer, indicesPointer, uvsPointer, meshPointer, shader);
	}
}

int countVerticesRecursive(aiNode* node, const aiScene* scene)
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
	for (int i = 0; i < node->mNumChildren; i++)
	{
		count += countVerticesRecursive(node->mChildren[i], scene);
	}
	return count;
}


Geometry::Geometry()
{

}

void Geometry::init(const std::string& displayFile, mat4& matrix, Shader* _shader, mat4x4 initTrans) {

	modelMatrix = matrix;
	shader = _shader;

	string datensatzDir = "Datensatz/";

	string modelDir = displayFile.substr(0, displayFile.find("/"));

	cout << "ModelDir:" << modelDir << endl;

	Assimp::Importer displayImporter;
	const aiScene* scene = displayImporter.ReadFile(datensatzDir + displayFile, aiProcessPreset_TargetRealtime_Quality);
	if (!scene)
	{
		std::cout << "Couldn't load model ";
		glfwTerminate();
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
	
	

	aiNode* rootNode = scene->mRootNode;

	// Notiz: Mit Blender ist wird jeder Eckpunkt 6-fach gespeichert, 2-mal je Seite
	size = countVerticesRecursive(rootNode, scene);
	positions = new float[size * 3];
	indices = new int[size];
	normals = new float[size * 3];
	uvs = new float[size * 2];


	meshCount = scene->mNumMeshes;
	mesh = new Mesh*[meshCount];
	

	iVertices = 0;
	meshIndex = 0;
	Geometry::searchNodesRecursive(modelDir, rootNode, scene, initTrans, positions, normals, indices, uvs, mesh, shader);
}

void Geometry::initActor()
{

}

Geometry::~Geometry()
{
	
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &normalsBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteVertexArrays(1, &vao);
	
	delete positions; positions = nullptr;
	delete normals; normals = nullptr;
	delete indices; indices = nullptr;
	delete uvs; uvs = nullptr;
	delete mesh; mesh = nullptr;
}

void Geometry::update(float time_delta)
{
	for (int i = 0; i < this->meshCount; i++)
	{
		mesh[i]->update(time_delta);
	}
}

void Geometry::draw(Shader* drawShader, mat4x4 view, glm::mat4x4 proj, mat4x4 camera_model, bool cull)
{
	if (!enableDraw) return;

	drawShader->useShader();

	auto enableLighting_location = glGetUniformLocation(drawShader->programHandle, "enableLighting");
	glUniform1i(enableLighting_location, enableLighting);

	auto enableGlow_location = glGetUniformLocation(drawShader->programHandle, "enableGlow");
	glUniform1i(enableGlow_location, enableGlow);

	auto alpha_location = glGetUniformLocation(drawShader->programHandle, "alpha");
	glUniform1f(alpha_location, alpha);

	auto transparency_location = glGetUniformLocation(drawShader->programHandle, "enableTransparency");
	glUniform1i(transparency_location, TRANSPARENCY ? 1 : 0);

	mat4x4 globalPose = getGlobalPose();

	for (int i = 0; i < meshCount; i++)
	{
		mesh[i]->draw(drawShader, view, proj, globalPose, cull);
	}
}

mat4x4 Geometry::getGlobalPose()
{
	return mat4x4(0.0f);
}

Shader* Geometry::getShader()
{
	return shader;
}

mat4 convertMetrix(const aiMatrix4x4 m)
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


void Geometry::setPhysX(PxPhysics* mPhysicsSDK, PxFoundation* mFoundation, PxDefaultErrorCallback mDefaultErrorCallback, PxDefaultAllocator mDefaultAllocatorCallback, PxScene* mScene)
{
	gPhysicsSDK = mPhysicsSDK;
	gFoundation = mFoundation;
	gDefaultErrorCallback = mDefaultErrorCallback;
	gDefaultAllocatorCallback = mDefaultAllocatorCallback;
	gScene = mScene;

	for (int i = 0; i < meshCount; i++) {
		mesh[i]->setPhysX(gPhysicsSDK, gFoundation, gDefaultErrorCallback, gDefaultAllocatorCallback);
	}
}