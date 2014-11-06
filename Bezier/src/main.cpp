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
#include <Camera.h>
#include <iostream>
#include <math.h>

using namespace std;
double mouse_x, mouse_y;
double window_width = 800.0;
double window_height = 800.0;
double zbuffer[800][800];
vector<Object3D> objects = vector<Object3D>();
int selected_object = 0;
int selected_light_source = 0;
double ia = 1;
double il = 0.2;
double is = 20;
double eyeX = 0, eyeY = 0, eyeZ = 100;
bool mod = false;
vector<LightSource> sources = vector<LightSource>();
//LightSource source1 = LightSource(0,0,0); // no centro do mundo

//Propriedades da Camera
Camera camera = Camera(
	Point3D(CAMERAX_INICIAL, CAMERAY_INICIAL, CAMERAZ_INICIAL),
	Point3D(CAMERAX_INICIAL+1, CAMERAY_INICIAL, CAMERAZ_INICIAL),
	Point3D(CAMERAX_INICIAL, CAMERAY_INICIAL+1, CAMERAZ_INICIAL),
	Point3D(CAMERAX_INICIAL, CAMERAY_INICIAL, CAMERAZ_INICIAL-1)
	);


//Movimento do mouse;
int mouseInicialX = 0;
int mouseInicialY = 0;

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
	gluPerspective(30, ratio, 0, 3000);
	
}

void resetBuffer(){
	for (int i = 0; i < 800;i++) {
		for (int j = 0; j < 800;j++) {
			buffer[i][j] = DBL_MAX;
		}
	}
}

void drawObjects(){
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	

	for (int j = 0; j < objects.size(); j++) {
		if (mod) {
			objects[j].recalculate(sources[0].location, eyeX, eyeY, eyeZ);
			mod = false;
		}
		glPushMatrix();
		glTranslatef(objects[j].altX, objects[j].altY, objects[j].altZ);
		//glRotatef(objects[j].angleX, 1, 0, 0);
		glRotatef(objects[j].angleY, 0, 1, 0);
		glRotatef(objects[j].angleZ, 0, 0, 1);
		glScaled(objects[j].scale, objects[j].scale, objects[j].scale);

		for (int i = 0; i < objects[j].faces.size(); i++) {
			bool front = decide(objects[j].normais[i]);
			if (front) {
				double a = ia*objects[j].ka*objects[j].r;
				double b = ia*objects[j].ka*objects[j].g;
				double c = ia*objects[j].ka*objects[j].b;
				for (int t = 0; t < sources.size(); t++) {
					a += il*objects[j].kd*objects[j].prod[i] * sources[t].r + (il*is*objects[j].ks*pow(produtoInterno(objects[j].r1[i], objects[j].v[i]), objects[j].q)*sources[t].r) / 255;
					b += il*objects[j].kd*objects[j].prod[i] * sources[t].g + (il*is*objects[j].ks*pow(produtoInterno(objects[j].r1[i], objects[j].v[i]), objects[j].q)*sources[t].g) / 255;
					c += il*objects[j].kd*objects[j].prod[i] * sources[t].b + (il*is*objects[j].ks*pow(produtoInterno(objects[j].r1[i], objects[j].v[i]), objects[j].q)*sources[t].b) / 255;
				}
				glColor3f(a, b, c);//aux.x, aux.y, aux.z
				glBegin(GL_TRIANGLES);
				glVertex3f(objects[j].points[objects[j].faces[i].p1].x, objects[j].points[objects[j].faces[i].p1].y, objects[j].points[objects[j].faces[i].p1].z);
				glVertex3f(objects[j].points[objects[j].faces[i].p2].x, objects[j].points[objects[j].faces[i].p2].y, objects[j].points[objects[j].faces[i].p2].z);
				glVertex3f(objects[j].points[objects[j].faces[i].p3].x, objects[j].points[objects[j].faces[i].p3].y, objects[j].points[objects[j].faces[i].p3].z);
				glEnd();
			}
			
		}
		glPopMatrix();
	}
}

void mydisplay()
{

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	camera.loadCamera();
	
	double x = 0;
	double y = 0;
	double size = 0.5;

	drawObjects();

	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.25, 0.75, 0);
	glBegin(GL_QUADS);
	glVertex3f(-10, -5,10);
	glVertex3f(10, -5, 10);
	glVertex3f(10, -5, -10);
	glVertex3f(-10, -5, -10);
	glEnd();
	glPopMatrix();

	glFlush();
	glutPostRedisplay();
}

void handleMotion(int x, int y)
{
	camera.RotateMatrix((x - mouseInicialX)/ROTAT_CAMERA, 'y');
	camera.RotateMatrix((y - mouseInicialY) / ROTAT_CAMERA, 'x');
	mouseInicialX = x;
	mouseInicialY = y;
	/*cout << ((camera.directionX - camera.center).dot((camera.directionZ - camera.center)) /
		((camera.directionX - camera.center).module()*(camera.directionZ - camera.center).module())) << endl;*/
}

void handleMouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		mouseInicialX = x;
		mouseInicialY = y;
	}
}

void hadleKeyboard(unsigned char key, int x, int y)
{
	mod = true;
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
<<<<<<< HEAD

	if (key == 'q'){
		camera.RotateMatrix(90, 'y');
		/*cout << ((camera.directionX-camera.center).dot((camera.directionZ-camera.center)) /
			((camera.directionX - camera.center).module()*(camera.directionZ - camera.center).module())) << endl;*/
	}

	if (key == 'a'){
		Point3D deslocamento = (camera.directionX - camera.center)*-MOVE_CAMERA;
		camera.translate(deslocamento.x, deslocamento.y, deslocamento.z);
	}
	if (key == 'd'){
		Point3D deslocamento = (camera.directionX - camera.center)*MOVE_CAMERA;
		camera.translate(deslocamento.x, deslocamento.y, deslocamento.z);
	}

	if (key == 'w'){
		Point3D deslocamento = (camera.directionZ - camera.center)*MOVE_CAMERA;
		camera.translate(deslocamento.x, deslocamento.y, deslocamento.z);
	}
	if (key == 's'){
		Point3D deslocamento = (camera.directionZ - camera.center)*-MOVE_CAMERA;
		camera.translate(deslocamento.x, deslocamento.y, deslocamento.z);
	}
=======
	if (key == 'q'){

	}
	if (key == 'w') {

	}
	if (key=='e') {

	}
	if (key=='r') {

	}
	if (key=='t') {

	}
	if (key=='y'){

	}
	if (key=='u') {
		selected_light_source = (selected_light_source + 1) % sources.size();
	}
	if (key =='i') {
		selected_light_source = (selected_light_source - 1) % sources.size();
	}
	/*for (int i = 0; i < objects.size();i++){
		objects[i].recalculate(source1.location, eyeX, eyeY, eyeZ);
	}*/
>>>>>>> origin/cb
}

void handleSpecialKeyboard(int key, int x, int y)
{
}

int main(int argc, char **argv){
	resetBuffer();

	//fonte de luz 1:
	LightSource source1 = LightSource(0, 0, 0);
	source1.setColor(1, 1, 1);
	sources.push_back(source1);

	//fonte de luz 2:
	LightSource source2 = LightSource(2000, 2000, 2000);
	source1.setColor(1, 0, 0);
	//sources.push_back(source2);

	//setando objeto 0:
	objects.push_back(readObject("pumpkin.obj"));
	objects[0].selectka(0.6);
	objects[0].selectkd(0.5);
	objects[0].selectks(0.1);
	objects[0].selectq(1);
	objects[0].selectColor(1, 0.54902, 0);
	objects[0].recalculate(source1.location, eyeX, eyeY, eyeZ);
	//setando objeto 1:
	objects.push_back(readObject("dog.obj"));
	objects[1].selectka(0.6);
	objects[1].selectkd(0.5);
	objects[1].selectks(0.1);
	objects[1].selectq(1);
	objects[1].selectColor(0.7, 0.5, 0);
	objects[1].recalculate(source1.location, eyeX, eyeY, eyeZ);
	//setando objeto 2:
	/*objects.push_back(readObject("teste.obj"));
	objects[2].selectka(0.6);
	objects[2].selectkd(0.5);
	objects[2].selectks(0.5);
	objects[2].selectq(1);
	objects[2].selectColor(1,1,1);
	objects[2].recalculate(source1.location, eyeX, eyeY, eyeZ);*/

	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Projeto 1");
	glutDisplayFunc(mydisplay);
	glutReshapeFunc(myreshape);
	glutIdleFunc(mydisplay);
	glutSpecialFunc(handleSpecialKeyboard);
	glutMouseFunc(handleMouse);
	glutMotionFunc(handleMotion);
	glutKeyboardFunc(hadleKeyboard);
	glutSpecialUpFunc(hadleSpecialKeyboard);

	//glEnable(GL_DEPTH_TEST); //add profundidade, opcional (ver se fica melhor com ou sem)
	glutMainLoop();
	return 1;

}

