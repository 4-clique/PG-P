#ifndef __POINT_3D_H__
#define __POINT_3D_H__
#include <math.h>
#define PI 3.14159265
class Point3D
{
public:
	double x;
	double y;
	double z;
	Point3D(){};
	Point3D(double a, double b, double c){
		x = a;
		y = b;
		z = c;
	};

	Point3D operator + (double a){
		Point3D retorno = *this;
		retorno.x += a;
		retorno.y += a;
		retorno.z += a;
		return retorno;
	};
	Point3D operator - (double a){
		Point3D retorno = *this;
		retorno.x -= a;
		retorno.y -= a;
		retorno.z -= a;
		return retorno;
	};
	Point3D operator + (Point3D a){
		Point3D retorno = *this;
		retorno.x += a.x;
		retorno.y += a.y;
		retorno.z += a.z;
		return retorno;
	};
	Point3D operator - (Point3D a){
		Point3D retorno = *this;
		retorno.x -= a.x;
		retorno.y -= a.y;
		retorno.z -= a.z;
		return retorno;
	};
	Point3D operator = (Point3D a){
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	};
	Point3D operator / (double c){
		Point3D retorno = *this;
		retorno.x /= c;
		retorno.y /= c;
		retorno.z /= c;
		return retorno;
	};
	Point3D operator * (double c){
		Point3D retorno = *this;
		retorno.x *= c;
		retorno.y *= c;
		retorno.z *= c;
		return retorno;
	};
	double module(){
		return sqrt(x*x + y*y + z*z);
	};

	double dot(Point3D a){
		return (x*a.x + y*a.y + z*a.z);
	};

	Point3D normalized(){
		return ((*this) / module());
	};

private:

};

#endif
