#include "FrustumG.hpp"
#include <iostream>
#include <string>

using namespace glm;

#define ANG2RAD 3.14159265358979323846/180.0

Plane::Plane(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {

	set3Points(v1, v2, v3);
}

Plane::Plane() {}

Plane::~Plane() {}

void Plane::set3Points(vec3 v1, vec3 v2, vec3 v3) {


	glm::vec3 aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = glm::cross(aux2, aux1);
	auto normal2 = glm::cross(aux1, aux2);

	normal = glm::normalize(normal);
	point.x = v2.x; point.y = v2.y; point.z = v2.z;
	d = -(glm::dot(normal, point));
}

/*
float Plane::distance(vec3 p) {
	return A * p.x + B * p.y + C * p.z;
}
*/
float Plane::distance(vec3 p) {
	return (d + glm::dot(normal, p));
}


void Plane::setCoefficients(float a, float b, float c, float d) {

	// set the normal vector
	normal = vec3(a, b, c);
	//compute the lenght of the vector
	float l = normal.length();
	// normalize the vector
	normal = vec3(a / l, b / l, c / l);
	// and divide d by th length as well
	this->d = d / l;
}

Box::Box()
{

}

void Box::setVertex(vec3 ncorners[]) {
	for (int i = 0; i < 8; i++) {
		corners[i] = ncorners[i];
	}
}

Box::Box(vec3 ncorners[]) {
	for (int i = 0; i < 8; i++) {
		corners[i] = ncorners[i];
	}
}

vec3 Box::getVertex(int k, mat4x4 mvp) {
	vec4 v = mvp * vec4(corners[k], 1);
	return vec3(v.x, v.y, v.z);
}



FrustumG::FrustumG() {}

FrustumG::~FrustumG() {}

void FrustumG::setCamInternals(float angle, float ratio, float nearD, float farD) {

	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	tang = (float)tan(angle * 0.5);
	nh = nearD * tang;
	nw = nh * ratio;
	fh = farD  * tang;
	fw = fh * ratio;


}

void FrustumG::setCamDef(glm::vec3 &p, glm::vec3 &l, glm::vec3 &u) {

	glm::vec3 dir, nc, fc, X, Y, Z;

	Z = p - l;
	Z = glm::normalize(Z);

	X = glm::cross(u, Z);
	X = glm::normalize(X);

	Y = glm::cross(Z, X);

	nc = p - (Z * nearD);
	fc = p - (Z * farD);

	ntl = nc + (Y * nh) - (X * nw);
	ntr = nc + (Y * nh) + (X * nw);
	nbl = nc - (Y * nh) - (X * nw);
	nbr = nc - (Y * nh) + (X * nw);

	ftl = fc + (Y * fh) - (X * fw);
	ftr = fc + (Y * fh) + (X * fw);
	fbl = fc - (Y * fh) - (X * fw);
	fbr = fc - (Y * fh) + (X * fw);
	
	pl[TOP].set3Points(ntr, ntl, ftl);
	pl[BOTTOM].set3Points(nbl, nbr, fbr);
	pl[LEFT].set3Points(ntl, nbl, fbl);
	pl[RIGHT].set3Points(nbr, ntr, fbr);
	pl[NEARP].set3Points(ntl, ntr, nbr);
	pl[FARP].set3Points(ftr, ftl, fbl);
}

int FrustumG::boxInFrustum(Box &b, mat4x4 mvp) {


	int result = INSIDE, out, in;

	// for each plane do ...
	for (int i = 0; i < 6; i++) {

		// reset counters for corners in and out
		out = 0; in = 0;
		// for each corner of the box do ...
		// get out of the cycle as soon as a box as corners
		// both inside and out of the frustum
		for (int k = 0; k < 8 && (in == 0 || out == 0); k++) {

			// is the corner outside or inside
			if (pl[i].distance(b.getVertex(k, mvp)) < 0)
				out++;
			else
				in++;
		}
		//if all corners are out
		if (!in)
			return (OUTSIDE);
		// if some corners are out and others are in	
		else if (out)
			result = INTERSECT;
	}
	return(result);

}


int FrustumG::pointInFrustum(glm::vec3 &p) {

	int result = INSIDE;
	for (int i = 0; i < 6; i++) {

		if (pl[i].distance(p) < 0)
			return OUTSIDE;
	}
	return(result);

}


int FrustumG::sphereInFrustum(glm::vec3 &p, float raio) {

	int result = INSIDE;
	float distance;

	for (int i = 0; i < 6; i++) {
		distance = pl[i].distance(p);
		if (distance < -raio)
			return OUTSIDE;
		else if (distance < raio)
			result = INTERSECT;
	}
	return(result);

}
