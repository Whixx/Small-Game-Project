#version 440

uniform int width;
uniform int height;
uniform vec2 drawOrder[256];
uniform vec3 cameraPos;

void main()
{
	int i = int(drawOrder[gl_VertexID].x) + int(cameraPos.x) - (width / 2);
	int j = int(drawOrder[gl_VertexID].y) + int(cameraPos.z) - (height / 2);

	gl_Position = vec4(i, j, 0.0, 1.0);
}