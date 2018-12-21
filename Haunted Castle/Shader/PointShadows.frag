#version 330 core
in vec4 FragPos;

uniform vec3 flameCenterPosition;
uniform float pointShadowsFarPlane;

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - flameCenterPosition);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / pointShadowsFarPlane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}  