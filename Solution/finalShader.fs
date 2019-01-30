#version 440

out vec4 fragment_color;

in vec2 texCoord0;

uniform sampler2D scene;

void main()
{
	vec3 materialColor = texture2D(scene,texCoord0).xyz;
    fragment_color = vec4(materialColor, 1.0);
}