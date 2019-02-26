#version 440
precision highp int;

in vec2 texCoord0_FS_in;
in vec3 posWorld0_FS_in;
in mat3 TBN0_FS_in;
in float type0_FS_in;

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
	uint type = uint(type0_FS_in);

	// Deferred rendering Geometry pass
	WorldPosOut = posWorld0_FS_in.xyz;
	TextureRGBOut = texture2D(TextureDiffuse[type], texCoord0_FS_in).xyz;

	// Sample the normalMap
	WorldNormalOut = texture2D(TextureNormal[type], texCoord0_FS_in).xyz;
	// Adjust the values to the range [-1,1], (range is originally [0,1])
	WorldNormalOut = normalize((WorldNormalOut * 2.0) - 1.0);
	// Transform the normal from tangent space to world space
	WorldNormalOut = normalize(TBN0_FS_in * WorldNormalOut);

	TextureSpecularAndHeightOut.r = texture2D(TextureSpecular[type], texCoord0_FS_in).r;
	TextureSpecularAndHeightOut.g = 16.0;//shininess;
	//TextureSpecularAndHeightOut.b = 0;

	AmbientOut = texture2D(TextureAmbient[type], texCoord0_FS_in).rgb;
}