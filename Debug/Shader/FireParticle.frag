#version 430 core
out vec4 FragColor;

in vec2 UV;
in vec4 Color;
in float TTL;

uniform sampler2D fireTexture;

uniform float flameIntensity;

vec2 spriteUV()
{
	int rows = 5;
	int cols = 5;

	int texIndex = int(TTL*float(rows*cols));
	
    int col = texIndex % cols;
    int row = texIndex / rows;
	float textXOffset = float(col) / float(cols);
    float textYOffset = float(row) / float(rows);

	return vec2(UV.x / float(cols) + textXOffset, UV.y / float(rows) + textYOffset);
}

void main()
{
	vec2 spriteUV = spriteUV();
	
	vec3 color = texture( fireTexture, spriteUV ).rgb;
	FragColor = vec4(color, 0.04 * TTL * flameIntensity);
}  