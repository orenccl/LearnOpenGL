#version 330 core

layout (location = 0) in vec3 aPos;
// multi colors for gradient 
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 bColor;
layout (location = 3) in vec3 cColor;

out vec3 ourAColor;
out vec3 ourBColor;
out vec3 ourCColor;

uniform float xOffset;

void main()
{
    gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0);
    ourAColor = aColor;
    ourBColor = bColor;
    ourCColor = cColor;
}