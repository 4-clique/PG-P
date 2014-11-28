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
double window_width = 1080.0;
double window_height = 1920.0;
vector<Object3D> objects = vector<Object3D>();
int selected_object = 0;
int selected_light_source = 0;
int togle = -1;
double ia = 0.5;
double id = 0.4;
double is = 0.25;
int t = 1;
vector<LightSource> sources = vector<LightSource>();
bool diretorON = false;
float ratio;
double angle = 35;
double near = 1;

//Propriedades da Camera
Camera camera = Camera(
	Point3D(CAMERAX_INICIAL, CAMERAY_INICIAL, CAMERAZ_INICIAL),
	Point3D(CAMERAX_INICIAL+1, CAMERAY_INICIAL, CAMERAZ_INICIAL),
	Point3D(CAMERAX_INICIAL, CAMERAY_INICIAL+1, CAMERAZ_INICIAL),
	Point3D(CAMERAX_INICIAL, CAMERAY_INICIAL, CAMERAZ_INICIAL-1)
	);

Camera diretor = Camera(
	Point3D(DIRETORX, DIRETORY, DIRETORZ),
	Point3D(DIRETORX + 1, DIRETORY, DIRETORZ),
	Point3D(DIRETORX, DIRETORY + 1, DIRETORZ),
	Point3D(DIRETORX, DIRETORY, DIRETORZ - 1)
	);

//Movimento do mouse;
int mouseInicialX = 0;
int mouseInicialY = 0;


void myreshape (GLsizei w, GLsizei h)
{
	// evita uma divis�o por zero na hora do reshape
	if (h == 0) h = 1;
	if (diretorON) ratio = (w * 1.0 / h)/2; // ratio d� no msm do w/h, s� que tem isso de haver o retorno do h como 0, o que causa erro
	else ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	glViewport(0, 0, w, h);
	window_width = (GLfloat) w;
	window_height = (GLfloat) h;
	gluPerspective(30, ratio, 1, 3000);
	glEnable(GL_DEPTH_TEST); //add profundidade, opcional (ver se fica melhor com ou sem)
	glDepthFunc(GL_LESS);
}


void drawPlane(){
	glColor3f(.3, .3, .3);
	glBegin(GL_QUADS);
	glVertex3f(0, -5, 0);
	glVertex3f(0, -5, 10);
	glVertex3f(10, -5, 10);
	glVertex3f(10, -5, 0);
	glEnd();

	glBegin(GL_LINES);
	for (int i = 0; i <= 10; i++) {
		if (i == 0) { glColor3f(.6, .3, .3); }
		else { glColor3f(.25, .25, .25); };
		glVertex3f(i, -5, 0);
		glVertex3f(i, -5, 10);
		if (i == 0) { glColor3f(.3, .3, .6); }
		else { glColor3f(.25, .25, .25); };
		glVertex3f(0, -5, i);
		glVertex3f(10, -5, i);
	};
	glEnd();

	glColor3f(0.5,0,0);
	glBegin(GL_QUADS);
	glVertex3f(-120,120,-100);
	glVertex3f(-120, -120, -100);
	glVertex3f(120, -120, -100);
	glVertex3f(120, 120, -100);
	glEnd();
	glColor3f(0.5, 0.5, 0);
	glBegin(GL_QUADS);
	glVertex3f(-60, 60, -99);
	glVertex3f(-60, -60, -99);
	glVertex3f(60, -60, -99);
	glVertex3f(60, 60, -99);
	glEnd();
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_QUADS);
	glVertex3f(-30, 30, -98);
	glVertex3f(-30, -30, -98);
	glVertex3f(30, -30, -98);
	glVertex3f(30, 30, -98);
	glEnd();
}

void drawObjects(){

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int j = 0; j < objects.size(); j++) {
		glPushMatrix();
		//glTranslatef(objects[j].altX, objects[j].altY, objects[j].altZ);
		//glRotatef(objects[j].angleX, 1, 0, 0);
		glScaled(objects[j].scale, objects[j].scale, objects[j].scale);

		for (int i = 0; i < objects[j].faces.size(); i++) {
			double r = 0;
			double g = 0;
			double b = 0;
			for (int t = 0; t < sources.size(); t++) {
				r += ia*objects[j].ka*sources[t].r;
				g += ia*objects[j].ka*sources[t].g;
				b += ia*objects[j].ka*sources[t].b;
				Point3D direcaoNormal = objects[j].normais[i];
				Point3D direcaoF = sources[t].location - objects[j].faces[i].p1;
				direcaoF = direcaoF.normalized();
					//r += id*objects[j].kd*(direcaoF.dot(direcaoNormal)) * sources[t].r;
					//g += id*objects[j].kd*(direcaoF.dot(direcaoNormal)) * sources[t].g;
					//b += id*objects[j].kd*(direcaoF.dot(direcaoNormal)) * sources[t].b;
				/*	r += (is*objects[j].ks*pow(objects[j].reflexaoLuz[i].dot(objects[j].v[i]), objects[j].q));
					g += (is*objects[j].ks*pow(objects[j].reflexaoLuz[i].dot(objects[j].v[i]), objects[j].q));
					b += (is*objects[j].ks*pow(objects[j].reflexaoLuz[i].dot(objects[j].v[i]), objects[j].q));*/

				if (direcaoF.dot(direcaoNormal) > 0){
					//cout << direcaoF.dot(direcaoNormal) << endl;
					r += id*objects[j].kd*(direcaoF.dot(direcaoNormal)) * sources[t].r;
					g += id*objects[j].kd*(direcaoF.dot(direcaoNormal)) * sources[t].g;
					b += id*objects[j].kd*(direcaoF.dot(direcaoNormal)) * sources[t].b;
				}
				if (objects[j].reflexaoLuz[i].dot(objects[j].v[i]) > 0){
					r += (is*objects[j].ks*pow(objects[j].reflexaoLuz[i].dot(objects[j].v[i]), objects[j].q));
					g += (is*objects[j].ks*pow(objects[j].reflexaoLuz[i].dot(objects[j].v[i]), objects[j].q));
					b += (is*objects[j].ks*pow(objects[j].reflexaoLuz[i].dot(objects[j].v[i]), objects[j].q));
				}
			}
			glColor3f(r*objects[j].r, g*objects[j].g, b*objects[j].b);//aux.x, aux.y, aux.z
			glBegin(GL_TRIANGLES);
			glVertex3f(objects[j].points[objects[j].faces[i].p1].x, objects[j].points[objects[j].faces[i].p1].y, objects[j].points[objects[j].faces[i].p1].z);
			glVertex3f(objects[j].points[objects[j].faces[i].p2].x, objects[j].points[objects[j].faces[i].p2].y, objects[j].points[objects[j].faces[i].p2].z);
			glVertex3f(objects[j].points[objects[j].faces[i].p3].x, objects[j].points[objects[j].faces[i].p3].y, objects[j].points[objects[j].faces[i].p3].z);
			glEnd();
			
		}
		glPopMatrix();
	}
}

void drawCamera(Camera camera) {
	/*
	Point3D center;
	Point3D directionX;
	Point3D directionY;
	Point3D directionZ;
	*/
	Point3D ct = camera.center;
	//direction x:
	/*glBegin(GL_LINES);
	
	glColor3f(1, 0, 0);
	glVertex3f(camera.directionX.x, camera.directionX.y, camera.directionX.z);
	glVertex3f(ct.x, ct.y, ct.z);
	glEnd();
	//direction y:
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(camera.directionY.x, camera.directionY.y, camera.directionY.z);
	glVertex3f(ct.x, ct.y, ct.z);
	glEnd();
	//direction z:
	//glEnable(GL_LINE_WIDTH);//GL_LINE_SMOOTH*/

	glBegin(GL_LINES);
	glColor3f(1, 1, 0);
	glVertex3d(camera.directionZ.x, camera.directionZ.y, camera.directionZ.z);
	glVertex3d(ct.x, ct.y, ct.z);
	glEnd();
	//quadrado:
	for (int i = 0; i < camera.cubo.size(); i++) {
		for (int j = i + 1; j < camera.cubo.size(); j++) {
			glBegin(GL_LINES);
			glColor3f(0, 1, 0);
			glVertex3d(camera.cubo[i].x, camera.cubo[i].y, camera.cubo[i].z);
			glVertex3d(camera.cubo[j].x, camera.cubo[j].y, camera.cubo[j].z);
			glEnd();
		}
	}
	//pontos do cubo:
	for (int i = 0; i < 8; i++) {
		glPointSize(10);
		glBegin(GL_POINTS);
		
		glColor3f(0, 0, 1);
		glVertex3f(camera.cubo[i].x, camera.cubo[i].y, camera.cubo[i].z);
		//printf("cubo>> %f %f %f", diretor.cubo[i].x, diretor.cubo[i].y, diretor.cubo[i].z);
		glEnd();
	}
	/*glBegin(GL_POINTS);
	glPointSize(5);
	glColor3f(0, 0, 1);
	glVertex3f(diretor.cubo[0].x, diretor.cubo[0].y, diretor.cubo[0].z);
	//printf("cubo>> %f %f %f", diretor.cubo[i].x, diretor.cubo[i].y, diretor.cubo[i].z);
	glEnd();*/
	//cilindro:
	/*glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslated(ct.x, ct.y, ct.z);
	//gluCylinder(1);
	glPopMatrix();*/
}

void mydisplay()
{

	//glClearColor(0, 0, 0, 0);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	double x = 0;
	double y = 0;
	double size = 0.5;
	
	if (diretorON) {
		//JANELA 1:
		glViewport(0, 0, window_width / 2, window_height);
		camera.loadCamera();
		drawObjects();

		//JANELA 2:
		glViewport(window_width / 2, 0, window_width / 2, window_height);
		diretor.loadCamera();
		drawObjects();
		drawCamera(camera);
	}
	else {
		glViewport(0, 0, window_width, window_height);
		camera.loadCamera();
		drawObjects();
	}

	
	
	glFlush();
	glutPostRedisplay();
}

void handleMotion(int x, int y)
{
	//double degrees =  ((x - mouseInicialX) / ROTAT_CAMERA);//acos(-1.0)*
	camera.RotateMatrix((x - mouseInicialX)/ROTAT_CAMERA, 'y');
	camera.RotateMatrix((y - mouseInicialY) / ROTAT_CAMERA, 'x');
	mouseInicialX = x;
	mouseInicialY = y;
	for (int i = 0; i < sources.size(); i++){
		objects[selected_object].recalculate(sources[i].location, camera.center);
	}
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
	if(key == ESC){
		exit(0);
	}
	if (key == '1'){
		objects[selected_object].translate(-TRANS_SCALA, 0, 0);
		for (int i = 0; i < sources.size(); i++){
			objects[selected_object].recalculate(sources[i].location, camera.center);
		}		
	}
	if (key == '2'){
		objects[selected_object].translate(TRANS_SCALA, 0, 0);
		for (int i = 0; i < sources.size(); i++){
			objects[selected_object].recalculate(sources[i].location, camera.center);
		}
	}
	if (key == '3'){
		objects[selected_object].translate(0, -TRANS_SCALA, 0);
		for (int i = 0; i < sources.size(); i++){
			objects[selected_object].recalculate(sources[i].location, camera.center);
		}
	}
	if (key == '4'){
		objects[selected_object].translate(0, TRANS_SCALA, 0);
		for (int i = 0; i < sources.size(); i++){
			objects[selected_object].recalculate(sources[i].location, camera.center);
		}
	}
	if (key == '5'){
		objects[selected_object].translate(0, 0, -TRANS_SCALA);
		for (int i = 0; i < sources.size(); i++){
			objects[selected_object].recalculate(sources[i].location, camera.center);
		}
	}
	if (key == '6'){
		objects[selected_object].translate(0, 0, TRANS_SCALA);
		for (int i = 0; i < sources.size(); i++){
			objects[selected_object].recalculate(sources[i].location, camera.center);
		}
	}
	if (key == '7'){
		objects[selected_object].RotateMatrix(1, 'x');
		for (int i = 0; i < sources.size(); i++){
			objects[selected_object].recalculate(sources[i].location, camera.center);
		}
	}
	if (key == '8'){
		objects[selected_object].RotateMatrix(1, 'y');
		for (int i = 0; i < sources.size(); i++){
			objects[selected_object].recalculate(sources[i].location, camera.center);
		}
	}
	if (key == '9'){
		objects[selected_object].RotateMatrix(1, 'z');
		for (int i = 0; i < sources.size(); i++){
			objects[selected_object].recalculate(sources[i].location, camera.center);
		}
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
	if (key == 'a'){
		Point3D deslocamento = (camera.directionX - camera.center)*-MOVE_CAMERA;
		camera.translate(deslocamento.x, deslocamento.y, deslocamento.z);
		for (int i = 0; i < sources.size(); i++){
			objects[selected_object].recalculate(sources[i].location, camera.center);
		}
	}
	if (key == 'd'){
		Point3D deslocamento = (camera.directionX - camera.center)*MOVE_CAMERA;
		camera.translate(deslocamento.x, deslocamento.y, deslocamento.z);
		for (int i = 0; i < sources.size(); i++){
			objects[selected_object].recalculate(sources[i].location, camera.center);
		}
	}

	if (key == 'w'){
		Point3D deslocamento = (camera.directionZ - camera.center)*MOVE_CAMERA;
		camera.translate(deslocamento.x, deslocamento.y, deslocamento.z);
		for (int i = 0; i < sources.size(); i++){
			objects[selected_object].recalculate(sources[i].location, camera.center);
		}
	}
	if (key == 's'){
		Point3D deslocamento = (camera.directionZ - camera.center)*-MOVE_CAMERA;
		camera.translate(deslocamento.x, deslocamento.y, deslocamento.z);
		for (int i = 0; i < sources.size(); i++){
			objects[selected_object].recalculate(sources[i].location, camera.center);
		}
	}
	if (key == 'f'){
		Point3D cameraDirection = (camera.directionZ - camera.center).normalized();
		Point3D objectDirection = (objects[selected_object].bariCenter - camera.center).normalized();
		cout << cameraDirection.x - objectDirection.x << endl;
		if (cameraDirection.x - objectDirection.x <= -0.1){
			if ((acos(cameraDirection.dot(objectDirection))*180.0 / acos(-1)) > 1.02) camera.RotateMatrix(-(acos(cameraDirection.dot(objectDirection))*180.0 / acos(-1)), 'y');
		}
		else if (cameraDirection.x - objectDirection.x >= 0.1){
			if ((acos(cameraDirection.dot(objectDirection))*180.0 / acos(-1)) > 1.02) camera.RotateMatrix((acos(cameraDirection.dot(objectDirection))*180.0 / acos(-1)), 'y');
		}
	}
	if (key=='e') {
		sources[selected_light_source].location.x -= LIGHT_SCALA;
	}
	if (key=='r') {
		sources[selected_light_source].location.x += LIGHT_SCALA;
	}
	if (key=='t') {
		sources[selected_light_source].location.y -= LIGHT_SCALA;
	}
	if (key=='y'){
		sources[selected_light_source].location.y += LIGHT_SCALA;
	}
	if (key == 'u') {
		sources[selected_light_source].location.z -= LIGHT_SCALA;
	}
	if (key == 'i') {
		sources[selected_light_source].location.z += LIGHT_SCALA;
	}
	if (key=='o') {
		selected_light_source = (selected_light_source + 1) % sources.size();
	}
	if (key == 'p') {
		selected_light_source = (selected_light_source - 1) % sources.size();
	}
	if (key == '[') {
		if (angle < 64){
			angle += 0.275;
			Point3D deslocamento = (camera.directionZ - camera.center)*0.1;
			camera.translate(deslocamento.x, deslocamento.y, deslocamento.z);
			for (int i = 0; i < sources.size(); i++){
				objects[selected_object].recalculate(sources[i].location, camera.center);
			}
		}
		
	}
	if (key == ']') {
		if (angle > 35){
			angle -= 0.275;
			Point3D deslocamento = (camera.directionZ - camera.center)*-0.1;
			camera.translate(deslocamento.x, deslocamento.y, deslocamento.z);
			for (int i = 0; i < sources.size(); i++){
				objects[selected_object].recalculate(sources[i].location, camera.center);
			}
		}
	}
	if (key == 'x') {
		if (diretorON) {
			diretorON = false;
			myreshape(window_width, window_height);
		}
		else {
			diretorON = true;
			myreshape(window_width, window_height);
		}
	}
	if (key == 'm') {
		Point3D deslocamento = (diretor.directionX - diretor.center)*-MOVE_CAMERA;
		diretor.translate(deslocamento.x, deslocamento.y, deslocamento.z);
	}
	if (key == 'n') {
		Point3D deslocamento = (diretor.directionX - diretor.center)*+MOVE_CAMERA;
		diretor.translate(deslocamento.x, deslocamento.y, deslocamento.z);
	}
	if (key == 'b') {
		Point3D deslocamento = (diretor.directionY - diretor.center)*+MOVE_CAMERA;
		diretor.translate(deslocamento.x, deslocamento.y, deslocamento.z);
	}
	
}

void hadleSpecialKeyboard(int key, int x, int y){
	if (key == GLUT_KEY_F1){
		//printf("entrou hihihi");
		if (!PAINT_FACE){
			objects[selected_object].r = 0;
			objects[selected_object].g = 0.1;
			objects[selected_object].b = 1;
		}

	}
	if (key == GLUT_KEY_F2){
		//printf("entrou hihihi");
		if (!PAINT_FACE){
			objects[selected_object].r = 0;
			objects[selected_object].g = 0.8;
			objects[selected_object].b = 0;
		}

	}
	if (key == GLUT_KEY_F3){
		//printf("entrou hihihi");
		if (!PAINT_FACE){
			objects[selected_object].r = 1;
			objects[selected_object].g = 0;
			objects[selected_object].b = 0;
		}
		
	}
	if (key == GLUT_KEY_F4){
		//printf("entrou hihihi");
		if (!PAINT_FACE){
			objects[selected_object].r = 0.5;
			objects[selected_object].g = 0;
			objects[selected_object].b = 0.5;
		}
	}
	if (key == GLUT_KEY_F5){
		//printf("entrou hihihi");
		if (!PAINT_FACE){
			objects[selected_object].r = 1;
			objects[selected_object].g = 0.2;
			objects[selected_object].b = 0.4;
		}

	}
	if (key == GLUT_KEY_F6){
		//printf("entrou hihihi");
		if (!PAINT_FACE){
			objects[selected_object].r = 1;
			objects[selected_object].g = 0.6;
			objects[selected_object].b = 0.1;
		}

	}
	if (key == GLUT_KEY_F7){
		//printf("entrou hihihi");
		if (!PAINT_FACE){
			objects[selected_object].r = 1;
			objects[selected_object].g = 1;
			objects[selected_object].b = 0;
		}

	}
	if (key == GLUT_KEY_F8){
		//printf("entrou hihihi");
		if (!PAINT_FACE){
			objects[selected_object].r = 1;
			objects[selected_object].g = 1;
			objects[selected_object].b = 1;
		}
	}
	if (key == GLUT_KEY_F10){
		//printf("entrou hihihi");
		if (!PAINT_FACE){
			objects[selected_object].r = 0.67;
			objects[selected_object].g = 1;
			objects[selected_object].b = 0.18;
		}

	}
	if (key == GLUT_KEY_F11){
		//printf("entrou hihihi");
		if (!PAINT_FACE){
			objects[selected_object].r = 0.5;
			objects[selected_object].g = 1;
			objects[selected_object].b = 0.8;
		}

	}
	if (key == GLUT_KEY_F9){
		//printf("entrou hihihi");
		if (!PAINT_FACE){
			objects[selected_object].r = 0.87;
			objects[selected_object].g = 0.72;
			objects[selected_object].b = 0.53;
		}

	}
}



int main(int argc, char **argv){
	//fonte de luz 1:
	LightSource source1 = LightSource(0, 0 , 1000);
	source1.setColor(1, 1, 1);
	sources.push_back(source1);

	//fonte de luz 2:
	LightSource source2 = LightSource(0, 0, 11);
	source2.setColor(0, 0, 1);
	//sources.push_back(source2);

	//setando objeto 0:
	/*objects.push_back(readObject("pumpkin.obj"));
	objects[0].selectka(0.3);
	objects[0].selectkd(0.45);
	objects[0].selectks(1);
	objects[0].selectq(10);
	objects[0].selectColor(0.8, 0.4, 0);
	objects[0].recalculate(source1.location, camera.center);*/
	//setando objeto 1:
	/*objects.push_back(readObject("dog.obj"));
	objects[0].selectka(0.6);
	objects[0].selectkd(0.5);
	objects[0].selectks(0.55);
	objects[0].selectq(1);
	objects[0].selectColor(1, 1, 1);
	objects[0].recalculate(source1.location, camera.center);*/
	
	//setando objeto:
	objects.push_back(readObject("Dog.obj"));
	objects[0].selectka(0.6);
	objects[0].selectkd(0.5);
	objects[0].selectks(0.5);
	objects[0].selectq(1);
	objects[0].selectColor(1, 1, 0);
	objects[0].recalculate(source1.location, camera.center);

	//setando objeto 2:
	objects.push_back(readObject("teste.obj"));
	objects[1].selectka(0.6);
	objects[1].selectkd(0.5);
	objects[1].selectks(0.5);
	objects[1].selectq(1);
	objects[1].selectColor(1, 0, 1);
	objects[1].recalculate(source1.location, camera.center);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Projeto 1");
	
	glutDisplayFunc(mydisplay);
	glutReshapeFunc(myreshape);
	glutIdleFunc(mydisplay);
	glutSpecialFunc(hadleSpecialKeyboard);
	glutMouseFunc(handleMouse);
	glutMotionFunc(handleMotion);
	glutKeyboardFunc(hadleKeyboard);
	glutSpecialUpFunc(hadleSpecialKeyboard);

	glutMainLoop();
	return 1;

}

