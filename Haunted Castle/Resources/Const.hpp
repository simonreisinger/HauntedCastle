#pragma once

#include <GL\glew.h>
#include <PhysX\PxPhysicsAPI.h>
#include <glm\glm.hpp>
#include <iostream>
#include "FrustumG.hpp"

using namespace std;
using namespace glm;

using namespace physx;

extern float PI;

extern int CGUE_RENDER;
extern bool CGUE_F3_PRESSED;
extern bool CGUE_DISPLAY_FRAME_TIME;
extern bool CGUE_F2_PRESSED;
extern bool CGUE_SHOT_ACTOR1_PRESSED;
extern bool CGUE_SHOT_ACTOR2_PRESSED;

extern bool CGUE_F1_PRESSED;
extern bool CGUE_F2_PRESSED;
extern bool CGUE_DISPLAY_FRAME_TIME;
extern bool CGUE_F3_PRESSED; 
extern int CGUE_RENDER;
extern bool CGUE_F4_PRESSED;
extern bool TEXTURE_SAMPLING_QUALITY;
extern bool CGUE_F5_PRESSED;
extern int MIP_MAPPING_QUALITY;
extern bool CGUE_F6_PRESSED;
extern bool EFFECT_HDR_ENABLED;
extern bool CGUE_F8_PRESSED;
extern bool VIEWFRUSTUM_CULLING;
extern int NUMBER_OF_CULLED_MESHES;
extern bool CGUE_F9_PRESSED;
extern bool TRANSPARENCY;

extern mat4x4 pxMatToGlm(PxMat44 pxMat);
extern PxMat44 glmToPxMat(mat4x4 mat);
extern void printMatGeometry(mat4x4 mat);

extern GLuint depthTexture;
extern glm::mat4 depthBiasMVP;
extern glm::mat4 depthMVP;

extern vec3 torch1Pos;
extern vec3 torch2Pos;

extern float wnear;
extern float hnear;
extern float wfar;
extern float hfar;
extern float nearDist;
extern float farDist;

extern int iMeshesLoaded;
extern int countMeshesLoading;

extern FrustumG* frustumG;

extern bool renderObjects;

// Change the Axis from (x, z, y) to (x, z, -y)
extern mat3x3 changeAxis;