#version 440

in vec2 outTexCoords;
out vec4 fragColor;

uniform sampler2D texture;
void main()
{
	vec3 materialColor = texture2D(texture, outTexCoords).rgb;
	fragColor = vec4(1.0f,1.0f,1.0f,1.0f);
}