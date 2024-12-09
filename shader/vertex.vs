#version 410 core

in vec4 aPos;
in vec4 aColor;
uniform mat4 uModel;
uniform mat4 uView;

out vec4 vColor;

void main()
{
    vColor = aColor;
    gl_Position = uView * uModel * aPos;
    gl_Position.z *= -1;
}