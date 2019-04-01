#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

smooth out vec3 outPosition;
smooth out vec3 outNormal;

uniform mat4 mvMatrix;
uniform mat4 pMatrix;

uniform vec3 lightPosition;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

uniform float materialShininess;
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;

void main()
{
    gl_Position = pMatrix * mvMatrix * vec4(position, 1.0);
    outPosition = position;
    outNormal = normal;
}
