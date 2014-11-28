#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <Point3D.h>
#include <math.h>;
class Camera
{
public:
	Point3D center;
	Point3D directionX;
	Point3D directionY;
	Point3D directionZ;
	Object3D cameraCubo;
	vector<Point3D> cubo;
	Camera(){};
	Camera(Point3D c, Point3D xAxes, Point3D yAxes, Point3D zAxes){
		center = c;
		directionX = xAxes;
		directionY = yAxes;
		directionZ = zAxes;
		CalculateCube(c);
	};

	void CalculateCube(Point3D c) {
		Point3D c1 = Point3D(c.x + 0.5, c.y + 0.5, c.z + 1);
		Point3D c2 = Point3D(c.x + 0.5, c.y + 0.5, c.z - 1);
		Point3D c3 = Point3D(c.x + 0.5, c.y - 0.5, c.z + 1);
		Point3D c4 = Point3D(c.x + 0.5, c.y - 0.5, c.z - 1);
		Point3D c5 = Point3D(c.x - 0.5, c.y + 0.5, c.z + 1);
		Point3D c6 = Point3D(c.x - 0.5, c.y + 0.5, c.z - 1);
		Point3D c7 = Point3D(c.x - 0.5, c.y - 0.5, c.z + 1);
		Point3D c8 = Point3D(c.x - 0.5, c.y - 0.5, c.z - 1);
		cubo.push_back(c1);
		cubo.push_back(c2);
		cubo.push_back(c3);
		cubo.push_back(c4);
		cubo.push_back(c5);
		cubo.push_back(c6);
		cubo.push_back(c7);
		cubo.push_back(c8);
	}

	void emptyCube() {
		while (cubo.size() != 0) {
			cubo.pop_back();
		}
	}

	void loadCamera(){
		Point3D xCamera = directionX-center;
		xCamera = xCamera.normalized();
		Point3D yCamera = directionY-center;
		yCamera = yCamera.normalized();
		Point3D zCamera = center-directionZ;
		zCamera = zCamera.normalized();
		glMatrixMode(GL_MODELVIEW);
		//glGetDoublev(GL_MODELVIEW, modelview);
		glLoadIdentity();
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
		for (int i = 0; i < cubo.size(); i++) {
			cubo[i].x += transX;
			cubo[i].y += transY;
			cubo[i].z += transZ;
		}
	};

	void RotateMatrix(double degrees, char axis){
		Point3D c = center;
		translate(-c.x, -c.y, -c.z);
		if (axis == 'y'){
			double dxX = directionX.x;
			double dxZ = directionX.z;
			directionX.x = cos((acos(-1.0)*degrees)/180.0)*dxX - sin((acos(-1.0)*degrees)/180.0)* dxZ;
			directionX.z = cos((acos(-1.0)*degrees)/180.0)*dxZ + sin((acos(-1.0)*degrees)/180.0)* dxX;
			directionZ.x = - directionX.y*directionY.z + directionX.z*directionY.y;
			directionZ.y = - directionX.z*directionY.x + directionX.x*directionY.z;
			directionZ.z = - directionX.x*directionY.y + directionX.y*directionY.x;

			for (int i = 0; i < cubo.size(); i++) {
				double dx = cubo[i].x;
				double dz = cubo[i].z;
				cubo[i].x = cos((acos(-1.0)*degrees) / 180.0)*dx - sin((acos(-1.0)*degrees) / 180.0)* dz;
				cubo[i].z = cos((acos(-1.0)*degrees) / 180.0)*dz + sin((acos(-1.0)*degrees) / 180.0)* dx;
			}
		}
		else if (axis == 'x'){
			double dyY = directionY.y;
			double dyZ = directionY.z;
			directionY.y = cos((acos(-1.0)*degrees)/180.0)*dyY - sin((acos(-1.0)*degrees)/180.0) *dyZ;
			directionY.z = sin((acos(-1.0)*degrees)/180.0)*dyY + cos((acos(-1.0)*degrees)/180.0)*dyZ;
			directionZ.x = -directionX.y*directionY.z + directionX.z*directionY.y;
			directionZ.y = -directionX.z*directionY.x + directionX.x*directionY.z;
			directionZ.z = -directionX.x*directionY.y + directionX.y*directionY.x;
			for (int i = 0; i < cubo.size(); i++) {
				double dy = cubo[i].y;
				double dz = cubo[i].z;
				cubo[i].y =
					cos((acos(-1.0)*degrees) / 180.0)*dy - sin((acos(-1.0)*degrees) / 180.0) *dz;
				cubo[i].z = sin((acos(-1.0)*degrees) / 180.0)*dy + cos((acos(-1.0)*degrees) / 180.0)*dz;
			}
		}
		directionX = directionX.normalized();
		directionY = directionY.normalized();
		directionZ = directionZ.normalized();
		translate(c.x, c.y, c.z);

	};

private:

};

#endif