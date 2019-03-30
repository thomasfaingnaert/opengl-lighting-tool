#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

smooth out vec3 outColor;

uniform mat4 mvMatrix;
uniform mat4 pMatrix;

void main()
{
    gl_Position = pMatrix * mvMatrix * vec4(position, 1.0f);
    outColor = color;
}
