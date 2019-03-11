#version 440

out vec4 fragment_color;

in vec2 texCoord0;

uniform sampler2D texture;
uniform int index;
uniform int MAX_NR_OF_INVENTORY_COINS;

void main()
{
	// Calculate what parts of the texture to use
	vec2 textureCoords = texCoord0;

	textureCoords.x = (index * texCoord0.x)/MAX_NR_OF_INVENTORY_COINS;

	// Sample from the texture
	vec3 materialColor = texture2D(texture, textureCoords).rgb;
	
	fragment_color = vec4(materialColor.rgb, 1.0f);
}