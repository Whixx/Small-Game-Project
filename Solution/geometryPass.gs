#version 440

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 texCoord0[];

out vec2 texCoords;
out vec4 posWorld1;
out mat3 TBN;

uniform mat4 transformationMatrix; 
uniform mat4 WorldMatrix; 

uniform vec3 cameraPosGP;

// functions
vec4 createNormalWorld();
vec4 createTangent();

void main()
{	
	vec4 normalWorld = createNormalWorld();
	vec4 tangent = WorldMatrix * createTangent();
	vec4 bitangent = vec4(cross(normalWorld.xyz, tangent.xyz), 0.0f);

	vec4 posWorld[3];
	for(int i = 0; i < 3; i++)
	{
		posWorld[i] = WorldMatrix * gl_in[i].gl_Position;
	}

	// Backface culling using geometry shader
	float alpha = dot(normalWorld.xyz,cameraPosGP.xyz - posWorld[1].xyz);
	if(alpha > 0)
	{
		for(int i = 0; i < 3; i++)
		{
			// Create the matrix that will transform the normalMap to tangent space.
			vec4 T = normalize(vec4(tangent.xyz, 0.0));											// Tangent is already in world space
			vec4 B = normalize(vec4(WorldMatrix * vec4(bitangent.xyz, 0.0)));					// Bitangent in world space
			vec4 N = vec4(normalWorld.xyz, 0.0);												// Normal is already in world space
			

			gl_Position = transformationMatrix * gl_in[i].gl_Position;
			texCoords = texCoord0[i];
			posWorld1 = posWorld[i];
			TBN = mat3(T.xyz, B.xyz, N.xyz);
			EmitVertex();
		}
		EndPrimitive();
	}	
}

vec4 createNormalWorld()
{
	vec4 vectorUsedForNormal1 = gl_in[1].gl_Position - gl_in[0].gl_Position;
	vec4 vectorUsedForNormal2 = gl_in[2].gl_Position - gl_in[0].gl_Position;  

	vec4 normal = vec4(normalize(cross(vectorUsedForNormal1.xyz, vectorUsedForNormal2.xyz)), 0.0f);
	vec4 normalWorld = normalize(WorldMatrix * normal);

	return vec4(normalWorld.xyz, 0.0f);	
}

vec4 createTangent()
{
	vec2 UVedge1 = texCoord0[1] - texCoord0[0];
	vec2 UVedge2 = texCoord0[2] - texCoord0[0];
	vec4 edge1 = gl_in[1].gl_Position - gl_in[0].gl_Position;
	vec4 edge2 = gl_in[2].gl_Position - gl_in[0].gl_Position;

	vec4 tangent = vec4((edge1.xyz * UVedge2.y - edge2.xyz * UVedge1.y) / (UVedge1.x * UVedge2.y - UVedge1.y * UVedge2.x), 0.0f);
	tangent = normalize(tangent);

	return vec4(tangent.xyz, 0.0f);	
}
