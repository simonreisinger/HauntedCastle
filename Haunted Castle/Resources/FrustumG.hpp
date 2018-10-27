#pragma once

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Plane {
private:
	float A, B, C, D;
	float d;
	vec3 normal;
	vec3 point;
public:
	Plane();
	~Plane();
	Plane(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
	void set3Points(vec3 ntr, vec3 ntl, vec3 ftl);
	void setCoefficients(float A, float B, float C, float D);
	float distance(vec3 p);
};

class Box {
public:
	Box();
	Box(vec3 ncorners[]);
	void setVertex(vec3 ncorners[]);
	vec3 getVertex(int k, mat4x4 mvp);
private:
	vec3 corners[8];
};

class FrustumG {

private:

	enum {
		TOP = 0, BOTTOM, LEFT,
		RIGHT, NEARP, FARP
	};

public:

	static enum { OUTSIDE = 0, INTERSECT = 1, INSIDE = 2 };

	void extractPlanes(mat4x4 mat);

	Plane pl[6];

	vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	float nearD, farD, ratio, angle, tang;
	float nw, nh, fw, fh;

	FrustumG::FrustumG();
	FrustumG::~FrustumG();

	void FrustumG::setFrustum(mat4x4 *m);
	void setCamInternals(float angle, float ratio, float nearD, float farD);
	void setCamDef(vec3 &p, vec3 &l, vec3 &u);
	int pointInFrustum(vec3 &p);
	int sphereInFrustum(vec3 &p, float raio);
	int boxInFrustum(Box &b, mat4x4 mvp);

};