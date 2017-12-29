#version 330 core

in vec2 textureCoordsOut;

out vec4 colour;

uniform sampler2D texture0;

uniform vec4 blackColour = vec4(1.,1.,1.,1.);

int RGBSplit(vec3 colour)
{
if (colour.r > colour.g)
{
return 0;
}
else
{
return 1;
}
}

float getLuminance(vec3 colour)
{
return (colour.r+colour.g+colour.b)/3.0f;
}

void main()
{
    colour=texture(texture0,textureCoordsOut);
	//colour = blackColour - colour;

	vec4 textureColour=texture(texture0,(textureCoordsOut -100000.));
	//float luminance = getLuminance(textureColour.rgb);
	
	int whatever = RGBSplit(textureColour.rgb);
	if (whatever == 0)
	{
	colour = vec4(0.,1.,0.,1.);
	}


}
