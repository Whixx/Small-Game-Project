#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform int currNrOfCoins;
uniform int MAX_NR_OF_INVENTORY_COINS;

out vec2 texCoord0;

void main()
{
	gl_Position = vec4(position, 1.0f);

	// Remove one "block" for each coin tossed
	gl_Position.x *= 1 * (currNrOfCoins * texCoord.x)/MAX_NR_OF_INVENTORY_COINS;

	// Translate the quad to the leftside
	gl_Position.x -= 1;

	// Translate the quad to the bottom
	gl_Position.y -= 0.55;

	// Calculate what parts of the texture to use
	texCoord0 = texCoord;
	texCoord0.x = (currNrOfCoins * texCoord.x)/MAX_NR_OF_INVENTORY_COINS;
}