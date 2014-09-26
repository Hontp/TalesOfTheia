#include "utils.h"

void Utils::Color4f(const aiColor4D * color)
{
	glColor4f(color->r, color->g, color->b, color->a);
}

void Utils::set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void Utils::color4_to_float4(const aiColor4D * c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}