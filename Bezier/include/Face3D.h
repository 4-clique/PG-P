#ifndef __FACE_3D_H__
#define __FACE_3D_H__

class Face3D
{
public:
	int p1;
	int p2;
	int p3;
	int c1 = 0, c2 = 0, c3 = 0;
	Face3D(int a, int b, int c){
		p1 = a;
		p2 = b;
		p3 = c;
	};
	void setColor(float a, float b, float c){
		c1 = a;
		c2 = b;
		c3 = c;
	};
private:

};

#endif