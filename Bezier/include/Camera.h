#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <Point3D.h>
#include <math.h>;
#include <Transform.h>

class Camera
{
public:
	Point3D center;
	Point3D directionX;
	Point3D directionY;
	Point3D directionZ;
	Camera(){};
	Camera(Point3D c, Point3D xAxes, Point3D yAxes, Point3D zAxes){
		center = c;
		directionX = xAxes;
		directionY = yAxes;
		directionZ = zAxes;
	};

	void loadCamera(){
		Point3D xCamera = directionX-center;
		xCamera = xCamera.normalized();
		Point3D yCamera = directionY-center;
		yCamera = yCamera.normalized();
		Point3D zCamera = center-directionZ;
		zCamera = zCamera.normalized();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);               // Where The 16 Doubles Of The Projection Matrix Are To Be Stored
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		GLfloat extrinsic1[16] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-center.x, -center.y, -center.z, 1
		};
		GLfloat extrinsic2[16] = {
			xCamera.x, yCamera.x, zCamera.x, 0,
			xCamera.y, yCamera.y, zCamera.y, 0,
			xCamera.z, yCamera.z, zCamera.z, 0,
			0, 0, 0, 1
		}; 
		
		glMultMatrixf(extrinsic2);
		glMultMatrixf(extrinsic1);
		
		/*glPushMatrix();
		glColor3f(1, 0, 0);
		glPointSize(10);
		glBegin(GL_POINTS);
		glVertex3f(directionX.x, directionX.y, directionX.z);
		glVertex3f(directionY.x, directionY.y, directionY.z);
		glVertex3f(directionZ.x, directionZ.y, directionZ.z);
		glEnd();
		glPopMatrix();*/
		
	};

	void translate(double transX, double transY, double transZ){
		center.x += transX;
		directionX.x += transX;
		directionY.x += transX;
		directionZ.x += transX;
		center.y += transY;
		directionX.y += transY;
		directionY.y += transY;
		directionZ.y += transY;
		center.z += transZ;
		directionX.z += transZ;
		directionY.z += transZ;
		directionZ.z += transZ;
	};

	void RotateMatrix(double degrees, char axis){
		Point3D c = center;
		translate(-c.x, -c.y, -c.z);
		if (axis == 'y'){
			double dxX = directionX.x;
			double dxZ = directionX.z;
			directionX.x = cos((acos(-1.0)*degrees)/180.0)*dxX + sin((acos(-1.0)*degrees)/180.0)* dxZ;
			directionX.z = cos((acos(-1.0)*degrees)/180.0)*dxZ - sin((acos(-1.0)*degrees)/180.0)* dxX;

			directionZ.x = - directionX.y*directionY.z + directionX.z*directionY.y;
			directionZ.y = - directionX.z*directionY.x + directionX.x*directionY.z;
			directionZ.z = - directionX.x*directionY.y + directionX.y*directionY.x;
		}
		if (axis == 'x'){
			double dyY = directionY.y;
			double dyZ = directionY.z;
			directionY.y = cos((acos(-1.0)*degrees)/180.0)*dyY - sin((acos(-1.0)*degrees)/180.0) *dyZ;
			directionY.z = sin((acos(-1.0)*degrees)/180.0)*dyY + cos((acos(-1.0)*degrees)/180.0)*dyZ;

			directionZ.x = -directionX.y*directionY.z + directionX.z*directionY.y;
			directionZ.y = -directionX.z*directionY.x + directionX.x*directionY.z;
			directionZ.z = -directionX.x*directionY.y + directionX.y*directionY.x;
		}
		directionX = directionX.normalized();
		directionY = directionY.normalized();
		directionZ = directionZ.normalized();
		/*cout << directionX.x << " " << directionX.y << " " << directionX.z << endl;
		cout << directionZ.x << " " << directionZ.y << " " << directionZ.z << endl;*/
		translate(c.x, c.y, c.z);
		//translate(-center.x, -center.y, -center.z);
		//translate(c.x, c.y, c.z);
		/*Point3D c = Point3D(center.x, center.y, center.z);
		center.x -= c.x;
		direction.x -= c.x;
		center.y -= c.y;
		direction.y -= c.y;
		center.z -= c.z;
		direction.z -= c.z;
		if (axis == 'x'){
			double y = direction.y;
			double z = direction.z;
			direction.y = cos((acos(-1.0)*degrees) / 180.0)*y - sin((acos(-1.0)*degrees) / 180.0) * z;
			direction.z = sin((acos(-1.0)*degrees) / 180.0)*y + cos((acos(-1.0)*degrees) / 180.0)*z;
		}
		else if (axis == 'y') {
			double x = direction.x;
			double z = direction.z;
			direction.x = cos((acos(-1.0)*degrees) / 180.0)*x + sin((acos(-1.0)*degrees) / 180.0) * z;
			direction.z = cos((acos(-1.0)*degrees) / 180.0)*z - sin((acos(-1.0)*degrees) / 180.0)*x;
		}
		center.x += c.x;
		direction.x += c.x;
		center.y += c.y;
		direction.y += c.y;
		center.z += c.z;
		direction.z += c.z;*/
	};

private:

};

#endif