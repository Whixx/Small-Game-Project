#version 440

uniform int width;
uniform int height;
uniform vec2 translation;
uniform int scaleUVXZ;

uniform vec2 drawOrder[256];
uniform vec3 cameraPos;

void main()
{
    bool isAWall = true;

    // NOT NEEDED Transform world coords to texture coords. ( 1 pixel on texture corresponds to 1.0, origo is (0, 0) for both spaces

    int i = int(drawOrder[gl_VertexID].x) + int(cameraPos.x);
    int j = int(drawOrder[gl_VertexID].y) + int(cameraPos.z);

    // The maze can be translated
    i -= int(translation.x);
    j -= int(translation.y);

    // The maze can be scaled
    i /= scaleUVXZ;
    j /= scaleUVXZ;

    i += width / 2;
    j += height / 2;

    gl_Position = vec4(i, j, 0.0, 1.0);
}