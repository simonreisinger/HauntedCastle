#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 depthMVP;

in vec3 position;
in vec3 normal;
in vec2 uv;

uniform vec3 light;
uniform vec3 cameraPosition;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform int hasTexture;

uniform mat4 model;
uniform mat4 updateTrans;

void main(){
	gl_Position =  depthMVP * vec4(vertexPosition_modelspace,1);
}
