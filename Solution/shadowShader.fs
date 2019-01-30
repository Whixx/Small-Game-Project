#version 440

in vec4 worldPos;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
	float lightDistance = length(worldPos.xyz - lightPos);
	// lightDistance should be in [0,1] range adjusted to the farPlane
	lightDistance = lightDistance / farPlane;
	gl_FragDepth = lightDistance;
}