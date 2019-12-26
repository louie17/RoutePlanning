#include "Mission_G.h"

Mission_G::Mission_G()
{
}

Mission_G::Mission_G(int t, float x, float y, float z, float step, float para)
{
	this->type = t;
	this->x = x;
	this->y = y;
	this->z = z;
	this->step = step;
	this->para = para;

}


Mission_G::~Mission_G()
{
}
