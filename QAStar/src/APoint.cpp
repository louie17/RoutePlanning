#include "APoint.h"



APoint::APoint()
{


}

 APoint::APoint(float x, float y, float z, float g, float f, int t, float time, float s)
{

	X = x;
	Y = y;
	Z = z;
	G = g;
	F = f;
	TIME_COST = time;
	S_COST = s;
	TYPE = t;


}

 APoint::~APoint()
{
}
