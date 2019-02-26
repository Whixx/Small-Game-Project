#version 440

// define the number of CPs in the output patch
layout(vertices = 3) out;

uniform vec3 EyeWorldPos;

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

// Decide how detailed the mesh should be depending on the distance to the player
float GetTessLevel(float dist0, float dist1)
{
    float avgDist = (dist0 + dist1) / 2.0;

    if (avgDist <= 8.0) 
    {
        return 100.0;
    }
    /*else if (avgDist <= 15.0) 
    {
        return 5.0;
    }*/
    else 
    {
        return 0.01;
    }
}

void main()
{
    // Set the control points of the output patch
    normal_ES_in[gl_InvocationID] = normal_CS_in[gl_InvocationID];
    tangent_ES_in[gl_InvocationID] = tangent_CS_in[gl_InvocationID];
    texCoord0_ES_in[gl_InvocationID] = texCoord0_CS_in[gl_InvocationID];
    posWorld0_ES_in[gl_InvocationID] = posWorld0_CS_in[gl_InvocationID];
    type0_ES_in[gl_InvocationID] = type0_CS_in[gl_InvocationID];

    // Calculate the distance from the camera to the three control points
    float EyeToVertexDist0 = distance(EyeWorldPos, posWorld0_ES_in[0]);
    float EyeToVertexDist1 = distance(EyeWorldPos, posWorld0_ES_in[1]);
    float EyeToVertexDist2 = distance(EyeWorldPos, posWorld0_ES_in[2]);

    // Calculate the tessellation levels
    gl_TessLevelOuter[0] = GetTessLevel(EyeToVertexDist1, EyeToVertexDist2);
    gl_TessLevelOuter[1] = GetTessLevel(EyeToVertexDist2, EyeToVertexDist0);
    gl_TessLevelOuter[2] = GetTessLevel(EyeToVertexDist0, EyeToVertexDist1);
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}