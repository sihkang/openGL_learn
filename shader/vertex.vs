#version 410 core

in vec4 aPos;
in vec4 aColor;
uniform mat4 uMat;

out vec4 vColor;

void main()
{
    vColor = aColor;
    gl_Position = uMat * aPos;
    gl_Position.z *= -1;
}