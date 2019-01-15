#pragma once

#include "SceneObject.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "CameraPoint.hpp"
#include "Const.hpp"
#include "Shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace cgue {
	class Camera : public SceneObject
	{
	public:
		Camera();
		virtual ~Camera();

		mat4x4 getCameraModel();
		mat4x4 getInverseCameraModel();
		vec3 getCameraPos();
		vec3 getCameraUp();
		vec3 getCameraLookAt();
		void advance(float tf);
		void drawCurve(Shader* shader, mat4x4 VP);
		bool getAutomaticCameraMovementActivated();
		void changeAutomaticCameraMovementActivatedState();
	private:
		float t = 0;

		bool automaticCameraMovementActivated = true;

		vec3 p, d;

		GLuint vao;
		GLuint vb;

		bool initiatializedLineStrop = false;
		void initLineStrop(Shader *shader, mat4x4 VP, vector<vec3> points);
		void drawLineStrop(Shader *shader, mat4x4 VP, vector<vec3> points);
	};
}