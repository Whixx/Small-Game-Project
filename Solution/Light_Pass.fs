#version 440

uniform sampler2D gPosition;
uniform sampler2D gDiffuse;
uniform sampler2D gNormal;
uniform vec2 screenSize;

out vec4 finalColor;

void main()
{
    vec2 texCoord = gl_FragCoord.xy / screenSize.xy;

    //vec3 pixelPos = texture(gPosition, texCoord).xyz;
    //vec3 normal = texture(gDiffuse, texCoord).xyz;
    //vec3 textureRGB = texture(gNormal, texCoord).xyz;

    //vec4 finalColor = vec4(textureRGB.xyz, 1.0f);

    vec3 color = texture(gDiffuse, texCoord).rgb;
    finalColor = vec4(color, 1.0f);

    //finalColor = vec4(0.0f,1.0f,0.0f,1.0f);
}