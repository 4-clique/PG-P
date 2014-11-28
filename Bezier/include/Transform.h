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
#include <sstream>
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
const double LIGHT_SCALA = 30;
const double TRANS_SCALA = 1;
const double ROTAT_SCALA = 4;
const double MOVE_CAMERA = 0.5;
const double ROTAT_CAMERA = 10;
const double CAMERAX_INICIAL = 0;
const double CAMERAY_INICIAL = 0;
const double CAMERAZ_INICIAL = 10;
const double DIRETORX = 0;
const double DIRETORY = 0;
const double DIRETORZ = 20;
bool PAINT_FACE = true;

Object3D readObject(string arquivo){
	
	ifstream ifs(arquivo, ifstream::in);
	string type; double a, b, c;
	vector<Point3D> point_vector = vector<Point3D>();
	vector<Point3D> texturePoints_vector = vector<Point3D>();
	vector<Face3D> face_vector = vector<Face3D>();
	vector<Face3D> normalFace_vector = vector<Face3D>();
	vector<Point3D> normalPoint_vector = vector<Point3D>();
	vector<Face3D> textureFace_vector = vector<Face3D>();
	Point3D bariCenter = Point3D(0, 0, 0);
	if (ifs.good()) cout << "Lendo Arquivo " << arquivo << endl;
	else cout << "Arquivo " << arquivo << " não pôde ser lido" << endl;
	while ((ifs >> type)){
		for (int i = 0; i < type.size(); i++) type[i] = tolower(type[i]);
		if (type.compare("v") == 0){
			ifs >> a >> b >> c;
			bariCenter.x += a;
			bariCenter.y += b;
			bariCenter.z += c;
			point_vector.push_back(Point3D(a, b, c));
		}
		else if (type.compare("vt") == 0){
			string texture;
			getline(ifs, texture);
			stringstream ss(texture);
			vector<double> t = vector<double>();
			while (ss >> a) t.push_back(a);
			if(t.size() == 2) texturePoints_vector.push_back(Point3D(t[0], t[1], 0));
			else if (t.size() == 3) texturePoints_vector.push_back(Point3D(t[0], t[1], t[2]));
		}
		else if (type.compare("vn") == 0){
			ifs >> a >> b >> c;
			normalPoint_vector.push_back(Point3D(a, b, c));
		}
		else if (type.compare("f") == 0){
			string faceLine;
			getline(ifs, faceLine);
			stringstream ss(faceLine);
			string face;
			vector<int> pFace = vector<int>();
			vector<int> pNormal = vector<int>();
			vector<int> pTexture = vector<int>();
			while (ss >> face){
				for (int i = 0; i < 3; i++){
					int p = -1;
					if (face.find_first_of('/') != string::npos){
						if (face.substr(0, face.find_first_of('/')).compare("") != 0)
							p = stoi(face.substr(0,face.find_first_of('/')));
					}
					else p = stoi(face);
					if (face.find_first_of('/') != string::npos) {
						face.erase(0, face.find_first_of('/') + 1);
					}
					if (i == 0 && p != -1)	pFace.push_back(p);
					if (i == 1 && p != -1) pTexture.push_back(p);
					if (i == 2 && p != -1) pNormal.push_back(p);
				}
			}
			if(pFace.size() == 3)face_vector.push_back(Face3D(pFace[0] - 1, pFace[1] - 1, pFace[2] - 1));
			else if (pFace.size() == 4) face_vector.push_back(Face3D(pFace[0] - 1, pFace[1] - 1, pFace[2] - 1, pFace[3] - 1));
			if (pNormal.size() == 3) normalFace_vector.push_back(Face3D(pNormal[0] - 1, pNormal[1] - 1, pNormal[2] - 1));
			else if (pNormal.size() == 4) normalFace_vector.push_back(Face3D(pNormal[0] - 1, pNormal[1] - 1, pNormal[2] - 1, pNormal[3] - 1));
			if (pTexture.size() == 3) textureFace_vector.push_back(Face3D(pTexture[0] - 1, pTexture[1] - 1, pTexture[2] - 1));
			else if (pTexture.size() == 4) textureFace_vector.push_back(Face3D(pTexture[0] - 1, pTexture[1] - 1, pTexture[2] - 1, pTexture[3] - 1));
		}
		else ifs.ignore(numeric_limits<streamsize>::max(), '\n');
		
	}
	if (texturePoints_vector.size() == 0)
		return Object3D(point_vector, face_vector, bariCenter/point_vector.size());
	else
		return Object3D(point_vector, face_vector, texturePoints_vector, bariCenter / point_vector.size());
};



//double X2NDC(double x, double width){ return (((x * 2) / width) - 1); }
//double Y2NDC(double y, double height){ return ((y * (-2)) / height) + 1; }

#endif

