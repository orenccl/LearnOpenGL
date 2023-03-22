#version 330 core

out vec4 FragColor;

uniform float greenValue;

void main()
{
   FragColor = vec4(0.0f, greenValue, 0.0f, 1.0f);
}