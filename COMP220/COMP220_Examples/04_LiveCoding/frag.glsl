#version 330 core

uniform vec4 fragColour=vec4(1.0,1.0,1.0,1.0);

uniform float time=0.0f;

out vec4 colour;

void main()
{
colour.y+=sin(time);
colour.x+=cos(time);
//colour = fragColour;
}
