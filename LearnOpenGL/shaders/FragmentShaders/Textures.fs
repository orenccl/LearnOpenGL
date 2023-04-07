#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float mixValue;

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    FragColor = mix(texture(texture0, TexCoord), texture(texture1, vec2(1.0 - TexCoord.x, TexCoord.y)), mixValue);
}
