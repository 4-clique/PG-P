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
#include <iostream>
#include <math.h>

using namespace std;
GLfloat mouse_x, mouse_y;
GLfloat window_width = 800.0;
GLfloat window_height = 800.0;
vector<Object3D> objects = vector<Object3D>();
int selected_object = 0;

float lx = 0.0f, lz = -1.0f; //line of sight
float eyeX = 0.0f, eyeZ = 5.0f; //posição da câmera
float angle = 0.0f;

void myreshape (GLsizei w, GLsizei h)
{
	// evita uma divisão por zero na hora do reshape
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h; // ratio dá no msm do w/h, só que tem isso de haver o retorno do h como 0, o que causa erro

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	window_width = (GLfloat) w;
	window_height = (GLfloat) h;
	gluPerspective(45, ratio, 0, 1);
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 1); // a visão de objetos mais distantes
	glMatrixMode(GL_MODELVIEW);
}

void mydisplay(void)
{

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity(); //Substitui matrizes pela identidade (sem isso a câmera não funciona bem)
	
	//Look At Point = Line Of Sight + Camera Position >> em gluLookAt, os parâmetros centerX e centerZ são baseados nessa equação

	gluLookAt(eyeX, 1.0f, eyeZ, //visão do observador
		eyeX + lx, 1.0f, eyeZ + lz, //lx e lz estão relacionados com o giro da câmera para direita e esquerda
		.0f, 1.0f, .0f); //plano de visão da câmera, mudar um dos parâmetros, altera a inclinação do mundo

	double x = 0;
	double y = 0;
	double size = 0.5;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (int j = 0; j < objects.size();j++) {
		if(j == selected_object) glColor3f(1, 0.54902, 0);//1 0.54902 0
		else glColor3f(1, 1, 1);
		glPushMatrix();
		glTranslatef(objects[j].altX, objects[j].altY, objects[j].altZ);
		//glRotatef(objects[j].angleX, 1, 0, 0);
		glRotatef(objects[j].angleY, 0, 1, 0);
		glRotatef(objects[j].angleZ, 0, 0, 1);
		glScaled(objects[j].scale, objects[j].scale, objects[j].scale);
		for (int i = 0; i < objects[j].faces.size(); i++) {
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

	if (key == 'a'){ //câmera gira para a esquerda (objetos vão ficando pra direita)
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
	}
	if (key == 'd'){ //câmera gira para a direita (objetos vão ficando pra esquerda)
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
	}

	// lx ou lz multiplicados por um fator de aproximação; quanto maior, mais rápido se aproxima dos objetos
	if (key == 'w'){ //aproximação da câmera
		eyeX += lx * 2;
		eyeZ += lz * 2;
	}
	if (key == 's'){ //afastamento da câmera
		eyeX -= lx * 2;
		eyeZ -= lz * 2;
	}
}

void hadleSpecialKeyboard(int key, int x, int y)
{
}


int main(int argc, char **argv)
{
	objects.push_back(readObject("pumpkin.obj"));
	objects.push_back(readObject("teste.obj"));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Projeto 1");

	glutDisplayFunc(mydisplay);
	glutReshapeFunc(myreshape);
	glutIdleFunc(mydisplay);
	//glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(handleMouse);
	glutMotionFunc(handleMotion);
	glutKeyboardFunc(hadleKeyboard);
	glutSpecialUpFunc(hadleSpecialKeyboard);
	
	//glEnable(GL_DEPTH_TEST); //add profundidade, opcional (ver se fica melhor com ou sem)

	glutMainLoop();
	return 1;

}
