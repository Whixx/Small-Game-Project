#version 440

out vec4 fragment_color;

in vec2 texCoord0;

uniform sampler2D scene;
uniform sampler2D bright;

void main()
{
	vec3 sceneColor = texture2D(scene, texCoord0).xyz;
	vec3 bloomColor = texture2D(bright, texCoord0).xyz;

	fragment_color = vec4(sceneColor.xyz + bloomColor.xyz, 1.0f);
}