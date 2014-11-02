/*
-----------------------------------------------------------------------------
OpenGL Tutorial
VOXAR Labs
Computer Science Center - CIn
Federal University of Pernambuco - UFPE
http://www.cin.ufpe.br/~voxarlabs

Referencias:
Funcoes de C/C++:
	http://www.cplusplus.com/
Copia online do Red Book (OpenGL Programming Guide):
	http://fly.cc.fer.hr/~unreal/theredbook/
Referencia online para os comandos OpenGL (Man pages):
	http://www.opengl.org/sdk/docs/man/

-----------------------------------------------------------------------------
*/

#include "openGL_tutorial.h"
#include <Transform.h>


using namespace std;
GLfloat mouse_x, mouse_y;
GLfloat window_width = 800.0;
GLfloat window_height = 800.0;
//double myScreen[800][800];
vector<Object3D> objects = vector<Object3D>();
int selected_object = 0;
double ia = 1;
double il = 1;
LightSource source1 = LightSource(0,0,0); // no centro do mundo

void myreshape (GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	window_width = (GLfloat) w;
	window_height = (GLfloat) h;
	gluPerspective(45, w/h, 0, 1);
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 1);
}

void mydisplay()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	double x = 0;
	double y = 0;
	double size = 0.5;

	

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int j = 0; j < objects.size();j++) {		
		glPushMatrix();
		glTranslatef(objects[j].altX, objects[j].altY, objects[j].altZ);
		//glRotatef(objects[j].angleX, 1, 0, 0);
		glRotatef(objects[j].angleY, 0, 1, 0);
		glRotatef(objects[j].angleZ, 0, 0, 1);
		glScaled(objects[j].scale, objects[j].scale, objects[j].scale);
		for (int i = 0; i < objects[j].faces.size(); i++) {
			double a = ((ia*objects[j].ka)*objects[j].r + (il*objects[j].kd*objects[j].prod[i])*source1.r);
			double b = ((ia*objects[j].ka)*objects[j].g + (il*objects[j].kd*objects[j].prod[i])*source1.g);
			double c = ((ia*objects[j].ka)*objects[j].b + (il*objects[j].kd*objects[j].prod[i])*source1.b);
			glColor3f(a,b,c);
			glBegin(GL_TRIANGLES);
			glVertex3f(objects[j].points[objects[j].faces[i].p1].x, objects[j].points[objects[j].faces[i].p1].y, objects[j].points[objects[j].faces[i].p1].z);
			glVertex3f(objects[j].points[objects[j].faces[i].p2].x, objects[j].points[objects[j].faces[i].p2].y, objects[j].points[objects[j].faces[i].p2].z);
			glVertex3f(objects[j].points[objects[j].faces[i].p3].x, objects[j].points[objects[j].faces[i].p3].y, objects[j].points[objects[j].faces[i].p3].z);
			glEnd();
		}
		glPopMatrix();
	}

	
	glFlush();
	glutPostRedisplay();
}

void handleMotion(int x, int y)
{
}

void handleMouse(int btn, int state, int x, int y)
{
}

void hadleKeyboard(unsigned char key, int x, int y)
{
	if(key == ESC){
		exit(0);
	}
	if (key == '1'){
		objects[selected_object].altX -= TRANS_SCALA;
	}
	if (key == '2'){
		objects[selected_object].altX += TRANS_SCALA;
	}
	if (key == '3'){
		objects[selected_object].altY -= TRANS_SCALA;
	}
	if (key == '4'){
		objects[selected_object].altY += TRANS_SCALA;
	}
	if (key == '5'){
		objects[selected_object].altZ -= TRANS_SCALA;
	}
	if (key == '6'){
		objects[selected_object].altZ += TRANS_SCALA;
	}
	if (key == '7'){
		objects[selected_object].RotateMatrix(1, 'x');
	}
	if (key == '8'){
		objects[selected_object].RotateMatrix(1, 'y');
	}
	if (key == '9'){
		objects[selected_object].RotateMatrix(1, 'z');
	}
	if (key == '='){
		objects[selected_object].scale = 1.01*objects[selected_object].scale;
	}
	if (key == '-'){
		objects[selected_object].scale = 0.99*objects[selected_object].scale;
	}
	if (key == '.') {
		selected_object = (selected_object + 1) % objects.size();
	}
	if (key == ',') {
		selected_object = (selected_object - 1) % objects.size();
	}
}

void hadleSpecialKeyboard(int key, int x, int y)
{
}

int main(int argc, char **argv)
{
	source1.setColor(1, 1, 1);
	//setando objeto 0:
	/*objects.push_back(readObject("pumpkin.obj"));
	objects[0].selectka(0.6);
	objects[0].selectkd(0.5);
	objects[0].selectColor(1, 0.54902, 0);
	objects[0].recalculate(source1.location);*/
	//setando objeto 1:
	objects.push_back(readObject("dog.obj"));
	objects[0].selectka(0.6);
	objects[0].selectkd(0.5);
	objects[0].selectColor(0.7, 0.5, 0);
	objects[0].recalculate(source1.location);
	//setando objeto 2:
	/*objects.push_back(readObject("teste.obj"));
	objects[2].selectka(0.6);
	objects[2].selectkd(0.5);
	objects[2].selectColor(1,1,1);
	objects[2].recalculate(source1.location);*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Projeto 1");

	glutDisplayFunc(mydisplay);
	glutReshapeFunc(myreshape);
	glutMouseFunc(handleMouse);
	glutMotionFunc(handleMotion);
	glutKeyboardFunc(hadleKeyboard);
	glutSpecialUpFunc(hadleSpecialKeyboard);

	glutMainLoop();
	return 0;
}

