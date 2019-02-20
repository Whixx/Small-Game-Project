#version 440

in vec2 texCoords;
in vec4 posWorld1;
in mat3 TBN;

out vec3 WorldPosOut;
out vec3 TextureRGBOut;
out vec3 WorldNormalOut;
out vec3 TextureSpecularOut;

uniform sampler2D TextureDiffuse;
uniform sampler2D TextureNormal;
uniform sampler2D TextureSpecular;

uniform float illuminated;

void main()
{	
	// Deferred rendering Geometry pass
	WorldPosOut = posWorld1.xyz;
	TextureRGBOut = texture2D(TextureDiffuse,texCoords).xyz;

	// Sample the normalMap
	WorldNormalOut = texture2D(TextureNormal, texCoords).xyz;
	// Adjust the values to the range [-1,1], (range is originally [0,1])
	WorldNormalOut = normalize((WorldNormalOut * 2.0) - 1.0);
	// Transform the normal from tangent space to world space
	WorldNormalOut = normalize(TBN * WorldNormalOut);

	TextureSpecularOut.r = texture2D(TextureSpecular, texCoords).r;
	TextureSpecularOut.g = 0;
	TextureSpecularOut.b = 0;

}