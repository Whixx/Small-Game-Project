#version 440

in vec2 texCoord0;
in vec3 posWorld0;
in mat3 TBN0;

layout(location = 0) out vec3 WorldPosOut;
layout(location = 1) out vec3 TextureRGBOut;
layout(location = 2) out vec3 WorldNormalOut;
layout(location = 3) out vec3 TextureSpecularAndHeightOut;
layout(location = 4) out vec3 EmissiveOut;
layout(location = 5) out vec3 AmbientOut;

uniform sampler2D TextureDiffuse;
uniform sampler2D TextureNormal;
uniform sampler2D TextureSpecular;
uniform sampler2D TextureEmissive;

uniform float shininess;
uniform vec3 ambient;

void main()
{	
	// Deferred rendering Geometry pass
	WorldPosOut = posWorld0.xyz;
	TextureRGBOut = texture2D(TextureDiffuse,texCoord0).xyz;

	// Sample the normalMap
	WorldNormalOut = texture2D(TextureNormal, texCoord0).xyz;
	// Adjust the values to the range [-1,1], (range is originally [0,1])
	WorldNormalOut = normalize((WorldNormalOut * 2.0) - 1.0);
	// Transform the normal from tangent space to world space
	WorldNormalOut = normalize(TBN0 * WorldNormalOut);

	TextureSpecularAndHeightOut.r = texture2D(TextureSpecular, texCoord0).r;
	TextureSpecularAndHeightOut.g = shininess;
	AmbientOut = ambient;

	EmissiveOut = texture2D(TextureEmissive, texCoord0).rgb;
}