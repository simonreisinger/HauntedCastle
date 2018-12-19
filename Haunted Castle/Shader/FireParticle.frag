#version 430 core
out vec4 FragColor;

in vec2 UV;
in vec4 Color;

uniform sampler2D fireTexture;

void main()
{
	/*
	float sigma = 0.4;
	vec2 mu = vec2(0.5, 0.5);
	vec2 x = 2*(UV - mu);
	float gaussian = exp(-dot(x, x)/(2*sigma*sigma));
	
	vec3 tex = texture( fireTexture, UV ).r * Color.rgb;
	
	FragColor = vec4(tex.rgb, gaussian);
	*/
	
	vec3 color = texture( fireTexture, UV ).rgb;
	float alpha = length(color);
	FragColor = vec4(color, alpha*0.1);
}  