#version 440

out vec4 fragment_color;
out vec4 bright_color;

in vec2 texCoord0;

struct PointLight
{
	vec3 position;
	vec3 color;
};

uniform int NR_OF_POINT_LIGHTS;   

uniform vec3 cameraPos;
uniform PointLight PointLights[256];

// Gbuffer variables
uniform sampler2D gPosition;
uniform sampler2D gDiffuse;
uniform sampler2D gNormal;
uniform sampler2D gSpecularAndHeight;
uniform sampler2D gAmbient;

// ShadowBuffer variables
uniform samplerCube shadowMap;
uniform float farPlane;

float calculateShadows(vec3 objPos)
{
	vec3 lightToObj = objPos - PointLights[0].position.xyz;
	float depthValue = texture(shadowMap, lightToObj).x;
	depthValue *= farPlane;
	float currDepth = length(lightToObj);
	float bias = 0.15f; // Avoids "Shadow Acne"

	float shadow;
	if(currDepth - bias > depthValue)
		shadow = 1.0f;
	else
		shadow = 0.0f;

	return shadow;
}

void main()
{
	// Sample from the gBuffer
	vec3 pixelPos = texture2D(gPosition, texCoord0).xyz;
	vec3 materialColor = texture2D(gDiffuse, texCoord0).rgb;
	vec3 normal = texture2D(gNormal, texCoord0).xyz;
	float specular = texture2D(gSpecularAndHeight, texCoord0).r;
	float heightMap = texture2D(gSpecularAndHeight, texCoord0).b;
	vec3 ambient = texture2D(gAmbient, texCoord0).rgb;

	// Attenuation
	float attenuation;
	float distancePixelToLight;

	// Ambient
	//vec4 ambient = vec4(0.1f,0.1f,0.1,1.0f) * vec4(materialColor.rgb, 1.0f);
	
	// Diffuse
	vec3 lightDir;
	vec4 diffuse;
	float alpha;
	
	// Specular
	vec3 vecToCam;
	vec4 reflection;
	vec4 finalSpecular;
	float shininess = 30;

	for(int i = 0; i < NR_OF_POINT_LIGHTS; i++)
	{
		// Diffuse
		lightDir = normalize(PointLights[i].position.xyz - pixelPos.xyz);
		alpha = dot(normal.xyz,lightDir);
		diffuse += vec4(materialColor.rgb,1.0f) * vec4(PointLights[i].color.rgb, 1.0f) * max(alpha, 0);

		// Specular
		vecToCam = normalize(vec3(cameraPos.xyz - pixelPos.xyz));	
		// Source: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/reflect.xhtml
		reflection = reflect(vec4(-lightDir.xyz, 0.0f), vec4(normal.xyz,1.0f));
		finalSpecular += specular * vec4(PointLights[i].color.rgb, 1.0f) * pow(max(dot(reflection.xyz, vecToCam.xyz),0), shininess);
		finalSpecular.w = 1.0;

		// attenuation
		distancePixelToLight = length(PointLights[i].position - pixelPos);
		attenuation = 1.0f / (1.0f + (0.1 * distancePixelToLight)+ (0.01 * pow(distancePixelToLight, 2)));
	}

	float shadow = calculateShadows(pixelPos);

	vec4 finalColor = ambient + ((1 - shadow) * attenuation*(diffuse + finalSpecular));
	finalColor = min(vec4(1.0f,1.0f,1.0f,1.0f), finalColor);

	fragment_color = vec4(finalColor.xyz, 1.0f);

	// Calculate brightness (used for bloom)
	vec3 lumaVec = vec3(0.2126, 0.7152, 0.0722);
	float brightness = dot(fragment_color.rgb, lumaVec.xyz);

	if(brightness > 0.84f)
	{
		bright_color = vec4(fragment_color.rgb, 1.0f);
	}
	else
	{
		bright_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}