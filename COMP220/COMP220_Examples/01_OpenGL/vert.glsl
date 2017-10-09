#version 330 core

layout(location = 0) in vec3 vertexPosition;

uniform float time=0.0f;

void main()
{
	vec3 newVertexPosition=vertexPosition;

	newVertexPosition.xy*=sin(time);
	newVertexPosition.x+=cos(time);
	//newVertexPosition.x*=tan(time);

	gl_Position=vec4(newVertexPosition,1.0);
}