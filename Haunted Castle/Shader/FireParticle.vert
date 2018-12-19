#version 430 core
layout(location = 0) in vec4 xyzs;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP;

void main()
{
	float particleSize = xyzs.w;
	vec3 particleCenter_wordspace = xyzs.xyz;
	
	vec3 vertexPosition_worldspace = particleCenter_wordspace;

	gl_Position = vec4(vertexPosition_worldspace, 1.0f);
}