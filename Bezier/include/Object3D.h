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
	vector<Point3D> f1; // para fonte de luz 1
	vector<double> prod; //produto interno
	Point3D bariCenter;
	double altX = 0, altY = 0, altZ = 0;
	double angleX = 0, angleY = 0, angleZ = 0;
	double scale = 1;
	double ka, kd; //ka é coeficiente de reflexao ambiente, kb é coeficiente de reflexao difusa
	double r, g, b;

	Object3D() {
		points = vector<Point3D>();
		faces = vector<Face3D>();
		normais = vector<Point3D>();
		f1 = vector<Point3D>();
		prod = vector<double>();
	}

	Object3D(vector<Point3D> p, vector<Face3D> f, Point3D b) {
		points = p;
		faces = f;
		bariCenter = b;
	}

	void insertPoint(double x, double y, double z) {
		points.push_back(Point3D(x, y, z));
	}

	void insertFace(int a, int b, int c) {
		faces.push_back(Face3D(a, b, c));
	}

	void calculateProd(){
		for (int i = 0; i < faces.size(); i++){
			prod.push_back(f1[i].x*normais[i].x + f1[i].y*normais[i].y + f1[i].z*normais[i].z);
		}
	}

	void calculateF1(Point3D fonte){
		for (int i = 0; i < faces.size();i++) {
			Point3D bar;
			bar.x = (points[faces[i].p1].x + points[faces[i].p2].x + points[faces[i].p3].x) / 3;
			bar.y = (points[faces[i].p1].y + points[faces[i].p2].y + points[faces[i].p3].y) / 3;
			bar.z = (points[faces[i].p1].z + points[faces[i].p2].z + points[faces[i].p3].z) / 3;

			Point3D ver = bar - fonte;

			//normaliza:
			double div = sqrt(ver.x*ver.x + ver.y*ver.y + ver.z*ver.z);
			ver = ver/ div;

			f1.push_back(ver);
		}
	}

	void calculateNormal() { 
		for (int i = 0; i < faces.size(); i++) {
			Point3D v1, v2, res;
			Point3D a = points[faces[i].p1];
			Point3D b = points[faces[i].p2];
			Point3D c = points[faces[i].p3];
			v1.x = b.x - a.x;
			v1.y = b.y - a.y;
			v1.z = b.z - a.z;
			
			v2.x = c.x - a.x;
			v2.y = c.y - a.y;
			v2.z = c.z - a.z;

			//produto vetorial:
			res.x = (v1.y * v2.z) - (v1.z * v2.y);
			res.y = (v1.z * v2.x) - (v1.x*v2.z);
			res.z = (v1.x * v2.y) - (v1.y*v2.x);
			
			//normalizando:
			double div = sqrt(res.x*res.x + res.y*res.y + res.z*res.z);
			res = res / div;
			
			normais.push_back(res);
		}
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

	void selectColor(double c1, double c2, double c3) {
		r = c1;
		g = c2;
		b = c3;
	}

	void recalculate(Point3D fonte){
		calculateF1(fonte);
		calculateNormal();
		calculateProd();
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