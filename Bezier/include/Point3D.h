#ifndef __POINT_3D_H__
#define __POINT_3D_H__

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
		x += a;
		y += a;
		z += a;
		return *this;
	};
	Point3D operator - (double a){
		x -= a;
		y -= a;
		z -= a;
		return *this;
	};
	Point3D operator + (Point3D a){
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	};
	Point3D operator - (Point3D a){
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	};
	Point3D operator = (Point3D a){
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	};
	Point3D operator / (unsigned int c){
		x /= c;
		y /= c;
		z /= c;
		return *this;
	};
private:

};

#endif
