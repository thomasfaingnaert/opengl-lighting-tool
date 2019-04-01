#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

smooth out vec3 outColor;

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
    vec3 ambientProduct = lightAmbient * materialAmbient;
    vec3 diffuseProduct = lightDiffuse * materialDiffuse;
    vec3 specularProduct = lightSpecular * materialSpecular;

    // Calculate outputs
    vec3 viewSpacePosition = vec3(mvMatrix * vec4(position, 1.0));  // position of vertex in view space

    vec3 L = normalize(lightPosition - viewSpacePosition);          // vector from vertex to light source
    vec3 E = normalize(-viewSpacePosition);                         // vector from vertex to camera (at the origin)
    vec3 H = normalize(L + E);                                      // "half-way" vector
    vec3 N = vec3(normalize(mvMatrix * vec4(normal, 0.0)));         // normal vector in view space

    vec3 ambient = ambientProduct;

    float Kd = max(dot(L, N), 0.0);
    vec3 diffuse = Kd * diffuseProduct;

    float Ks = pow(max(dot(N, H), 0.0), materialShininess);
    vec3 specular = Ks * specularProduct;

    if (dot(L, N) < 0.0)
        specular = vec3(0.0, 0.0, 0.0);

    gl_Position = pMatrix * mvMatrix * vec4(position, 1.0);
    outColor = ambient + diffuse + specular;
}
