#version 440

out vec4 fragment_color;

in vec2 texCoord0;

uniform bool horizontal;
uniform sampler2D scene;

void main()
{
	float fade[5] = {0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162};

    vec2 offset = 1.0 / textureSize(scene, 0); // gets size of single texel
    vec3 result = texture(scene, texCoord0).rgb * fade[0];

	//vec3 materialColor = texture2D(scene, texCoord0).rgb;
    if(horizontal)
    {
			//fragment_color = vec4(1.0f,0.0f,0.0f,1.0f);
        for(int i = 1; i < 5; ++i)
        {
           result += texture2D(scene, texCoord0 + vec2(offset.x * i, 0.0)).rgb * fade[i];
           result += texture2D(scene, texCoord0 - vec2(offset.x * i, 0.0)).rgb * fade[i];
        }
    }
    else
    {
			//fragment_color = vec4(0.0f,1.0f,0.0f,1.0f);
        for(int i = 1; i < 5; ++i)
        {
            result += texture2D(scene, texCoord0 + vec2(0.0, offset.y * i)).rgb * fade[i];
            result += texture2D(scene, texCoord0 - vec2(0.0, offset.y * i)).rgb * fade[i];
        }
    }
    fragment_color = vec4(result, 1.0);
}