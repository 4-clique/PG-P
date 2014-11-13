#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__
#include <Object3D.h>
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
#include <LightSource.h>
#include <float.h>
#include <limits.h>
const double TRANS_SCALA = 1;
const double ROTAT_SCALA = 4;
const double MOVE_CAMERA = 0.75;
const double ROTAT_CAMERA = 10;
const double CAMERAX_INICIAL = 0;
const double CAMERAY_INICIAL = 0;
const double CAMERAZ_INICIAL = 10;

Object3D readObject(string arquivo){
	ifstream ifs(arquivo, ifstream::in);
	string type; double a, b, c;
	vector<Point3D> point_vector = vector<Point3D>();
	vector<Point3D> texturePoints_vector = vector<Point3D>();
	vector<Face3D> face_vector = vector<Face3D>();
	Point3D bariCenter = Point3D(0, 0, 0);
	if (ifs.good()) cout << "Lendo Arquivo " << arquivo << endl;
	else cout << "Arquivo " << arquivo << " não pôde ser lido" << endl;
	while ((ifs >> type)){
		for (int i = 0; i < type.size(); i++) type[i] = tolower(type[i]);

		if (type.compare("#") == 0) ifs.ignore(numeric_limits<streamsize>::max(), '\n');
		else if (type.compare("v") == 0){
			ifs >> a >> b >> c;
			bariCenter.x += a;
			bariCenter.y += b;
			bariCenter.z += c;
			point_vector.push_back(Point3D(a, b, c));
		}
		else if (type.compare("vt") == 0){
			ifs >> a >> b;
			texturePoints_vector.push_back(Point3D(a, b, 0));
		}
		else if (type.compare("f") == 0){
			ifs >> a >> b >> c;
			face_vector.push_back(Face3D(a - 1, b - 1, c - 1));
		}
	}
	if (texturePoints_vector.size() == 0)
		return Object3D(point_vector, face_vector, bariCenter/point_vector.size());
	else
		return Object3D(point_vector, face_vector, texturePoints_vector, bariCenter / point_vector.size());
};



//double X2NDC(double x, double width){ return (((x * 2) / width) - 1); }
//double Y2NDC(double y, double height){ return ((y * (-2)) / height) + 1; }

#endif

