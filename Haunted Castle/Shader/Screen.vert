#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;

out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 Torch1Direction_cameraspace;
out vec3 Torch2Direction_cameraspace;
out vec4 ShadowCoord;

	//////////////// calculate point shadow ///////////////
	out vec3 FragPos;

	//////////////////////////////////////////////////////

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat4 directionalShadowsDepthVP;
uniform vec3 Torch1Position_worldspace;
uniform vec3 Torch2Position_worldspace;
uniform vec3 SunDirection_worldspace;

void main(){

	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

	FragPos = (M * vec4(vertexPosition_modelspace,1)).xyz; // vec3(model * vec4(aPos, 1.0));

	mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
						 0.0, 0.5, 0.0, 0.0,
						 0.0, 0.0, 0.5, 0.0,
						 0.5, 0.5, 0.5, 1.0);
	
	ShadowCoord = biasMatrix * directionalShadowsDepthVP * M * vec4(vertexPosition_modelspace,1);
	
	Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;
	
	vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
	
	vec3 Torch1Position_cameraspace = ( V * vec4(Torch1Position_worldspace,1)).xyz;
	Torch1Direction_cameraspace = Torch1Position_cameraspace + EyeDirection_cameraspace;
	
	vec3 Torch2Position_cameraspace = ( V * vec4(Torch2Position_worldspace,1)).xyz;
	Torch2Direction_cameraspace = Torch2Position_cameraspace + EyeDirection_cameraspace;
	
	Normal_worldspace = ( M * vec4(vertexNormal_modelspace,0)).xyz;
	
	Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz;
	
	UV = vertexUV;
}

