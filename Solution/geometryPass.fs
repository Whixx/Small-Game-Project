#version 440

in vec2 texCoords;
in vec4 posWorld1;
in mat3 TBN;

out vec3 WorldPosOut;
out vec3 TextureRGBOut;
out vec3 WorldNormalOut;

uniform sampler2D texture;
uniform sampler2D normalMap;

uniform float illuminated;

void main()
{	
	// Deferred rendering Geometry pass
	WorldPosOut = posWorld1.xyz;
	TextureRGBOut = texture2D(texture,texCoords).xyz * illuminated;

	// Sample the normalMap
	WorldNormalOut = texture2D(normalMap, texCoords).xyz;
	// Adjust the values to the range [-1,1], (range is originally [0,1])
	WorldNormalOut = normalize((WorldNormalOut * 2.0) - 1.0);
	// Transform the normal from tangent space to world space
	WorldNormalOut = normalize(TBN * WorldNormalOut);
}