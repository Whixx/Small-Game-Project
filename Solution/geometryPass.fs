#version 440

in vec2 texCoords;
in vec3 posWorld1;
in mat3 TBN;

out vec3 WorldPosOut;
out vec3 TextureRGBOut;
out vec3 WorldNormalOut;
out vec3 TextureSpecularAndHeightOut;
out vec3 AmbientOut;

uniform sampler2D TextureDiffuse;
uniform sampler2D TextureNormal;
uniform sampler2D TextureSpecular;
uniform sampler2D TextureAmbient;

uniform float shininess;

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

	TextureSpecularAndHeightOut.rgb = texture2D(TextureSpecular, texCoords).rgb;
	TextureSpecularAndHeightOut.g = shininess;
	TextureSpecularAndHeightOut.b = 0;

	AmbientOut = texture2D(TextureAmbient, texCoords).rgb;
}