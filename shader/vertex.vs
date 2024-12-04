#version 410 core

in vec4 vertexPos;
in vec4 aColor;
uniform float uTheta;
uniform vec4 offset;

out vec4 Pos;
out vec4 vColor;

void main()
{
    vColor = aColor;
    gl_Position.x = vertexPos.x * cos(uTheta) - vertexPos.y * sin(uTheta) + offset.x * cos(uTheta) - offset.y * sin(uTheta);
    gl_Position.y = vertexPos.x * sin(uTheta) + vertexPos.y * cos(uTheta) + offset.x * sin(uTheta) + offset.y * cos(uTheta);
    gl_Position.z *= -1.0f;
    gl_Position.zw = vertexPos.yw;
    // gl_Position += offset;
    Pos = gl_Position;
}