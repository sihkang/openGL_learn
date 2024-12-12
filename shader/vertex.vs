#version 410 core

struct Light
{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 att;
};

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shineness;
};

in vec4 aPos;
in vec4 aNorm;

uniform mat4 uMat;

uniform Light light;
uniform Material material;

out vec4 vPos;
out vec4 vNorm;

void main()
{
    vPos = uMat * aPos;
    vPos.z *= -1;
    vNorm = transpose(inverse(uMat)) * aNorm;
    gl_Position = vPos;
    // gl_Position.z *= -1;
}