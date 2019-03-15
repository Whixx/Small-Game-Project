#version 440 

// define the number of CPs in the output patch
layout (vertices = 3) out;

// attributes of the input CPs
in vec3 normal_CS_in[];
in vec3 tangent_CS_in[];
in vec2 texCoord0_CS_in[];
in vec3 posWorld0_CS_in[];
in float type0_CS_in[];

// attributes of the output CPs
out vec3 normal_ES_in[];
out vec3 tangent_ES_in[];
out vec2 texCoord0_ES_in[];
out vec3 posWorld0_ES_in[];
out float type0_ES_in[];

// Decide how detailed the mesh should be
float GetTessLevel()
{
    float divide = 50;
    return divide;
}

void main()
{
    // Set the control points of the output patch
    normal_ES_in[gl_InvocationID] = normal_CS_in[gl_InvocationID];
    tangent_ES_in[gl_InvocationID] = tangent_CS_in[gl_InvocationID];
    texCoord0_ES_in[gl_InvocationID] = texCoord0_CS_in[gl_InvocationID];
    posWorld0_ES_in[gl_InvocationID] = posWorld0_CS_in[gl_InvocationID];
    type0_ES_in[gl_InvocationID] = type0_CS_in[gl_InvocationID];

    // Calculate the tessellation levels
    gl_TessLevelOuter[0] = GetTessLevel();
    gl_TessLevelOuter[1] = GetTessLevel();
    gl_TessLevelOuter[2] = GetTessLevel();
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}