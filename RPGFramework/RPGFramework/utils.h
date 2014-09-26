#ifndef UTILS_H
#define UTILS_H

#include<scene.h>
#include<postprocess.h>
#include<freeglut.h>

class Utils
{
public:
	void Color4f(const aiColor4D * color);
	void set_float4(float f[4], float a, float b, float c, float d);
	void color4_to_float4(const aiColor4D * c, float f[4]);
};
#endif