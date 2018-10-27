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
bool EFFECT_HDR_ENABLED = false;
bool CGUE_F8_PRESSED = false;
bool VIEWFRUSTUM_CULLING = false;
int NUMBER_OF_CULLED_MESHES = 0;
bool CGUE_F9_PRESSED = false;
bool TRANSPARENCY = true;

bool CGUE_SHOT_ACTOR1_PRESSED = false;
bool CGUE_SHOT_ACTOR2_PRESSED = false;

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
	cout << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << " " << mat[0][3] << endl;
	cout << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << " " << mat[1][3] << endl;
	cout << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << " " << mat[2][3] << endl;
	cout << mat[3][0] << " " << mat[3][1] << " " << mat[3][2] << " " << mat[3][3] << endl;
	//*/
}

GLuint depthTexture;
glm::mat4 depthBiasMVP;
glm::mat4 depthMVP;
vec3 lightPos = vec3(8, -2, -8);

float wnear = 0;
float hnear = 0;
float wfar = 0;
float hfar = 0;
float nearDist = 0;
float farDist = 0;