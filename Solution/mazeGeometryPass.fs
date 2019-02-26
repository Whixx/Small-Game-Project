#version 440
precision highp int;

in vec2 texCoord0;
in vec3 posWorld0;
in mat3 TBN0;
in float type0;

out vec3 WorldPosOut;
out vec3 TextureRGBOut;
out vec3 WorldNormalOut;
out vec3 TextureSpecularAndHeightOut;
out vec3 AmbientOut;

uniform sampler2D TextureDiffuse[2];
uniform sampler2D TextureNormal[2];
uniform sampler2D TextureSpecular[2];
uniform sampler2D TextureAmbient[2];

uniform float shininess[2];

void main()
{	
	uint type = uint(type0);
	// Deferred rendering Geometry pass
	WorldPosOut = posWorld0.xyz;
	TextureRGBOut = texture2D(TextureDiffuse[type],texCoord0).xyz;

	// Sample the normalMap
	WorldNormalOut = texture2D(TextureNormal[type], texCoord0).xyz;
	// Adjust the values to the range [-1,1], (range is originally [0,1])
	WorldNormalOut = normalize((WorldNormalOut * 2.0) - 1.0);
	// Transform the normal from tangent space to world space
	WorldNormalOut = normalize(TBN0 * WorldNormalOut);

	TextureSpecularAndHeightOut.r = texture2D(TextureSpecular[type], texCoord0).r;
	TextureSpecularAndHeightOut.g = 16.0;//shininess;
	//TextureSpecularAndHeightOut.b = 0;

	AmbientOut = texture2D(TextureAmbient[type], texCoord0).rgb;
}