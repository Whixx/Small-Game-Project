#version 440

out vec4 fragment_color;
out vec4 bright_color;

in vec2 texCoord0;

struct PointLight
{
	vec3 position;
	vec3 color;
	float intensity;
};

uniform int NR_OF_POINT_LIGHTS;   

uniform vec3 cameraPos;
uniform PointLight PointLights[256];

// Gbuffer variables
uniform sampler2D gPosition;
uniform sampler2D gDiffuse;
uniform sampler2D gNormal;
uniform sampler2D gSpecularShininessHeight;
uniform sampler2D gAmbient;

// ShadowBuffer variables
uniform samplerCube shadowMap;
uniform float farPlane;

float calculateShadows(vec3 objPos, vec3 camPos, vec3 normal)
{
	vec3 lightToObj = objPos - PointLights[0].position.xyz;
    float viewDist = length(camPos - objPos);
	float currDepth = length(lightToObj);

    // Avoids "Shadow Acne"
    normal = normalize(normal);
	float bias = max(0.05 * (1 - dot(normal, lightToObj)), 0.005);
    
    // PCF (Percentage-closer filtering)
    // Change these two to optimize the look of the shadow
    int smoothness = 100; // Higher value will make the overall shadow "more compact"
    int samples = 20; // Higher value will lower the performance but make the smoothness' stages translate better

    float diskRadius = (1.0 + (viewDist / farPlane)) / smoothness; // Sharper the closer the camera is
    vec3 sampleOffsetDirections[20] = vec3[]
    (
        vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
        vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
        vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
        vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
        vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    ); 

    // Calculate shadow
	float shadow = 0.0f;
    for (int i = 0; i < samples; i++)
    {
        float closestDepth = texture(shadowMap, lightToObj + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= farPlane;
        if(currDepth - bias > closestDepth) 
        {
            shadow += 1.0f;
        }
    }
    shadow /= float(samples);

	return shadow;
}

void main()
{
	// Sample from the gBuffer
	vec3 pixelPos = texture2D(gPosition, texCoord0).xyz;
	vec3 materialColor = texture2D(gDiffuse, texCoord0).rgb;
	vec3 normal = texture2D(gNormal, texCoord0).xyz;
	vec3 ambientColor = texture2D(gAmbient, texCoord0).rgb;
	float specular = texture2D(gSpecularShininessHeight, texCoord0).r;
	float shininess = texture2D(gSpecularShininessHeight, texCoord0).g;
	

	// Attenuation
	float radius = 6;
	float a = 0.07;
	float b = 0.04;
	float attenuation;
	float distancePixelToLight;

	// Ambient
	//vec3 ambient = ambientColor.r * materialColor.rgb;
    vec3 ambient = 0.2 * materialColor.rgb;
	//ambient = vec3(1.0);
	
	// Diffuse
	vec3 lightDir;
	vec3 diffuse;
	float alpha;
	
	// Specular
	vec3 vecToCam;
	vec3 reflection;
	vec3 finalSpecular;
    vec3 halfwayDir;

	for(int i = 0; i < NR_OF_POINT_LIGHTS; i++)
	{
		distancePixelToLight = length(PointLights[i].position - pixelPos);

		if(distancePixelToLight < radius)
		{
			// Diffuse
			lightDir = normalize(PointLights[i].position.xyz - pixelPos.xyz);
			alpha = dot(normal.xyz,lightDir);
			diffuse += materialColor.rgb * PointLights[i].intensity * PointLights[i].color.rgb * max(alpha, 0);

			// Specular (Blinn-Phong) 
			vecToCam = normalize(cameraPos - pixelPos);
			halfwayDir = normalize(lightDir + vecToCam);
			float spec = pow(max(dot(normal, halfwayDir), 0.0), ceil(shininess));
			finalSpecular += specular * materialColor.rgb * PointLights[i].color.rgb * spec;

			// attenuation
			attenuation = 1.0f + a * distancePixelToLight + (b * -pow(distancePixelToLight, 2.0f));
		}
	}

	float shadow = calculateShadows(pixelPos, cameraPos, normal);

	vec4 finalColor = vec4(ambient + ((1 - shadow) * attenuation*(diffuse + finalSpecular)), 1.0f);
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