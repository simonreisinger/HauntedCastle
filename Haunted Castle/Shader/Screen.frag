#version 430 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 Torch1Direction_cameraspace;
in vec3 Torch2Direction_cameraspace;
in vec4 ShadowCoord;


layout (location = 0) out vec4 FragColor;

// Values that stay constant for the whole mesh.
uniform sampler2D modelTexture; // modelTexture
uniform vec3 Torch1Position_worldspace;
uniform vec3 Torch2Position_worldspace;
uniform float flameIntensity[2];
uniform int hasTexture;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform sampler2DShadow directionalShadowsDepthMap;
uniform vec3 SunDirection_worldspace;

	//////////////// calculate point shadow ///////////////
	uniform samplerCube pointShadowsDepthCubeMap1;
	uniform samplerCube pointShadowsDepthCubeMap2;
	uniform vec3 viewPos;
	uniform vec3 flameCenterPosition1;
	uniform vec3 flameCenterPosition2;
	uniform float pointShadowsFarPlane;
	in vec3 FragPos;
	//////////////////////////////////////////////////////

vec2 disk[16] = vec2[]( 
   vec2( -0.94202634, -0.39907226 ), 
   vec2( 0.94558508, -0.76891735 ), 
   vec2( -0.094184211, -0.92938780 ), 
   vec2( 0.34495848, 0.29387650 ), 
   vec2( -0.91588491, 0.45771522 ), 
   vec2( -0.81544152, -0.87912464 ), 
   vec2( -0.38277463, 0.27676836 ), 
   vec2( 0.97484401, 0.75648380 ), 
   vec2( 0.44323319, -0.97511562 ), 
   vec2( 0.53742979, -0.47373419 ), 
   vec2( -0.26496815, -0.41893133 ), 
   vec2( 0.79197508, 0.19090191 ), 
   vec2( -0.24188838, 0.99706499 ), 
   vec2( -0.81409960, 0.91437588 ), 
   vec2( 0.19984118, 0.78641372 ), 
   vec2( 0.14383159, -0.14100800 ) 
);

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

// TODO aufraumen
float ShadowCalculation(vec3 fragPos, vec3 flameCenterPosition, samplerCube pointShadowsDepthCubeMap)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - flameCenterPosition;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows
    // PCF
    float shadow = 0.0;
    float bias = 0.25;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / pointShadowsFarPlane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(pointShadowsDepthCubeMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= pointShadowsFarPlane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
        
    return shadow;
}


void main(){
	
	// Use Texture or Material Color
	vec3 MaterialDiffuseColor;
	
	if(hasTexture == 1)
	{
		MaterialDiffuseColor = texture( modelTexture, UV ).rgb;
	}
	else
	{
		MaterialDiffuseColor = diffuseColor;
	}

	//////////////// calculate point shadow ///////////////
	float shadow1 = ShadowCalculation(FragPos, flameCenterPosition1, pointShadowsDepthCubeMap1);
	float shadow2 = ShadowCalculation(FragPos, flameCenterPosition2, pointShadowsDepthCubeMap2);
	//////////////////////////////////////////////////////

	float AmbientIntensity = 0.1;

	vec3 MaterialAmbientColor = AmbientIntensity * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = specularColor;




	vec3 n1 = normalize( Normal_cameraspace );
	vec3 l1 = normalize( Torch1Direction_cameraspace );
	float cosTheta1 = clamp( dot( n1, l1 ), 0, 1 );
	
	vec3 E1 = normalize(EyeDirection_cameraspace);
	vec3 R1 = reflect( -l1, n1 );
	float cosAlpha1 = clamp( dot( E1, R1 ), 0, 1 );

	float I1 = 1 / distance( Torch1Position_worldspace, Position_worldspace );



	vec3 n2 = normalize( Normal_cameraspace );
	vec3 l2 = normalize( Torch2Direction_cameraspace );
	float cosTheta2 = clamp( dot( n2, l2 ), 0, 1 );
	
	vec3 E2 = normalize(EyeDirection_cameraspace);
	vec3 R2 = reflect( -l2, n2 );
	float cosAlpha2 = clamp( dot( E2, R2 ), 0, 1 );

	float I2 = 1 / distance( Torch2Position_worldspace, Position_worldspace );

	float visibility=1.0;
	
	// Bias to get rid of Shadow acne
	float bias = 0.002;

	
	// PCF
	// 4 Times Stratified Poisson Sampling
	for (int i=0;i<4;i++){
		visibility -= 0.25*(1.0-texture( directionalShadowsDepthMap, vec3(ShadowCoord.xy + disk[i]/700.0,  (ShadowCoord.z-bias)/ShadowCoord.w) ));
	}

	
	vec3 nV = normalize( Normal_worldspace );
	vec3 lV = normalize( SunDirection_worldspace );
	float visibilityPosible = dot( nV, lV ) >= 0 ? 1 : 0;

	visibility = visibility * visibilityPosible;
	
	FragColor = vec4(
		// Ambient
		vec3(MaterialAmbientColor) +

		(1.0-shadow1) *	
		(
			flameIntensity[0]*
			(
				// Diffuse Torch 1
				MaterialDiffuseColor * I1 * cosTheta1 +
				// Specular Torch 1
				MaterialSpecularColor * I1 * pow(cosAlpha1, 5)
			)
			)
			 +
			(1.0-shadow2) *	
		(
			flameIntensity[1] *
			(
				// Diffuse Torch 2
				MaterialDiffuseColor * I2 * cosTheta2 +
				// Specular Torch 2
				MaterialSpecularColor * I2 * pow(cosAlpha2, 5)
			)
		) +
		// Window
		MaterialDiffuseColor * visibility
	, 1);
}