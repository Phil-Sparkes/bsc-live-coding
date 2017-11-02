#version 330 core

uniform vec4 fragColour=vec4(1.0,1.0,1.0,1.0);

uniform float time=0.0f;

out vec4 colour;

void main()
{
    //add rand below
	colour=vec4(0.0f,0.0f,0.0f,1.0f);
	colour.r+=sin(time);
	colour.g+=cos(time);
}
