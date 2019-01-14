#include "Geometry.hpp"
#include "SceneNode.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace cgue;
using namespace std;
using namespace glm;

Geometry::Geometry()
{

}

void Geometry::init(const std::string& displayFile, Shader* _shader)
{
	auto time_start = glfwGetTime();

	modelMatrix = mat4x4(1);
	shader = _shader;

	shader->useShader();

	string datensatzDir = "Datensatz/";

	string modelDir = displayFile.substr(0, displayFile.find("/"));

	Assimp::Importer displayImporter;

	string filePath = datensatzDir + displayFile;

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

	auto time_end = glfwGetTime();

	iObjectsLoaded++;
	cout << "Object " << iObjectsLoaded << " of " << countObjectsLoading << " loaded (Model: " << modelDir << ", Time: " << time_end - time_start << "s)" << endl;
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

void Geometry::renderGeometry(Shader* drawShader, mat4x4 view, glm::mat4x4 proj, mat4x4 camera_model, bool cull)
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

void Geometry::translateLinear(string meshName, vec3 t, float time_start, float duration, float time, float time_delta) {
	sceneNode->translateLinear(meshName, t, time_start, duration, time, time_delta);
}

void Geometry::translateGravity(string meshName, float trans_y, float time_start, float time, float time_delta) {
	sceneNode->translateGravity(meshName, trans_y, time_start, time, time_delta);
}

void Geometry::rotateLinear(string meshName, vec3 rotateAxis, float rotateValue, bool exactEndValue, float time_start, float duration, float time, float time_delta){
	sceneNode->rotateLinear(meshName, rotateAxis, rotateValue, exactEndValue, time_start, duration, time, time_delta);
}