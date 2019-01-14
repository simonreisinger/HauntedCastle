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
extern bool VIEWFRUSTUM_CULLING;
extern bool CGUE_F7_PRESSED;
extern bool NORMAL_MAPPING;
extern bool CGUE_F8_PRESSED;
extern bool FIRE_AND_SHADOWS_1;
extern float FIRE_AND_SHADOWS_INTENSITY_1;
extern bool CGUE_F9_PRESSED;
extern bool FIRE_AND_SHADOWS_2;
extern float FIRE_AND_SHADOWS_INTENSITY_2;
extern bool CGUE_F10_PRESSED;
extern bool CGUE_F11_PRESSED;
extern bool LIGHT_SHAFTS;
extern bool CGUE_P_PRESSED;
extern bool BLOOM;
extern int NUMBER_OF_CULLED_MESHES;
extern bool objectMoved;

extern mat4x4 pxMatToGlm(PxMat44 pxMat);
extern PxMat44 glmToPxMat(mat4x4 mat);
extern void printMatGeometry(mat4x4 mat);

extern GLuint directionalShadowsDepthMap;
extern glm::mat4 depthBiasMVP;
extern glm::mat4 depthMVP;

extern vec3 torchPos[2];

extern vec3 flameDir;

extern float flameIntensity[2];
extern float flameIntensityMax;
extern float flameIntensityMin;

extern float AmbientIntensity;;

extern vec3 SunDir;

extern float wnear;
extern float hnear;
extern float wfar;
extern float hfar;

extern int iMeshesLoaded;
extern int countMeshesLoading;

extern int iObjectsLoaded;
extern int countObjectsLoading;

extern FrustumG* frustumG;

extern bool renderObjects;
extern bool debugMode;
extern bool playSound;
extern bool fullscreen;

// Change the Axis from (x, z, y) to (x, z, -y)
extern mat3x3 changeAxis;
extern mat3x3 changeAxisInverse;

extern int TEXTURE_SLOT_MESH_DIFFUSE;
extern int TEXTURE_SLOT_MESH_NORMAL;
extern int TEXTURE_SLOT_DIRECTIONAL_SHADOW;
extern int TEXTURE_SLOT_POINT_SHADOWS[2];
extern int TEXTURE_SLOT_FIRE;
extern int TEXTURE_SLOT_BLOOM_COLORBUFFER;
extern int TEXTURE_SLOT_BLOOM_TOBEBLOOMED;
