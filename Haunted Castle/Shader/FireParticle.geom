#version 430 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 V;
uniform mat4 VP;

out vec2 UV;
out vec4 Color;

void main() {   

	vec4 color = vec4(1, 0, 0, 1);

	vec3 right = vec3(V[0][0], 
                      V[1][0], 
                      V[2][0]);

	vec3 up = vec3(V[0][1], 
				   V[1][1], 
				   V[2][1]);

    vec3 P = gl_in[0].gl_Position.xyz;

	float size = 1;

	vec3 va = P - (right + up) * size;
	gl_Position = VP * vec4(va, 1.0);
	UV = vec2(0.0, 0.0);
	Color = color;
	EmitVertex();  
	
	vec3 vb = P - (right - up) * size;
	gl_Position = VP * vec4(vb, 1.0);
	UV = vec2(0.0, 1.0);
	Color = color;
	EmitVertex();		
	
	vec3 vd = P + (right - up) * size;
	gl_Position = VP * vec4(vd, 1.0);
	UV = vec2(1.0, 0.0);
	Color = color;
	EmitVertex();  
	
	vec3 vc = P + (right + up) * size;
	gl_Position = VP * vec4(vc, 1.0);
	UV = vec2(1.0, 1.0);
	Color = color;
	EmitVertex();  
	
	EndPrimitive();  


	/*
	//							horizontal (+ nach rechts), vertical (+ ist rauf)
	
	// Links unten
    gl_Position = gl_in[0].gl_Position + vec4( -size, -size, 0.0, 0.0);
    EmitVertex();

	// Links oben
    gl_Position = gl_in[0].gl_Position + vec4( -size, size, 0.0, 0.0); 
    EmitVertex();

	// Rechts oben
    gl_Position = gl_in[0].gl_Position + vec4( size, size, 0.0, 0.0);
    EmitVertex();
    
    EndPrimitive();
	
	// Links unten
    gl_Position = gl_in[0].gl_Position + vec4( -size, -size, 0.0, 0.0);
    EmitVertex();

	// Rechts oben
    gl_Position = gl_in[0].gl_Position + vec4( size, size, 0.0, 0.0);
    EmitVertex();

	// Rechts unten
    gl_Position = gl_in[0].gl_Position + vec4( size, -size, 0.0, 0.0); 
    EmitVertex();
    
    EndPrimitive();
	*/
}