#version 410 core

in vec4 vertexPos;
in vec4 aColor;
uniform float uTheta;

out vec4 Pos;
out vec4 vColor;


void main()
{
    vColor = aColor;
    gl_Position.z = vertexPos.z * cos(uTheta) - vertexPos.x * sin(uTheta);
    gl_Position.x = vertexPos.z * sin(uTheta) + vertexPos.x * cos(uTheta);
    gl_Position.z *= -1.0f;
    gl_Position.yw = vertexPos.yw;
    Pos = gl_Position;
}