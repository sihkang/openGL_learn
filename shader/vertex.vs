#version 410 core

in vec4 vertexPos;
in vec4 aColor;
// uniform vec3 uAngle;
uniform mat4 transform;

out vec4 Pos;
out vec4 vColor;


void main()
{
    vColor = aColor;
    gl_Position = transform * vertexPos;
    Pos = gl_Position;
}