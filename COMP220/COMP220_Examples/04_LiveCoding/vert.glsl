#version 330 core

layout(location = 0) in vec3 vertexPosition;

//uniform float time=0.0f;
uniform mat4 modelMatrix;

void main()
{
	//mat4 MVPmatrix = projectionMatrix*viewMatrix*modelMatrix;

	gl_Position=modelMatrix*vec4(vertexPosition,1.0);
}