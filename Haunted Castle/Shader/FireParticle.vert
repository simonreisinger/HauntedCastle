#version 430 core
layout(location = 0) in vec4 xyzs;
//layout(location = 1) in vec4 Position;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP;

void main()
{
	gl_Position = xyzs;
}