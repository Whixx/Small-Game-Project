#version 440

in vec4 finalColor;
in vec2 UV;

out vec4 fragment_Color;

uniform sampler2D particleTexture;

void main()
{
	vec4 texColor = texture(particleTexture, UV);
	if (texColor.a < 0.1) 
	{
        discard;
	}

	fragment_Color = texColor * finalColor;
}