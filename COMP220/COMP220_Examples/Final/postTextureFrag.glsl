#version 330 core

in vec2 textureCoordsOut;

out vec4 colour;

uniform float time=0.0f;

uniform sampler2D texture0;

uniform vec4 blackColour = vec4(1.,1.,1.,1.);

uniform float effects = 0;

// https://hub.jmonkeyengine.org/t/august-2016-monthly-wip-screenshot-thread/36536/97
vec4 getDisplacedColor(in sampler2D img,in vec2 uv,in vec3 offsets){
    vec4 c=vec4(0.,0.,0.,1.);
    c.r+=texture(img,uv+offsets.r).r;
    c.g+=texture(img,uv+offsets.g).g;
    c.b+=texture(img,uv+offsets.b).b;
    return c;
}

float getLuminance(vec3 colour)
{
return (colour.r+colour.g+colour.b)/3.0f;
}

int Pixelise(vec3 colour)
{
if (colour.r < colour.g)
{
return 0;
}
else
{
return 1;
}
}


void main()
{	
	if (effects == 1.)
	{
	colour = getDisplacedColor(texture0, textureCoordsOut, vec3(sin(time),tan(-time),cos(time)));
	}
	else
	{
	colour=texture(texture0,textureCoordsOut);
	}

	if (effects == 2.)
	{
		vec4 textureColour=texture(texture0,(textureCoordsOut - 100000.));
		if (Pixelise(textureColour.rgb) == 0)
		{
		colour = blackColour - colour;
		}
	}
}
