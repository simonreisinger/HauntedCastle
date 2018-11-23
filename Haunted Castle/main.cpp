#include <iostream>
#include <string>
#include <sstream> // for: char**
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Resources/SceneObject.hpp"
#include "Resources/Camera.hpp"
#include <PhysX\PxPhysicsAPI.h>
#include "Resources\RenderBuffer.h"
#include "Resources\Const.hpp"
#include "Resources\FrustumG.hpp"

#include "Resources/Geometry.hpp"
#include "Scene/Actor.hpp"
#include "Scene/Knight1.hpp"
#include "Scene/Knight2.hpp"
#include "Scene/Room.hpp"
#include "Scene/Wardrobe.hpp"
#include "Scene/Door.hpp"
#include "Scene/Chair1.hpp"
#include "Scene/Chair2.hpp"
#include "Scene/Desk.hpp"
#include "Scene/Frame.hpp"
#include "Scene/Commode.hpp"
#include "Scene/Torch1.hpp"
#include "Scene/Torch2.hpp"
#include "Scene/Chess.hpp"
#include "Scene/Coordinatesystem.hpp"



#include <ft2build.h>
#include FT_FREETYPE_H  

//-------Loading PhysX libraries (32bit only)----------//

#ifdef _DEBUG //If in 'Debug' load libraries for debug mode 
#pragma comment(lib, "PhysX3DEBUG_x86.lib")				//Always be needed  
#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")		//Always be needed
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")		//PhysX extended library 
#pragma comment(lib, "PhysXVisualDebuggerSDKDEBUG.lib") //For PVD only 

#else //Else load libraries for 'Release' mode
#pragma comment(lib, "PhysX3_x86.lib")	
#pragma comment(lib, "PhysX3Common_x86.lib") 
#pragma comment(lib, "PhysX3Extensions.lib")
#pragma comment(lib, "PhysXVisualDebuggerSDK.lib")
#endif

using namespace std;
using namespace cgue;
using namespace glm;
using namespace physx;

void init();
void draw(Shader* shader, mat4x4 view, mat4x4 proj, mat4x4 camera_model);
void update(float time_delta, float time_abs);
void handleInput(GLFWwindow* window, float time_delta);
void OnShutdown();
mat4x4 pxMatToGlm(PxMat44 pxMat);
void RenderQuad();
void initDirectionalShadows();

Shader* renderShader;
Shader* shadowShader;

GLuint FramebufferName = 0;
GLuint hdrFBO;
GLuint colorBuffers[2];
GLuint pingpongFBO[2];
GLuint pingpongColorbuffers[2];

// TODO actor1 to actor
Actor* actor; 
Knight1* knight1;
Knight2* knight2;
Room* room;
Camera* camera;
Wardrobe* wardrobe;
Door* door;
Chair1* chair1;
Chair2* chair2;
Desk* desk;
Frame* frame;
Commode* commode;
Torch1* torch1;
Torch2* torch2;
Chess* chess;
Coordinatesystem* coordinatesystem;



double mouseXPosOld, mouseYPosOld;


FrustumG* frustumG;

float RING_HEIGHT_HIGH = 2.0f;
float RING_HEIGHT_MEDIUM = 6.0f;
float RING_HEIGHT_LOW = 10.0f;

int width;
int height;

float MOVESPEED = 80000.0f;
float ROTATESPEED = 5000.0f;

static PxPhysics*				gPhysicsSDK = NULL;			//Instance of PhysX SDK
static PxFoundation*			gFoundation = NULL;			//Instance of singleton foundation SDK class
static PxDefaultErrorCallback	gDefaultErrorCallback;		//Instance of default implementation of the error callback
static PxDefaultAllocator		gDefaultAllocatorCallback;	//Instance of default implementation of the allocator interface required by the SDK

PxScene*						gScene = NULL;				//Instance of PhysX Scene				


class SimulationEvents : public PxSimulationEventCallback
{

	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)	//This is called when a breakable constraint breaks.
	{
	}

	void onWake(PxActor** actor, PxU32 count)	//This is called during PxScene::fetchResults with the actors which have just been woken up.						
	{
	}

	void onSleep(PxActor** actor, PxU32 count)	////This is called during PxScene::fetchResults with the actors which have just been put to sleep.						
	{
	}

	void onTrigger(PxTriggerPair* pairs, PxU32 nbPairs)	//This is called during PxScene::fetchResults with the current trigger pair events.		
	{
	}


	//The method will be called for a pair of actors if one of the colliding shape pairs requested contact notification.
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
	{
	}

};

static SimulationEvents gSimulationEventCallback;			//Instance of 'SimulationEvents' class inherited from 'PxSimulationEventCallback' class

glm::mat4 proj;
glm::mat4 view;

//Defining a custome filter shader 
PxFilterFlags customFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// all initial and persisting reports for everything, with per-point data
	pairFlags = PxPairFlag::eCONTACT_DEFAULT
		| PxPairFlag::eTRIGGER_DEFAULT
		| PxPairFlag::eNOTIFY_CONTACT_POINTS
		| PxPairFlag::eCCD_LINEAR; //Set flag to enable CCD (Continuous Collision Detection) 

	return PxFilterFlag::eDEFAULT;
}

static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
	std::stringstream stringStream;
	std::string sourceString;
	std::string typeString;
	std::string severityString;

	// The AMD variant of this extension provides a less detailed classification of the error,
	// which is why some arguments might be "Unknown".
	switch (source) {
	case GL_DEBUG_CATEGORY_API_ERROR_AMD:
	case GL_DEBUG_SOURCE_API: {
								  sourceString = "API";
								  break;
	}
	case GL_DEBUG_CATEGORY_APPLICATION_AMD:
	case GL_DEBUG_SOURCE_APPLICATION: {
										  sourceString = "Application";
										  break;
	}
	case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
											sourceString = "Window System";
											break;
	}
	case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:
	case GL_DEBUG_SOURCE_SHADER_COMPILER: {
											  sourceString = "Shader Compiler";
											  break;
	}
	case GL_DEBUG_SOURCE_THIRD_PARTY: {
										  sourceString = "Third Party";
										  break;
	}
	case GL_DEBUG_CATEGORY_OTHER_AMD:
	case GL_DEBUG_SOURCE_OTHER: {
									sourceString = "Other";
									break;
	}
	default: {
				 sourceString = "Unknown";
				 break;
	}
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR: {
								  typeString = "Error";
								  break;
	}
	case GL_DEBUG_CATEGORY_DEPRECATION_AMD:
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
												typeString = "Deprecated Behavior";
												break;
	}
	case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
											   typeString = "Undefined Behavior";
											   break;
	}
	case GL_DEBUG_TYPE_PORTABILITY_ARB: {
											typeString = "Portability";
											break;
	}
	case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:
	case GL_DEBUG_TYPE_PERFORMANCE: {
										typeString = "Performance";
										break;
	}
	case GL_DEBUG_CATEGORY_OTHER_AMD:
	case GL_DEBUG_TYPE_OTHER: {
								  typeString = "Other";
								  break;
	}
	default: {
				 typeString = "Unknown";
				 break;
	}
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: {
									 severityString = "High";
									 break;
	}
	case GL_DEBUG_SEVERITY_MEDIUM: {
									   severityString = "Medium";
									   break;
	}
	case GL_DEBUG_SEVERITY_LOW: {
									severityString = "Low";
									break;
	}
	default: {
				 severityString = "Unknown";
				 break;
	}
	}

	if (type != GL_DEBUG_TYPE_OTHER)
	{
		stringStream << "OpenGL Error: " << msg;
		stringStream << " [Source = " << sourceString;
		stringStream << ", Type = " << typeString;
		stringStream << ", Severity = " << severityString;
		stringStream << ", ID = " << id << "]";
	}

	return "Error" + stringStream.str();
}

static void APIENTRY DebugCallbackAMD(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam) {
	std::string error = FormatDebugOutput(category, category, id, severity, message);
	std::cout << error;
	std::cout << std::endl;
}

static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
	if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB || type == GL_DEBUG_TYPE_OTHER_ARB) {
		/*
		Error "Buffer detailed info: Buffer object 1 (bound to GL_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) 
		will use VIDEO memory as the source for buffer object operations."
		Can be ignored due to https://stackoverflow.com/questions/46771287/why-is-opengl-telling-me-ive-used-gl-static-draw-when-ive-specified-otherwise
		*/
		return; 
	}
	std::string error = FormatDebugOutput(source, type, id, severity, message);
	std::cout << error;
	std::cout << std::endl;
}

int main(int argc, char** argv)
{
	cout << "Loading..." << endl;

	// TODO implement full screen 
	width = 1280;
	height = 768;
	auto fullscreen = false;

	// Parameters
	if (argc == 3) {
		if ((stringstream(argv[1]) >> width).fail() || (stringstream(argv[2]) >> height).fail()){
			cout << "ERROR: Invalid argument!" << endl;
			system("PAUSE");
			exit(EXIT_FAILURE);
		}
	}
	else {
		//cout << "Invalid parameters. Size is set to 800/600" << endl;
	}

	// Init GLFW
	if (!glfwInit()) {
		cout << "ERROR: Could not init glfw." << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	#if _DEBUG
		// Create a debug OpenGL context or tell your OpenGL library (GLFW, SDL) to do so.
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	#endif

	//OGL INITIALISIEREN
	//OGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Refresh rate
	int refresh_rate = 60;
	glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);





	GLFWwindow* window;
	if (fullscreen)
	{
		const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		width = mode->width;
		height = mode->height;

		// Make Window
		window = glfwCreateWindow(width, height, "Haunted Castle", glfwGetPrimaryMonitor(), nullptr);
		if (!window)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		window = glfwCreateWindow(width, height, "Haunted Castle", nullptr, nullptr);
		if (!window)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
	}

	glfwHideWindow(window);



	// Hide Cursor
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // blends out curser if nescessary

	// Set current
	glfwMakeContextCurrent(window);

	// Init Glew
	glewExperimental = true; // must have, so all functions are loaded
	if (glewInit() != GLEW_OK)
	{
		cout << "ERROR: Could init GLEW." << endl;
		glfwTerminate();
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	glGetError();
	#if _DEBUG
		// Query the OpenGL function to register your callback function.
		PFNGLDEBUGMESSAGECALLBACKPROC _glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)wglGetProcAddress("glDebugMessageCallback");
		PFNGLDEBUGMESSAGECALLBACKARBPROC _glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC)wglGetProcAddress("glDebugMessageCallbackARB");
		PFNGLDEBUGMESSAGECALLBACKAMDPROC _glDebugMessageCallbackAMD = (PFNGLDEBUGMESSAGECALLBACKAMDPROC)wglGetProcAddress("glDebugMessageCallbackAMD");

		// Register your callback function.
		if (_glDebugMessageCallback != NULL) {
			_glDebugMessageCallback(DebugCallback, NULL);
		}
		else if (_glDebugMessageCallbackARB != NULL) {
			_glDebugMessageCallbackARB(DebugCallback, NULL);
		}
		else if (_glDebugMessageCallbackAMD != NULL) {
			_glDebugMessageCallbackAMD(DebugCallbackAMD, NULL);
		}

		// Enable synchronous callback. This ensures that your callback function is called
		// right after an error has occurred. This capability is not defined in the AMD
		// version.
		if ((_glDebugMessageCallback != NULL) || (_glDebugMessageCallbackARB != NULL)) {
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}
	#endif

	// FreeType
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 48);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;



	init();


	glfwSetWindowTitle(window, "Haunted Castle");

	atexit(OnShutdown);			//Called on application exit 


	glfwShowWindow(window);


	// Define the color with which the screen is cleared
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Define the area to draw on

	glfwGetCursorPos(window, &mouseXPosOld, &mouseYPosOld);

	// Game Loop
	auto time = glfwGetTime();
	auto refreshTime = 0.0f;
	auto time_abs = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		auto time_new = glfwGetTime();
		auto time_delta = (float)(time_new - time);
		refreshTime += time_delta;
		time_abs += time_delta;
		time = time_new;

		handleInput(window, time_delta);

		update(time_delta, time_abs);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		NUMBER_OF_CULLED_MESHES = 0;




		//cout << "frametime: " << time_delta * 1000 << "ms" << " = " << 1.0 / time_delta << " fps" << endl;

		// Render to our framebuffer
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
		//glViewport(0, 0, 1024, 1024);  // Render on the whole framebuffer, complete from the lower left corner to the upper right

		// We don't use bias in the shader, but instead we draw back faces, 
		// which are already separated from the front faces by a small distance 
		// (if your geometry is made this way)
		glDisable(GL_CULL_FACE);
		//glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(shadowShader->programHandle);


		// Compute the MVP matrix from the light's point of view
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-20, 20, 20, -20, -40.0f, 40.0f);
		glm::mat4 depthViewMatrix = glm::lookAt(SunDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		//lookAt
		// or, for spot light :
		//glm::vec3 lightPos(5, 20, 20);
		//glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
		//glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos-lightInvDir, glm::vec3(0,1,0));

		glm::mat4 depthModelMatrix = glm::mat4(1.0);
		glm::mat4 depthVP = depthProjectionMatrix * depthViewMatrix;
		glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform

		// Shadow
		draw(shadowShader, depthViewMatrix, depthProjectionMatrix, mat4x4(1.0f));


		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

		// 1. Render scene into floating point framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// Clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(renderShader->programHandle);

		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);

		GLuint DepthVPID = glGetUniformLocation(renderShader->programHandle, "depthVP");
		glUniformMatrix4fv(DepthVPID, 1, GL_FALSE, &depthVP[0][0]);


		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		GLuint ShadowMapID = glGetUniformLocation(renderShader->programHandle, "shadowMap");
		glUniform1i(ShadowMapID, 2);


		mat4x4 camera_model = camera->getCameraModel();

		mat4x4 view = camera_model * pxMatToGlm(PxMat44(actor->actor->getGlobalPose().getInverse()));



		nearDist = 1.0f;
		farDist = 200.0f;
		float fov = 100.0f;
		float ratio = (float)width / (float)height;

		proj = glm::perspective(fov, ratio, nearDist, farDist);

		hnear = abs(2 * tan(fov / 2) * nearDist);
		wnear = abs(hnear * ratio);

		hfar = abs(2 * tan(fov / 2) * farDist);
		wfar = abs(hfar * ratio);



		vec4 camera_pos = pxMatToGlm(PxMat44(actor->actor->getGlobalPose())) * vec4(camera->getCameraPos(), 1);
		vec3 p = vec3(camera_pos.x, camera_pos.y, camera_pos.z);

		vec4 look_pos = pxMatToGlm(PxMat44(actor->actor->getGlobalPose())) * vec4(camera->getCameraLookAt(), 1);
		vec3 l = vec3(look_pos.x, look_pos.y, look_pos.z);

		vec4 up_pos = pxMatToGlm(PxMat44(actor->actor->getGlobalPose())) * vec4(camera->getCameraUp(), 1);
		vec3 u = vec3(up_pos.x, up_pos.y, up_pos.z);
		u = u - p;
		u = normalize(u);



		mat4x4 lookAt = glm::lookAt(p, l, u);
		

			
		frustumG->setCamInternals(fov, ratio, nearDist, farDist);
		frustumG->setCamDef(p, l, u);
			


		draw(renderShader, lookAt, proj, camera_model);

		

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glGetError() != GL_NO_ERROR)
		{
			cout << "ERROR: OpenGL Error" << endl;
		}

		if (refreshTime > 1)
		{
			if (CGUE_DISPLAY_FRAME_TIME)
			{
				cout << "Frame time: " << (int)(time_delta * 1000) << "ms, Frame/sec: " << (int)(1.0f / time_delta) << " PhysX Static Actors: " << gScene->getNbActors(PxActorTypeFlag::eRIGID_STATIC) << endl;
			}
			if (VIEWFRUSTUM_CULLING) {
				cout << "Number of Culled Meshes: " << NUMBER_OF_CULLED_MESHES << endl;
			}

			refreshTime = 0;
		}
	}

	// Close GLFW Window
	glfwTerminate();

	return EXIT_SUCCESS;
}

void OnShutdown()
{
	delete actor; actor = nullptr;
	delete knight1; knight1 = nullptr;
	delete knight2; knight2 = nullptr;
	delete room; room = nullptr;
	delete camera; camera = nullptr;
	delete wardrobe; wardrobe = nullptr;
	delete door; door = nullptr;
	delete chair1; chair1 = nullptr;
	delete chair2; chair2 = nullptr;
	delete desk; desk = nullptr;
	delete frame; frame = nullptr;
	delete commode; commode = nullptr;
	delete torch1; torch1 = nullptr;
	delete torch2; torch2 = nullptr;
	delete chess; chess = nullptr;
	delete coordinatesystem; coordinatesystem = nullptr;

	delete renderShader; renderShader = nullptr;
	delete shadowShader; shadowShader = nullptr;

	delete frustumG; frustumG = nullptr;

	gPhysicsSDK->release();			//Removes any actors,  particle systems, and constraint shaders from this scene
	gFoundation->release();			//Destroys the instance of foundation SDK
}

void init() 
{


	//* Source: Learning Physics Modeling with PhysX
	//---------------------------PHYSX-----------------------------]
	//Creating foundation for PhysX
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

	//Creating instance of PhysX SDK
	gPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());
	//Mesh::mgPhysicsSDK = gPhysicsSDK;

	if (gPhysicsSDK == NULL)
	{
		cerr << "Error creating PhysX3 device, Exiting..." << endl;
		exit(1);
	}

	//Creating scene
	PxSceneDesc sceneDesc(gPhysicsSDK->getTolerancesScale());		//Descriptor class for scenes 

	sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);					//Setting gravity
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);		//Creates default CPU dispatcher for the scene

	sceneDesc.filterShader = customFilterShader;					//Creates custom user collision filter shader for the scene
	sceneDesc.simulationEventCallback = &gSimulationEventCallback;  //Resgistering for receiving simulation events

	sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;					//Set flag to enable CCD (Continuous Collision Detection) 

	gScene = gPhysicsSDK->createScene(sceneDesc);					//Creates a scene 

	//*/This will enable basic visualization of PhysX objects like- actors collision shapes and their axes. 
	//The function PxScene::getRenderBuffer() is used to render any active visualization for scene.
	gScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0);	//Global visualization scale which gets multiplied with the individual scales
	gScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);	//Enable visualization of actor's shape
	gScene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1.0f);	//Enable visualization of actor's axis

	//Creating PhysX material (staticFriction, dynamicFriction, restitution)
	PxMaterial* material = gPhysicsSDK->createMaterial(0.5f, 0.5f, 0.5f);

	renderShader = new Shader("Shader/Draw.vert", "Shader/Draw.frag");

	
	frustumG = new FrustumG();


	glEnable(GL_DEPTH_TEST);
	camera = new Camera();

	actor = new Actor(renderShader);
	actor->setPhysX(gPhysicsSDK, gFoundation, gDefaultErrorCallback, gDefaultAllocatorCallback, gScene);
	actor->initActor();

	room = new Room(renderShader);

	frame = new Frame(renderShader);
	commode = new Commode(renderShader);

	if (renderObjects)
	{

		torch1 = new Torch1(renderShader);

		desk = new Desk(renderShader);

		knight1 = new Knight1(renderShader);
		knight2 = new Knight2(renderShader);

		wardrobe = new Wardrobe(renderShader);

		door = new Door(renderShader);

		chair1 = new Chair1(renderShader);
		
		chair2 = new Chair2(renderShader);


		torch2 = new Torch2(renderShader);

		chess = new Chess(renderShader);
		
	}

	//coordinatesystem = new Coordinatesystem(renderShader);
	

	// 60° Open angle, aspect, near, far
	proj = glm::perspective(100.0f, (float)width / (float)height, 0.1f, 200.0f);


	initDirectionalShadows();



	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Couldn't load frame buffer ";
		glfwTerminate();
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	
}

void initDirectionalShadows()
{
	// Shadow Maps
	shadowShader = new Shader(
		"Shader/Shadow.vert",
		"Shader/Shadow.frag");

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Couldn't load frame buffer ";
		glfwTerminate();
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set up floating point framebuffer to render scene to
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glGenTextures(2, colorBuffers);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_BGR, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
		);
	}

	// - Create and attach depth buffer (renderbuffer)
	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
}

GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void StepPhysX(float time_delta)					//Stepping PhysX
{
	gScene->simulate(time_delta > 0.0f ? time_delta : 0.001f);	//Advances the simulation by 'gTimeStep' time
	gScene->fetchResults(true);		//Block until the simulation run is completed
}

void draw(Shader* drawShader, mat4x4 view, mat4x4 proj, mat4x4 camera_model)
{
	bool cull = false;
	if (drawShader == renderShader) {
		cull = true;
	}


	room->draw(drawShader, view, proj, camera_model, cull);

	if (renderObjects)
	{
		knight1->draw(drawShader, view, proj, camera_model, cull);

		knight2->draw(drawShader, view, proj, camera_model, cull);

		wardrobe->draw(drawShader, view, proj, camera_model, cull);

		door->draw(drawShader, view, proj, camera_model, cull);

		chair1->draw(drawShader, view, proj, camera_model, cull);

		chair2->draw(drawShader, view, proj, camera_model, cull);

		desk->draw(drawShader, view, proj, camera_model, cull);

		frame->draw(drawShader, view, proj, camera_model, cull);

		commode->draw(drawShader, view, proj, camera_model, cull);

		torch1->draw(drawShader, view, proj, camera_model, cull);

		torch2->draw(drawShader, view, proj, camera_model, cull);

		chess->draw(drawShader, view, proj, camera_model, cull);

	}

	//coordinatesystem->draw(drawShader, view, proj, camera_model, cull);
	

	// Actor
	//actor->draw(drawShader, view, proj, camera_model, cull);

	glDisable(GL_CULL_FACE);
}

void update(float time_delta, float time_abs) // TODO change time_delta to delta_time
{
	if (gScene)
		StepPhysX(time_delta);
}

void handleInput(GLFWwindow* window, float time_delta)
{

	// Mouse

	double mouseXPos, mouseYPos;
	glfwGetCursorPos(window, &mouseXPos, &mouseYPos);

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS && (mouseXPos != mouseXPosOld || mouseYPos != mouseYPosOld))
	{
		double mouseXPosDiff = mouseXPos - mouseXPosOld;
		double mouseYPosDiff = mouseYPos - mouseYPosOld;

		actor->PxRotate(0, 0, ROTATESPEED * time_delta * mouseXPosDiff / 100);
		

		if (mouseYPosDiff > 0)
		{
			camera->rotateDown(time_delta * mouseYPosDiff / 10);
		}
		if (mouseYPosDiff < 0)
		{
			camera->rotateUp(-time_delta * mouseYPosDiff / 10);
		}
	}

	mouseXPosOld = mouseXPos;
	mouseYPosOld = mouseYPos;


	// camera - actor 0 
	
	if (glfwGetKey(window, GLFW_KEY_RIGHT))
	{
		actor->PxRotate(0, 0, -ROTATESPEED * time_delta);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT))
	{
		actor->PxRotate(0, 0, ROTATESPEED * time_delta);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN))
	{
		actor->PxTranslate(0, -MOVESPEED * time_delta, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_UP))
	{
		actor->PxTranslate(0, MOVESPEED * time_delta, 0);
	}

	// actor 0 - rotate
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		actor->PxRotate(0, 0, ROTATESPEED * time_delta);
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		actor->PxRotate(0, 0, -ROTATESPEED * time_delta);
	}
	// actor 0 - move 
	if (glfwGetKey(window, GLFW_KEY_Q))
	{
		actor->PxTranslate(0, 0, -MOVESPEED * time_delta);
	}
	if (glfwGetKey(window, GLFW_KEY_E))
	{
		actor->PxTranslate(0, 0, MOVESPEED * time_delta);
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		actor->PxTranslate(0, MOVESPEED * time_delta, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		actor->PxTranslate(0, -MOVESPEED * time_delta, 0);
	}

	// ESC - close game
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}

	// F1 - Help
	if (glfwGetKey(window, GLFW_KEY_F1)){
		if (!CGUE_F1_PRESSED)
		{
			cout << endl << "HOW TO PLAY:" << endl;
			cout << "Try to pass the parkour of knight1 faster than your opponent." << endl;
			cout << "-------------------" << endl;
			cout << "CONTROLS:" << endl;
			cout << "CONTROLS                        | FUNCTION" << endl;
			cout << "W, ARROW UP                     | Accelerate" << endl;
			cout << "S, ARROW DOWN                   | Decelerate/Backwards" << endl;
			cout << "A, ARROW LEFT, MOUSE DRAG RIGHT | Pan left" << endl;
			cout << "D, ARROW RIGHT, MOUSE DRAG LEFT | Pan right" << endl;
			cout << "ARROW UP                        | Camera up" << endl;
			cout << "ARROW DOWN                      | Camera down" << endl;
			cout << "ARROW LEFT                      | Camera left" << endl;
			cout << "ARROW RIGHT                     | Camera right" << endl;
			cout << "Q                               | Go down" << endl;
			cout << "E                               | Go up" << endl;
			cout << "MOUSE DRAG UP                   | Camera down" << endl;
			cout << "MOUSE DRAG DOWN                 | Camera up" << endl;
			cout << "-------------------" << endl;
			cout << "F1 - Help / Show Controls" << endl;
			cout << "F2 - Frame Time on/off" << endl;
			cout << "F3 - Wire Frame on/off" << endl;
			cout << "F4 - Textur-Sampling-Quality: Nearest Neighbor/Bilinear" << endl;
			cout << "F5 - Mip Mapping-Quality: Off/Nearest Neighbor/Linear" << endl;
			cout << "F8 - Viewfrustum-Culling on/off" << endl;
			//cout << "F9 - Transparency on/off" << endl;
			cout << "ESC - Quit Game" << endl << endl;
		}
		CGUE_F1_PRESSED = true;
	}
	else
	{
		CGUE_F1_PRESSED = false;
	}

	// F2 - Frame Time on/off
	if (glfwGetKey(window, GLFW_KEY_F2)){
		if (!CGUE_F2_PRESSED)
		{
			if (!CGUE_DISPLAY_FRAME_TIME)
			{
				cout << "Frame Time on" << endl;
				CGUE_DISPLAY_FRAME_TIME = true;
			}
			else
			{
				cout << "Frame Time off" << endl;
				CGUE_DISPLAY_FRAME_TIME = false;
			}
		}
		CGUE_F2_PRESSED = true;
	}
	else
	{
		CGUE_F2_PRESSED = false;
	}

	// F3 - Wire Frame on/off
	if (glfwGetKey(window, GLFW_KEY_F3)){
		if (!CGUE_F3_PRESSED)
		{
			if (CGUE_RENDER == GL_TRIANGLES)
			{
				cout << "Wire Frame on" << endl;
				CGUE_RENDER = GL_LINE_STRIP;// GL_LINES;
			}
			else
			{
				cout << "Wire Frame off" << endl;
				CGUE_RENDER = GL_TRIANGLES;
			}
		}
		CGUE_F3_PRESSED = true;
	}
	else
	{
		CGUE_F3_PRESSED = false;
	}

	//  F4 - Textur-Sampling-Quality: Nearest Neighbor/Bilinear
	if (glfwGetKey(window, GLFW_KEY_F4)){
		if (!CGUE_F4_PRESSED)
		{
			if (!TEXTURE_SAMPLING_QUALITY)
			{
				cout << "Textur-Sampling-Quality: Bilinear" << endl;
				TEXTURE_SAMPLING_QUALITY = true;
			}
			else
			{
				cout << "Textur-Sampling-Quality: Nearest Neighbor" << endl;
				TEXTURE_SAMPLING_QUALITY = false;
			}
		}
		CGUE_F4_PRESSED = true;
	}
	else
	{
		CGUE_F4_PRESSED = false;
	}


	// F5 - Mip Mapping-Quality: Off/Nearest Neighbor/Linear
	if (glfwGetKey(window, GLFW_KEY_F5)){
		if (CGUE_F5_PRESSED == false)
		{
			if (MIP_MAPPING_QUALITY == 0)
			{
				cout << "Mip Mapping-Quality: Nearest Neighbor" << endl;
				MIP_MAPPING_QUALITY = 1;
			}
			else if (MIP_MAPPING_QUALITY == 1)
			{
				cout << "Mip Mapping-Quality: Linear" << endl;
				MIP_MAPPING_QUALITY = 2;
			}
			else
			{
				cout << "Mip Mapping-Quality: Off" << endl;
				MIP_MAPPING_QUALITY = 0;
			}
		}
		CGUE_F5_PRESSED = true;
	}
	else
	{
		CGUE_F5_PRESSED = false;
	}


	// F8 - Viewfrustum-Culling on/off
	if (glfwGetKey(window, GLFW_KEY_F8)){
		if (CGUE_F8_PRESSED == false)
		{
			if (!VIEWFRUSTUM_CULLING)
			{
				cout << "Viewfrustum-Culling on" << endl;
				VIEWFRUSTUM_CULLING = true;
			}
			else
			{
				cout << "Viewfrustum-Culling off" << endl;
				VIEWFRUSTUM_CULLING = false;
			}
		}
		CGUE_F8_PRESSED = true;
	}
	else
	{
		CGUE_F8_PRESSED = false;
	}
}