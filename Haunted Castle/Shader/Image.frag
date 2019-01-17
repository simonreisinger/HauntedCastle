#version 430 core
out vec4 FragColor;
in vec2 TexCoords;

uniform float progress;

uniform sampler2D image;

void main()
{
     vec3 result = texture(image, TexCoords).rgb;
	 if (TexCoords.y >= 0.15 && TexCoords.y <= 0.2 && progress > 0 && TexCoords.x <= progress) {
		result = 0.2 * vec3(1, 1, 1) + 0.8 * result;
	 }
     FragColor = vec4(result, 1.0);
}