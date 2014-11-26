#ifndef __OBJECT_3D_H__
#define __OBJECT_3D_H__
#include <Face3D.h>
#include <Point3D.h>
#include <cmath>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <fstream>
#include <deque>
#include <queue>
#include <functional>
#include <map>
#include <bitset>
#include <stack>
#include <set>
using namespace std;
#define PI 3.14159265
class Object3D
{
public:
	vector<Point3D> points;
	vector<Face3D> faces;
	vector<Point3D> normais;
	vector<Point3D> reflexaoLuz; // guarda as reflexoes da luz
	vector<Point3D> v; //guarda os v - que apontam para o observador
	vector<Point3D> texturesPoints;
	Point3D bariCenter;
	double scale = 1;
	double ka, kd, ks; //ka é coeficiente de reflexao ambiente, kb é coeficiente de reflexao difusa, ks é de especular
	double q; //para calculo da especular
	double r, g, b;

	Object3D() {
		points = vector<Point3D>();
		faces = vector<Face3D>();
		normais = vector<Point3D>();
	}

	Object3D(vector<Point3D> p, vector<Face3D> f, Point3D b) {
		points = p;
		faces = f;
		bariCenter = b;
	}

	Object3D(vector<Point3D> p, vector<Face3D> f, vector<Point3D> t, Point3D b) {
		points = p;
		faces = f;
		texturesPoints = t;
		bariCenter = b;
	}

	void insertPoint(double x, double y, double z) {
		points.push_back(Point3D(x, y, z));
	}

	void insertFace(int a, int b, int c) {
		faces.push_back(Face3D(a, b, c));
	}

	void translate(double transX, double transY, double transZ){
		for (int i = 0; i < points.size(); i++){
			points[i].x += transX;
			points[i].y += transY;
			points[i].z += transZ;
		}
		bariCenter.x += transX;
		bariCenter.y += transY;
		bariCenter.z += transZ;
	}

	void RotateMatrix(double degrees, char axis){
		take2Center();
		if (axis == 'x'){
			for (int i = 0; i < points.size(); i++){
				points[i].y = cos((PI*degrees) / 180)*points[i].y - sin((PI*degrees) / 180) * points[i].z;
				points[i].z = sin((PI*degrees) / 180)*points[i].y + cos((PI*degrees) / 180)*points[i].z;
			}
		}
		else if (axis == 'y') {
			for (int i = 0; i < points.size(); i++){
				points[i].x = cos((PI*degrees) / 180)*points[i].x + sin((PI*degrees) / 180) * points[i].z;
				points[i].z = cos((PI*degrees) / 180)*points[i].z - sin((PI*degrees) / 180)*points[i].x;
			}
		}
		else if (axis == 'z') {
			for (int i = 0; i < points.size(); i++){
				points[i].x = cos((PI*degrees) / 180)*points[i].x - sin((PI*degrees) / 180) * points[i].y;
				points[i].y = cos((PI*degrees) / 180)*points[i].y + sin((PI*degrees) / 180)*points[i].x;
			}
		}
		takeBack();
	}

	void selectka(double a) {
		if (a<=1.0 && a>=0.0){
			ka = a;
		}
	}

	void selectkd(double a) {
		if (a <= 1.0 && a >= 0.0){
			kd = a;
		}
	}

	void selectks(double a) {
		if (a <= 1.0 && a >= 0.0){
			ks = a;
		}
	}
	
	void selectq(double a) {
		q = a;
	}

	void selectColor(double c1, double c2, double c3) {
		r = c1;
		g = c2;
		b = c3;
	}

	void recalculate(Point3D fonte, Point3D cameraCenter){
		normais.clear();
		reflexaoLuz.clear();
		v.clear();
		for (int i = 0; i < faces.size(); i++) {		
			//Normais:
			Point3D v1 = (points[faces[i].p2] - points[faces[i].p3]).normalized();
			Point3D v2 = (points[faces[i].p1] - points[faces[i].p3]).normalized();
			Point3D normal = v1.cross(v2);
			normais.push_back(normal.normalized());

			//reflexaoLuz:
			Point3D N = normais[i];
			Point3D L = (fonte - points[faces[i].p1]).normalized();
			Point3D R = N * 2 * (N.dot(L)) - L;
			reflexaoLuz.push_back(R.normalized());

			//V:
			Point3D v_1 = (cameraCenter - points[faces[i].p1]).normalized();
			v.push_back(v_1.normalized());
		}

	}

private:
	void take2Center(){
		for (int i = 0; i < points.size(); i++){
			points[i] = points[i] - bariCenter;
		}
	}

	void takeBack(){
		for (int i = 0; i < points.size(); i++){
			points[i] = points[i] + bariCenter;
		}
	}
};

#endif