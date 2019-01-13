#include "Camera.hpp"

using namespace cgue;
using namespace glm;

float eh = 6;

/*
CameraPoint cameraPoints[] =
{
	CameraPoint(vec3(0, 2, eh), vec3(0, -1, 0), 0),
	CameraPoint(vec3(0, 1, eh), vec3(0, -1, 0), 1),
	CameraPoint(vec3(0, 0, eh), vec3(0, -1, 0), 2),
	CameraPoint(vec3(0, -1, eh), vec3(0, -1, 0), 3),
	CameraPoint(vec3(0, -4, eh - 1), vec3(0, -1, -1), 4),
	CameraPoint(vec3(0, -7, eh), vec3(0, -1, 0), 5),
	CameraPoint(vec3(2, -9, eh), vec3(1, 0, 0), 6),
	CameraPoint(vec3(3, -9, eh), vec3(1, 0, 0), 7)
};
*/

// Manuell
//*
CameraPoint cameraPoints[] =
{
CameraPoint(vec3(-9.00599, 14.4593, 8.4675), vec3(0.319084, -0.919355, -0.230153), 0),
CameraPoint(vec3(-6.57561, 6.1819, 8.02367), vec3(0.22869, -0.94976, 0.213676), 1),
CameraPoint(vec3(3.8632, -10.4071, 6.33467), vec3(0.75546, -0.646688, 0.10523), 1),
CameraPoint(vec3(6.81585, -12.4818, 5.61332), vec3(0.732864, 0.175303, 0.657404), 0)
};
//*/

Camera::Camera(){
	//							CamPos			BallonPos			UP
	modelMatrix = mat4x4(1.0);// = glm::lookAt(vec3(0, 0, 15), vec3(0, 0, 0), vec3(0, 1, 0));
	//modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -15));

	//				+X = T�r, -Y = Fenster, +Z = H�he
	float height = 7.0;
	bSpline.addPoint(vec3(0, 9, height), 1);
	bSpline.addPoint(vec3(0, 7, height), 2);
	bSpline.addPoint(vec3(0, 5, height - 0.6), 2);
	bSpline.addPoint(vec3(0, 3, height - 0.8), 2);
	bSpline.addPoint(vec3(0, 1, height - 0.4), 2);
	bSpline.addPoint(vec3(0, -1, height), 2);
	bSpline.addPoint(vec3(0, -3, height), 2);
	bSpline.addPoint(vec3(1, -5, height), 2);
	bSpline.addPoint(vec3(1, -7, height), 2);
	bSpline.addPoint(vec3(2, -9, height), 2);
	bSpline.addPoint(vec3(2, -11, height), 2);
	bSpline.addPoint(vec3(3, -13, height), 2);
	bSpline.addPoint(vec3(4, -13, height), 2);
	bSpline.addPoint(vec3(6, -13, height), 2);
	bSpline.addPoint(vec3(8, -13, height), 2);
	/*
	bSpline.addPoint(vec3(2, -5, height), 2);
	bSpline.addPoint(vec3(4, -5, height), 2);
	bSpline.addPoint(vec3(6, -3, height), 2);
	bSpline.addPoint(vec3(6, 5, height), 2);
	*/
	bSpline.addLastPoint(vec3(6, -3, height), 1, 1, 1);
}

Camera::~Camera(){}

mat4x4 Camera::getCameraModel()
{
	//return modelMatrix;
	//return glm::translate(mat4x4(1.0f), glm::vec3(0, 0, -15)) * modelMatrix;
	return lookAt(getCameraPos(), getCameraLookAt(), getCameraUp());
}

mat4x4 Camera::getInverseCameraModel()
{
	//return modelMatrix;
	//return glm::translate(mat4x4(1.0f), glm::vec3(0, 0, -15)) * modelMatrix;
	return inverse(lookAt(getCameraPos(), getCameraLookAt(), getCameraUp()));
}

vec3 Camera::getCameraPos()
{
	if (automaticCameraMovementActivated)
	{
		return changeAxis * p;
	}
	else
	{
		vec4 v = vec4(changeAxis * vec3(0, -1, 0), 1) * modelMatrix;
		return vec3(v.x, v.y, v.z);
	}
}

vec3 Camera::getCameraLookAt()
{
	if (automaticCameraMovementActivated)
	{
		return changeAxis * (p + normalize(d));
	}
	else
	{
		return vec3(0, 0, 0);
	}
}

vec3 Camera::getCameraUp()
{
	if (automaticCameraMovementActivated)
	{
		return changeAxis * (p + normalize(d) + vec3(0, 0, -1));
	}
	else
	{
		vec4 v = vec4(changeAxis * vec3(0, 0, -1), 1) * modelMatrix;
		return vec3(v.x, v.y, v.z);
	}
}

bool Camera::getAutomaticCameraMovementActivated()
{
	return automaticCameraMovementActivated;
}

void Camera::changeAutomaticCameraMovementActivatedState()
{
	automaticCameraMovementActivated = !automaticCameraMovementActivated;
}

void computePointCatmullRomCurve(float t, vec3 &p, vec3 &d, vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 d1, vec3 d2, vec3 d3, vec3 d4)
{
	mat3x4 c = mat3x4(p1.x, p2.x, p3.x, p4.x, p1.y, p2.y, p3.y, p4.y, p1.z, p2.z, p3.z, p4.z);

	float s = 0.5;

	mat4x4 cr = mat4x4(-s, 2*s, -s, 0, 2-s, s-3, 0, 1, s-2, 3-2*s, s, 0, s, -s, 0, 0);

	vec4 u = vec4(t*t*t, t*t, t, 1);

	p = u * cr * c;

	// Source: https://www.lucidarme.me/catmull-rom-splines/

	mat4x4 dr = mat4x4(0, -s, 2 * s, -s, 1, 0, s - 3, 2 - s, 0, s, 3 - 2 * s, s - 2, 0, 0, -s, s);

	vec4 du = vec4(0, 1, 2 * t, 3 * t*t);

	d = du * dr * c;
}

void computePointBezierCurve(float t, vec3 &p, vec3 &d, vec3 p1, vec3 p2, vec3 d1, vec3 d2)
{
	vec3 A = p1;
	vec3 D = p2;

	float distAB = length(A - D);
	float factor = distAB / 4;

	vec3 B = A + d1 * factor;
	vec3 C = D - d2 * factor;

	float s = 1 - t;
	vec3 AB = A * s + B * t;
	vec3 BC = B * s + C * t;
	vec3 CD = C * s + D * t;
	vec3 ABC = AB * s + BC * t;
	vec3 BCD = BC * s + CD * t;

	p = ABC * s + BCD * t;
	d = normalize(BCD - ABC);
}

void computePointCubicHermiteCurve(float t, vec3 &p, vec3 &d, vec3 p1, vec3 p2, vec3 d1, vec3 d2)
{

	// Source: https://github.com/keshavnandan/OpenGL/blob/master/hermite_curve/hermite.cpp

	float h0, h1, h2, h3, dh0, dh1, dh2, dh3, t2, t3;
	t2 = t * t;
	t3 = t2 * t;
	//hermite blending functions
	h0 = 2 * t3 - 3 * t2 + 1;
	h1 = -2 * t3 + 3 * t2;
	h2 = t3 - 2 * t2 + t;
	h3 = t3 - t2;

	p = p1 * h0 + p2 * h1 + d1 * h2 + d2 * h3;

	//cout << t << " P(" << p.x << " " << p.y << " " << p.z << ") ";
	//cout << " P1(" << p1.x << " " << p1.y << " " << p1.z << ") P2(" << p2.x << " " << p2.y << " " << p2.z << ")" << endl;

	// Source: https://math.stackexchange.com/questions/2444650/cubic-hermite-spline-derivative

	dh0 = 6 * t2 - 6 * t;
	dh1 = -6 * t2 + 6 * t;
	dh2 = 3 * t2 - 4 * t + 1;
	dh3 = 3 * t2 - 2 * t;

	d = p1 * dh0 + p2 * dh1 + d1 * dh2 + d2 * dh3;
}

vec3 computePointLinearInterpolation(float t, vec3 &p, vec3 &d, vec3 p1, vec3 p2, vec3 d1, vec3 d2)
{
	float beta = 1.5f;
	float t_scurve = 1 / (1 + pow(t / (1 - t), -beta));
	return p1 * (1 - t_scurve) + p2 * t_scurve;
}

vec3 computeDerivativeLinearInterpolation(float t, vec3 &p, vec3 &d, vec3 p1, vec3 p2, vec3 d1, vec3 d2)
{
	float beta = 2;
	float t_scurve = 1 / (1 + pow(t / (1 - t), -beta));
	return d1 * (1 - t_scurve) + d2 * t_scurve;
}

float computeB(int i, int k, float x)
{
	int n = (sizeof(cameraPoints) / sizeof(*cameraPoints));
	if (k == 0)
	{
		if (cameraPoints[i].getTime() <= x && x < cameraPoints[max(i+1, n-1)].getTime())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{

		float a = 0;
		if (i < n && i + k < n)
		{
			float t_i = cameraPoints[i].getTime();
			float t_ik = cameraPoints[i + k].getTime();
			a = (x - t_i) / (t_ik - t_i);
		}
		float b = 0;
		if (i + 1 < n && i + k + 1 < n)
		{
			float t_ik1 = cameraPoints[i + k + 1].getTime();
			float t_i1 = cameraPoints[i + 1].getTime();
			b = (t_ik1 - x) / (t_ik1 - t_i1);
		}

		return (a == 0 ? 0 : a * computeB(i, k - 1, x)) + (b==0 ? 0 : b * computeB(i + 1, k - 1, x));
	}
}

void computePointBSpline(float x)
{
	int n = (sizeof(cameraPoints) / sizeof(*cameraPoints));

	vec3 point = vec3(0);
	float weights = 0;
	for (int i = 0; i < n; i++)
	{
		float weight = computeB(i, 2, x);
		point += cameraPoints[i].getPoint() * weight;
		weights += weight;
	}
	cout << "Weight: " << weights << " - Point: " << point.x << ", " << point.y << ", " << point.z << endl;
}

const int CURVE_LINEAR = 1;
const int CURVE_HERMITE = 2;
const int CURVE_CATMULL = 3;
const int CURVE_BEZIER = 4;
const int CURVE_BSPLINE = 5;

int method = CURVE_LINEAR;

void Camera::advance(float time_delta)
{
	if (automaticCameraMovementActivated)
	{
		int indexLastEndPoint = (sizeof(cameraPoints) / sizeof(*cameraPoints)) - 1;
		if (t >= indexLastEndPoint)
		{
			/*
			p = cameraPoints[indexLastEndPoint].getPoint();
			d = normalize(cameraPoints[indexLastEndPoint].getDerivative());
			*/
		}
		else
		{
			int tfloor = (int)floor(t);
			int tceil = (int)ceil(t);
			if (tfloor == tceil) {
				tceil += 1;
			}

			//cout << tfloor << " " << tceil << " - ";

			CameraPoint Start = cameraPoints[tfloor];
			CameraPoint End = cameraPoints[tceil];

			vec3 p1 = Start.getPoint();
			vec3 p2 = End.getPoint();
			vec3 d1 = Start.getDerivative();
			vec3 d2 = End.getDerivative();

			float advance_t = (t - tfloor);

			switch (method)
			{
			case CURVE_LINEAR:
				p = computePointLinearInterpolation(advance_t, p, d, p1, p2, d1, d2);
				d = computeDerivativeLinearInterpolation(advance_t, p, d, p1, p2, d1, d2);
				break;
			case CURVE_HERMITE:
				computePointCubicHermiteCurve(advance_t, p, d, p1, p2, d1, d2);
				break;
			case CURVE_CATMULL:
				if (t < indexLastEndPoint - 2)
				{
					vec3 p3 = cameraPoints[tceil + 1].getPoint();
					vec3 p4 = cameraPoints[tceil + 2].getPoint();
					vec3 d3 = cameraPoints[tceil + 1].getDerivative();
					vec3 d4 = cameraPoints[tceil + 2].getDerivative();
					computePointCatmullRomCurve(advance_t, p, d, p1, p2, p3, p4, d1, d2, d3, d4);

					t += 1 * time_delta / length(p2 - p3);
				}
				break;
			case CURVE_BEZIER:
				computePointBezierCurve(advance_t, p, d, p1, p2, d1, d2);
				break;
			}
			

			if (method != CURVE_CATMULL && method != CURVE_BSPLINE)
			{
				t += 2.0f * time_delta / length(p1 - p2);
			}
		}
		if (method == CURVE_BSPLINE)
		{
			t += 1 * time_delta;
		}

		switch (method)
		{
		case CURVE_BSPLINE:

			cout << "t = " << t << " ";
			p = bSpline.calcPoint(t);
			d = bSpline.calcDerivative(t);
			cout << "Point: " << p.x << ", " << p.y << ", " << p.z << " ";
			cout << "Derivative: " << d.x << ", " << d.y << ", " << d.z << endl;

			break;
		}
	}
}

void drawLineStrop(Shader *shader, mat4x4 VP, vector<vec3> points) {
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vb;
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, 3 * points.size() * sizeof(float), &points[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glVertexAttribPointer(glGetAttribLocation(shader->programHandle, "pos"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(shader->programHandle, "pos"));
	shader->useShader();

	// Model View
	auto view_proj_location = glGetUniformLocation(shader->programHandle, "VP");
	glUniformMatrix4fv(view_proj_location, 1, GL_FALSE, value_ptr(VP));

	glLineWidth(10.0);

	glDrawArrays(GL_LINE_STRIP, 0, points.size());

	glUseProgram(0);

	glDisableVertexAttribArray(glGetAttribLocation(shader->programHandle, "pos"));
	glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);


	glBindVertexArray(0);
}

void Camera::drawCurve(Shader* shader, mat4x4 VP) {
	vector<vec3> points;
	/*
	points.push_back(vec3(5, 5, -5));
	points.push_back(vec3(-5, 5, -5));
	points.push_back(vec3(-5, -5, -5));
	points.push_back(vec3(5, -5, -5));
	points.push_back(vec3(5, 5, 5));
	points.push_back(vec3(-5, 5, 5));
	points.push_back(vec3(-5, -5, 5));
	points.push_back(vec3(5, -5, 5));
	*/


	for (int i = 0; i < sizeof(cameraPoints) / sizeof(*cameraPoints); i++) {
		vec3 point = cameraPoints[i].getPoint();
		points.push_back(vec3(point.x, point.z, -point.y));
		vec3 dir = cameraPoints[i].getDerivative();
		vec3 pointDir = point + dir;
		points.push_back(vec3(pointDir.x, pointDir.z, -pointDir.y));
		points.push_back(vec3(point.x, point.z, -point.y));
	}
	
	/*
	for (float t = 0; t < 30; t+=0.1) {
		vec3 point = bSpline.calcPoint(t);
		points.push_back(vec3(point.x, point.z, -point.y));

		//cout << "Point: " << point.x << ", " << point.y << ", " << point.z << endl;
	}
	*/
	

	//cout << "points size: " << points.size() << endl;


	drawLineStrop(shader, VP, points);

	
}