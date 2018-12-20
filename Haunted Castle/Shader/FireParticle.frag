#version 430 core
out vec4 FragColor;

in vec2 UV;
in vec4 Color;
in float TTL;

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

	int rows = 5;
	int cols = 5;

	int texIndex = int(TTL*float(rows*cols));
	
    int col = texIndex % cols;
    int row = texIndex / rows;
	float textXOffset = float(col) / float(cols);
    float textYOffset = float(row) / float(rows);

	vec2 UVtex = vec2(UV.x / float(cols) + textXOffset, UV.y / float(rows) + textYOffset);
	
	vec3 color = texture( fireTexture, UVtex ).rgb;
	float alpha = length(color);
	FragColor = vec4(color, alpha*0.03);

	//FragColor = vec4(vec3(float(row)/5.0),1);
}  