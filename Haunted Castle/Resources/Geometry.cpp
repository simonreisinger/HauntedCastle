#include "Geometry.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SceneNode.hpp"

using namespace cgue;
using namespace std;
using namespace glm;

Geometry::Geometry()
{

}

void Geometry::init(const std::string& displayFile, Shader* _shader)
{
	modelMatrix = mat4x4(1);
	shader = _shader;

	shader->useShader();

	string datensatzDir = "Datensatz/";

	string modelDir = displayFile.substr(0, displayFile.find("/"));

	//cout << "ModelDir:" << modelDir << endl;

	//cout << "ReadFile: " << datensatzDir + displayFile << endl;

	Assimp::Importer displayImporter;

	string filePath = datensatzDir + displayFile; //TODO edit here

	const aiScene* scene = displayImporter.ReadFile(filePath, aiProcessPreset_TargetRealtime_Quality);
	if (!scene)
	{
		string errorMsg = displayImporter.GetErrorString();
		std::cout << "Couldn't load model: " << errorMsg;
		glfwTerminate();
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
	
	

	aiNode* rootNode = scene->mRootNode;

	sceneNode = new SceneNode(rootNode, scene, modelDir, shader);

	iObjectsLoaded++;
	cout << "Object " << iObjectsLoaded << " of " << countObjectsLoading << " loaded" << endl;
}

void Geometry::initActor()
{

}

Geometry::~Geometry()
{
	delete sceneNode; sceneNode = nullptr;
}

void Geometry::update(float time_delta, float time_abs)
{

}

void Geometry::draw(Shader* drawShader, mat4x4 view, glm::mat4x4 proj, mat4x4 camera_model, bool cull)
{
	drawShader->useShader();

	mat4x4 globalPose = mat4x4(1);

	sceneNode->draw(drawShader, view, proj, globalPose, cull);
	
}

mat4x4 Geometry::getGlobalPose()
{
	return mat4x4(1);
}

Shader* Geometry::getShader()
{
	return shader;
}

void Geometry::setPhysX(PxPhysics* mPhysicsSDK, PxFoundation* mFoundation, PxDefaultErrorCallback mDefaultErrorCallback, PxDefaultAllocator mDefaultAllocatorCallback, PxScene* mScene)
{
	gPhysicsSDK = mPhysicsSDK;
	gFoundation = mFoundation;
	gDefaultErrorCallback = mDefaultErrorCallback;
	gDefaultAllocatorCallback = mDefaultAllocatorCallback;
	gScene = mScene;
}