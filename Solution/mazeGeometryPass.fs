#version 440

in vec2 texCoords;
in vec3 posWorld1;
in mat3 TBN;
in float type1;

out vec3 WorldPosOut;
out vec3 TextureRGBOut;
out vec3 WorldNormalOut;
out vec3 TextureSpecularAndHeightOut;
out vec3 AmbientOut;

uniform sampler2D TextureDiffuse[2];
uniform sampler2D TextureNormal[2];
uniform sampler2D TextureSpecular[2];
uniform sampler2D TextureAmbient[2];

uniform float shininess;

void main()
{	
	int type = int(type1);
	// Deferred rendering Geometry pass
	WorldPosOut = posWorld1.xyz;
	TextureRGBOut = texture2D(TextureDiffuse[type],texCoords).xyz;

	// Sample the normalMap
	WorldNormalOut = texture2D(TextureNormal[type], texCoords).xyz;
	// Adjust the values to the range [-1,1], (range is originally [0,1])
	WorldNormalOut = normalize((WorldNormalOut * 2.0) - 1.0);
	// Transform the normal from tangent space to world space
	WorldNormalOut = normalize(TBN * WorldNormalOut);

	//TextureSpecularAndHeightOut.rgb = texture2D(TextureSpecular, texCoords).rgb;
	//TextureSpecularAndHeightOut.g = shininess;
	//TextureSpecularAndHeightOut.b = 0;
	TextureSpecularAndHeightOut.rgb = vec3(1.0f, 1.0f, 1.0f);

	AmbientOut = texture2D(TextureAmbient[type], texCoords).rgb;
}