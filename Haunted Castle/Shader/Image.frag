#version 430 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D image;

void main()
{
     vec3 result = texture(image, TexCoords).rgb;
     FragColor = vec4(result, 1.0);
}