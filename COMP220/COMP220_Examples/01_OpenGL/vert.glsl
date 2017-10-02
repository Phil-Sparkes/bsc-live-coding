#version 330 core

layout(location = 0) in vec3 vertexPosition;

uniform vec3 myVariable;

void main()
{
// gl_Position=vec4(vertexPosition, 1.0)
gl_Position.xyz = myVariable;
gl_Position.w = 1.0;
}