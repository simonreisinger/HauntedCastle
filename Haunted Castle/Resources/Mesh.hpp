#pragma once

#include <GL\glew.h>
#include "../Resources/SceneObject.hpp"
#include "../Resources/Shader.hpp"
#include "../Resources/Texture.hpp"
#include <iostream>

#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include <PhysX\PxPhysicsAPI.h>

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

using namespace glm;
using namespace physx;

namespace cgue
{

	class Mesh
	{
	public:
		Mesh();
		Mesh(string modelDir, char* nameMesh, aiMesh* mesh, const aiMaterial* material, glm::mat4x4 initTrans, glm::mat4x4 transform, Shader* shader);
		virtual ~Mesh();
		void draw(Shader* shader, mat4x4 view, mat4x4 proj, mat4x4 globalPose, bool cull);
		void loadUniforms(Shader* shader, mat4x4 view, mat4x4 proj, mat4x4 globalPose);
		void setPhysX(PxPhysics* mPhysicsSDK, PxFoundation* mFoundation, PxDefaultErrorCallback mDefaultErrorCallback, PxDefaultAllocator mDefaultAllocatorCallback);
		
		bool isVisible(mat4x4 globalPose);

		GLuint texid;
		Shader* shader;
		bool hasTexture;
		bool hasNormalTexture;
		glm::vec3 diffuse;
		glm::vec3 specular;
		Texture* texture;
		Texture* normalTexture;

		GLuint vao, positionBuffer, normalsBuffer, tangentsBuffer, bitangentsBuffer, uvBuffer, indexBuffer;

		int size;
		float* positions;
		float* normals;
		float* tangents;
		float* bitangents;
		int* indices;
		float* uvs;

	private:
		const int numberOfEdgesOfACubePlusNumberOfPlanesPlusEdges = 8;
		vec3 positionOfExtremValues[8];

		Box box;
		float sphereRadius;
		vec3 sphereCenter;

		float xmin = 0.0;
		float xmax = 0.0;
		float ymin = 0.0;
		float ymax = 0.0;
		float zmin = 0.0;
		float zmax = 0.0;

		bool renderRayShafts = false;

		string nameMesh;
		mat4x4 updateTrans;

		PxCooking* mCooking;

		PxPhysics*				gPhysicsSDK = NULL;			//Instance of PhysX SDK
		PxFoundation*			gFoundation = NULL;			//Instance of singleton foundation SDK class
		PxDefaultErrorCallback	gDefaultErrorCallback;		//Instance of default implementation of the error callback
		PxDefaultAllocator		gDefaultAllocatorCallback;	//Instance of default implementation of the allocator interface required by the SDK
	};
};