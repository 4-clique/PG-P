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
	Point3D bariCenter;
	double altX = 0, altY = 0, altZ = 0;
	double angleX = 0, angleY = 0, angleZ = 0;
	double scale = 1;
	double ka;
	double r, g, b;

	Object3D() {
		points = vector<Point3D>();
		faces = vector<Face3D>();
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

	void selectColor(double c1, double c2, double c3) {
		r = c1;
		g = c2;
		b = c3;
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