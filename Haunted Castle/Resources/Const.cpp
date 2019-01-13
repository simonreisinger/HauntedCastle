#include "Const.hpp"

float PI = 3.14159265358979323846f;


bool CGUE_F1_PRESSED = false;
bool CGUE_F2_PRESSED = false;
bool CGUE_DISPLAY_FRAME_TIME = false;
bool CGUE_F3_PRESSED = false;
int CGUE_RENDER = GL_TRIANGLES;
bool CGUE_F4_PRESSED = false;
bool TEXTURE_SAMPLING_QUALITY = true;
bool CGUE_F5_PRESSED = false;
int MIP_MAPPING_QUALITY = 2;
bool CGUE_F6_PRESSED = false;
bool VIEWFRUSTUM_CULLING = false;
bool CGUE_F7_PRESSED = false;
bool NORMAL_MAPPING = true;
bool CGUE_F8_PRESSED = false;
bool FIRE_AND_SHADOWS_1 = true;
bool CGUE_F9_PRESSED = false;
bool FIRE_AND_SHADOWS_2 = true;
bool CGUE_F10_PRESSED = false;
bool CGUE_F11_PRESSED = false;
bool BLOOM = true;
int NUMBER_OF_CULLED_MESHES = 0;

bool CGUE_SHOT_ACTOR1_PRESSED = false;
bool CGUE_SHOT_ACTOR2_PRESSED = false;

int TEXTURE_SLOT_MESH_DIFFUSE = 0;
int TEXTURE_SLOT_MESH_NORMAL = 1;
int TEXTURE_SLOT_DIRECTIONAL_SHADOW = 2;
int TEXTURE_SLOT_POINT_SHADOWS[2] = { 3, 4 };
int TEXTURE_SLOT_FIRE= 5;
int TEXTURE_SLOT_BLOOM_COLORBUFFER = 6;
int TEXTURE_SLOT_BLOOM_TOBEBLOOMED = 7; // TODO rename this

mat4x4 pxMatToGlm(PxMat44 pxMat)
{
	return mat4x4(pxMat.column0.x, pxMat.column0.y, pxMat.column0.z, pxMat.column0.w,
		pxMat.column1.x, pxMat.column1.y, pxMat.column1.z, pxMat.column1.w,
		pxMat.column2.x, pxMat.column2.y, pxMat.column2.z, pxMat.column2.w,
		pxMat.column3.x, pxMat.column3.y, pxMat.column3.z, pxMat.column3.w);
}

PxMat44 glmToPxMat(mat4x4 mat)
{
	return PxMat44 (PxVec4(mat[0][0], mat[0][1], mat[0][2], mat[0][3]),
					PxVec4(mat[1][0], mat[1][1], mat[1][2], mat[1][3]),
					PxVec4(mat[2][0], mat[2][1], mat[2][2], mat[2][3]),
					PxVec4(mat[3][0], mat[3][1], mat[3][2], mat[3][3]));
}

void printMatGeometry(mat4x4 mat)
{
	//*
	cout << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << " " << mat[3][0] << endl;
	cout << mat[0][1] << " " << mat[1][1] << " " << mat[2][1] << " " << mat[3][1] << endl;
	cout << mat[0][2] << " " << mat[1][2] << " " << mat[2][2] << " " << mat[3][2] << endl;
	cout << mat[0][3] << " " << mat[1][3] << " " << mat[2][3] << " " << mat[3][3] << endl;
	//*/
}

GLuint directionalShadowsDepthMap;
glm::mat4 depthBiasMVP;
glm::mat4 depthMVP;

// Change the Axis from (x, z, y) to (x, z, -y)
mat3x3 changeAxis = mat3x3(1, 0, 0, 0, 0, -1, 0, 1, 0);
mat3x3 changeAxisInverse = mat3x3(1, 0, 0, 0, 0, 1, 0, -1, 0);

vec3 flameDir = normalize(vec3(-0.3, 1, 0));

vec3 torchPos[2] =
{
	changeAxis * vec3(12.5634, -11.12736, 9.75919) + flameDir * 0.01f,
	changeAxis * vec3(12.5634, 8.90968, 9.75919) + flameDir * 0.01f
};

float flameIntensity[2] =
{
	1,
	1
};
float flameIntensityMax = 1.1;
float flameIntensityMin = 0.9;

float AmbientIntensity = 0.1f;

vec3 SunDir = changeAxis * vec3(0.0, -1.0, 1.0);

float wnear = 0;
float hnear = 0;
float wfar = 0;
float hfar = 0;

int iMeshesLoaded = 0;
int countMeshesLoading = 73;

int iObjectsLoaded = 0;
int countObjectsLoading = 15;

bool renderObjects = true;
bool debugMode = false;
