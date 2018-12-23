#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    hdrColor += bloomColor; // additive blending
    // tone mapping
	vec3(1.0) - exp(-hdrColor * exposure);
    vec3 result = vec3(1.0,0.0,0.0);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(hdrColor, 1);// vec4(texture(scene, TexCoords).rgb, 1);//
    //FragColor = vec4(bloomColor, 1);
	//FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}