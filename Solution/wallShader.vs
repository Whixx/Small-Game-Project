#version 440

uniform vec2 drawOrder[256];
uniform vec3 cameraPos;

void main()
{
	int i = int(drawOrder[gl_VertexID].x) + int(cameraPos.x);
	int j = int(drawOrder[gl_VertexID].y) + int(cameraPos.z);

	gl_Position = vec4(i, j, 0.0, 1.0);
}