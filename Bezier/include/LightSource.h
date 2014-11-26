#ifndef __LIGHT_SOURCE_H__
#define __LIGHT_SOURCE_H__
#include <Point3D.h>
class LightSource
{
public:
	Point3D location = Point3D();
	double r, g, b;
	LightSource(double p1, double p2, double p3){
		location.x = p1;
		location.y = p2;
		location.z = p3;
	};
	void setColor(double c1, double c2, double c3){
		r = c1;
		g = c2;
		b = c3;
	};
private:

};

#endif