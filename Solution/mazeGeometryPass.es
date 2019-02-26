#version 440

layout(triangles, equal_spacing, ccw) in;

uniform mat4 VP;
uniform sampler2D TextureHeight[2];

in vec3 normal_ES_in[];
in vec3 tangent_ES_in[];
in vec2 texCoord0_ES_in[];
in vec3 posWorld0_ES_in[];
in float type0_ES_in[];

out mat3 TBN0_FS_in;
out vec2 texCoord0_FS_in;
out vec3 posWorld0_FS_in;
out float type0_FS_in;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
   	return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
   	return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

void main()
{
    // Set displacementfactor
    float dispFactor = 0.04f;

   	// Interpolate the attributes of the output vertex using the barycentric coordinates
   	texCoord0_FS_in = interpolate2D(texCoord0_ES_in[0], texCoord0_ES_in[1], texCoord0_ES_in[2]);
   	posWorld0_FS_in = interpolate3D(posWorld0_ES_in[0], posWorld0_ES_in[1], posWorld0_ES_in[2]);

    vec3 normal = interpolate3D(normal_ES_in[0], normal_ES_in[1], normal_ES_in[2]);
   	normal = normalize(normal);

    vec3 tangent = interpolate3D(tangent_ES_in[0], tangent_ES_in[1], tangent_ES_in[2]);
    tangent = normalize(tangent);

    type0_FS_in = type0_ES_in[0];
    uint type = uint(type0_FS_in);

    // Calculate bitangent
	vec3 bitangent = cross(normal, tangent);

	// Create the matrix that will transform the normalMap to tangent space.
	TBN0_FS_in = mat3(tangent, bitangent, normal);

    // Displace the vertex along the normal
   	float displacement = texture(TextureHeight[type], texCoord0_FS_in.xy).x;
   	posWorld0_FS_in += normal * displacement * dispFactor;
   	gl_Position = VP * vec4(posWorld0_FS_in, 1.0);
}