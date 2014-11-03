#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <Point3D.h>

class Camera
{
public:
	Point3D center;
	Point3D direction;
	Point3D way;
	Camera(){};
	Camera(double a, double b, double c){
		center = Point3D(a, b, c);
		direction = Point3D(0, 0, 0);
		way = Point3D(0, 1, 0); //Direcao default
	};

	void translate(double transX, double transY, double transZ){
		Point3D c = center;
		if (transZ != 0) {			
			center.x += (direction.x - c.x) / transZ;
			direction.x += (direction.x - c.x) / transZ;
			center.y += (direction.y - c.y) / transZ;
			direction.y += (direction.y - c.y) / transZ;
			center.z += (direction.z - c.z) / transZ;
			direction.z += (direction.z - c.z) / transZ;
		}
		if (transX != 0){
			
			RotateMatrix(90, 'y');
			center.x += (direction.x - c.x) / transX;
			direction.x += (direction.x - c.x) / transX;
			center.y += (direction.y - c.y) / transX;
			direction.y += (direction.y - c.y) / transX;
			center.z += (direction.z - c.z) / transX;
			direction.z += (direction.z - c.z) / transX;
			RotateMatrix(-90, 'y');
		}
		
		
	};

	void RotateMatrix(double degrees, char axis){
		Point3D c = Point3D(center.x, center.y, center.z);
		center.x -= c.x;
		direction.x -= c.x;
		center.y -= c.y;
		direction.y -= c.y;
		center.z -= c.z;
		direction.z -= c.z;
		if (axis == 'x'){
			double y = direction.y;
			double z = direction.z;
			direction.y = cos((PI*degrees) / 180)*y - sin((PI*degrees) / 180) * z;
			direction.z = sin((PI*degrees) / 180)*y + cos((PI*degrees) / 180)*z;
		}
		else if (axis == 'y') {
			double x = direction.x;
			double z = direction.z;
			direction.x = cos((PI*degrees) / 180)*x + sin((PI*degrees) / 180) * z;
			direction.z = cos((PI*degrees) / 180)*z - sin((PI*degrees) / 180)*x;
		}
		center.x += c.x;
		direction.x += c.x;
		center.y += c.y;
		direction.y += c.y;
		center.z += c.z;
		direction.z += c.z;
	};

private:

};

#endif