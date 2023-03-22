#version 330 core

out vec4 FragColor;

in vec3 ourAColor;
in vec3 ourBColor;
in vec3 ourCColor;

// Change by time¡Ainput range 0~3
uniform float gradientValue;

// color lerp algorithm
// --------------------
vec4 lerp(vec3 startColor, vec3 endColor, float normalValue)
{
    return vec4(
        startColor.x + (endColor.x - startColor.x) * normalValue,
        startColor.y + (endColor.y - startColor.y) * normalValue,
        startColor.z+ (endColor.z - startColor.z) * normalValue,
        1.0
    );
}

void main()
{
    // dicide linearGradient color
    if(gradientValue < 1)
    {
        // 0~1
        FragColor = lerp(ourAColor, ourBColor, gradientValue);
    } 
    else if(gradientValue < 2)
    {
        // 1~2
        FragColor = lerp(ourBColor, ourCColor, gradientValue - 1);
    }
    else if(gradientValue < 3)
    {
        // 2~3
        FragColor = lerp(ourCColor, ourAColor, gradientValue - 2);
    }
}
