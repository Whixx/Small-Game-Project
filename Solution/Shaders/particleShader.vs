#version 440

layout(location = 0) in vec3 vertices;
layout(location = 1) in vec4 positionSize;
layout(location = 2) in vec4 color;

out vec4 finalColor;
out vec2 UV;

uniform vec3 cameraRightWorld;
uniform vec3 cameraUpWorld;
uniform mat4 viewProjectionMatrix;

void main()
{
	vec3 centerPosition = positionSize.xyz;
	float size = positionSize.w;

	// Make the particle a billboard (= allways show the front of the particule to the camera)
	// We do this by adding the starting position, and then changing the vertices depending on x or y
	vec3 finalVertexPosition = centerPosition
	 + (cameraRightWorld * vertices.x * size)
	 + (cameraUpWorld * vertices.y * size);

	gl_Position = viewProjectionMatrix * vec4(finalVertexPosition, 1.0f);
	UV = vertices.xy + vec2(0.5, 0.5);
	finalColor = color;
}