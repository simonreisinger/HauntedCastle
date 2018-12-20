#version 330 core

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
uniform sampler2D myTextureSampler;
uniform vec3 Torch1Position_worldspace;
uniform vec3 Torch2Position_worldspace;
uniform int hasTexture;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform sampler2DShadow shadowMap;
uniform vec3 SunDirection_worldspace;

	//////////////// calculate point shadow ///////////////
	uniform samplerCube depthMap;
	uniform vec3 viewPos;
	uniform vec3 lightPos;
	uniform float far_plane;
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

float ShadowCalculation(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // use the light to fragment vector to sample from the depth map    
    //float closestDepth = texture(depthMap, fragToLight).r;
    //float closestDepth = texture(depthMap, vec3(gl_FragCoord.xy / vec2(1600,1600),1)).r;
    float closestDepth = texture(depthMap, vec3(0.5,0.3,1)).r;

    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float bias = 0.05; 
    float shadow = currentDepth/far_plane; // -  bias > closestDepth ? 1.0 : 0.0;
    //return shadow;
    return closestDepth/ 255.0;
}

void main(){
	
	// Use Texture or Material Color
	vec3 MaterialDiffuseColor;
	
	if(hasTexture == 1)
	{
		MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;
	}
	else
	{
		MaterialDiffuseColor = diffuseColor;
	}

	//////////////// calculate point shadow ///////////////
	// float shadow = ShadowCalculation(fs_in.FragPos);
	//float shadow /*= 0; //*/ ShadowCalculation(Position_worldspace);
	float shadow = ShadowCalculation(FragPos); //TODO error
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
		visibility -= 0.25*(1.0-texture( shadowMap, vec3(ShadowCoord.xy + disk[i]/700.0,  (ShadowCoord.z-bias)/ShadowCoord.w) ));
	}

	
	vec3 nV = normalize( Normal_worldspace );
	vec3 lV = normalize( SunDirection_worldspace );
	float visibilityPosible = dot( nV, lV ) >= 0 ? 1 : 0;

	visibility = visibility * visibilityPosible;
	

	
	/*
	vec3 fragToLight = FragPos - lightPos;

	float currentDepth = length(fragToLight);

	float closestDepth = texture(depthMap, fragToLight).r;
	*/


	vec3 fragToLight = FragPos - lightPos; 

	vec3 fragToLight2 = vec3(fragToLight.z, fragToLight.x, fragToLight.y);

	//vec3 fragToLight2 = vec3(fragToLight.x, fragToLight.z, fragToLight.y);

    float closestDepth = texture(depthMap, fragToLight2).r;
	closestDepth *= far_plane;
	float currentDepth = length(fragToLight2); 

	float biasx = 0.001;//5; 
	float xxxxxxx = currentDepth -  biasx > closestDepth ? 0.0 : 1.0; 

	//float xxxxxxx = texture(depthMap, Position_worldspace - lightPos).r;

	//vec4 FragColorShadow = vec4(vec3(closestDepth / far_plane), 1.0);  



	//*
	FragColor = vec4(
		// Ambient
		MaterialAmbientColor + 0.5 * vec3(xxxxxxx) // TODO is wrong here must be changed
		/*
		vec3(MaterialAmbientColor* (1.0+3.0*xxxxxxx))

		(1.0 - shadow) * 		
		(// Diffuse Torch 1
		MaterialDiffuseColor * I1 * cosTheta1 +
		// Specular Torch 1
		MaterialSpecularColor * I1 * pow(cosAlpha1, 5) +

		
		// Diffuse Torch 2
		MaterialDiffuseColor * I2 * cosTheta2 +
		// Specular Torch 2
		MaterialSpecularColor * I2 * pow(cosAlpha2, 5)) +
		// Window
		MaterialDiffuseColor * visibility
		*/
	, 1);
	// */
	//FragColor = vec4(lightPos.xyz,1);
	//     //float closestDepth = texture(depthMap, vec3(gl_FragCoord.xy / vec2(1600,1600),1)).r;


	/*
	float xxxxxxx = texture(depthMap, FragPos - lightPos).r;// texture(depthMap, vec3(gl_FragCoord.xy / vec2(1600.0,1600.0),1)).r/2.0;

	FragColor = vec4(xxxxxxx, xxxxxxx, xxxxxxx, 1.0);
	*/
	/*
	float shadowX = ShadowCalculation(FragPos);                      
    vec3 lighting = (MaterialAmbientColor * (1.0 - shadowX));    
    
    FragColor = vec4(lighting, 1.0);
	*/

}