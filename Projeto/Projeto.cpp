/*
	Hugo Sobral de Barros - 2020234332
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL\glut.h>

//--------------------------------- Definir cores
#define BLUE 0.0, 0.0, 1.0, 1.0
#define RED 1.0, 0.0, 0.0, 1.0
#define YELLOW 1.0, 1.0, 0.0, 1.0
#define GREEN 0.0, 1.0, 0.0, 1.0
#define ORANGE 1.0, 0.5, 0.1, 1.0
#define WHITE 1.0, 1.0, 1.0, 1.0
#define BLACK 0.0, 0.0, 0.0, 1.0
#define GRAY 0.92, 0.92, 0.92, 1.0
#define PI 3.14159

//===========================================================Variaveis e constantes

float centrox = 0;
float rotacao = 0;
float rotWheels = 0;
float yWindows = 0.7;

GLfloat tam = 0.5;
static GLfloat vertices[] = {
	//…………………………………………………………………………………………………… Esquerda
	-tam, -tam, tam,  // 0
	-tam, tam, tam,	  // 1
	-tam, tam, -tam,  // 2
	-tam, -tam, -tam, // 3
					  //…………………………………………………… Direita
	tam, -tam, tam,	  // 4
	tam, tam, tam,	  // 5
	tam, tam, -tam,	  // 6
	tam, -tam, -tam,  // 7
					  //……………………………………………………… Cima
	-tam, tam, tam,	  // 8
	-tam, tam, -tam,  // 9
	tam, tam, -tam,	  // 10
	tam, tam, tam,	  // 11
					  //……………………………………………………… Tras
	-tam, tam, -tam,  // 12
	tam, tam, -tam,	  // 13
	tam, -tam, -tam,  // 14
	-tam, -tam, -tam, // 15
					  //……………………………………………………… Frente
	-tam, tam, tam,	  // 16
	tam, tam, tam,	  // 17
	tam, -tam, tam,	  // 18
	-tam, -tam, tam,  // 19
					  //……………………………………………………… Baixo
	-tam, -tam, tam,  // 20
	-tam, -tam, -tam, // 21
	tam, -tam, -tam,  // 22
	tam, -tam, tam,	  // 23
};

static GLuint esquerda[] = {0, 1, 2, 3};
static GLuint direita[] = {4, 7, 6, 5};
static GLuint cima[] = {8, 11, 10, 9};
static GLuint tras[] = {12, 13, 14, 15};
static GLuint frente[] = {19, 18, 17, 16};
static GLuint baixo[] = {20, 23, 22, 21};

//------------------------------------------------------------ Sistema Coordenadas + objectos
GLint wScreen = 800, hScreen = 600;		 //.. janela
GLfloat xC = 10.0, yC = 10.0, zC = 10.0; //.. Mundo
GLboolean frenteVisivel = 1;

//------------------------------------------------------------ Observador
GLfloat rVisao = 10, aVisao = 0.5 * PI, incVisao = 0.05;
GLfloat obsP[] = {rVisao * cos(aVisao), 3.0, rVisao *sin(aVisao)};
GLfloat angZoom = 45;
GLfloat incZoom = 3;

//------------------------------------------------------------ Rotacao
GLint msec = 10;
GLint sempreRodar = 0;
GLint windows = 1;
GLint wheelsRotation = 0;

GLfloat altura = 0;
GLfloat incALT = 0.03;


//================================================================================
//=========================================================================== INIT

void init(void)
{
	glClearColor(WHITE);	 //………………………………………………………………………………Apagar
	glShadeModel(GL_SMOOTH); //………………………………………………………………………………Interpolacao de cores
	glEnable(GL_DEPTH_TEST); //………………………………………………………………………………Profundidade

	glVertexPointer(3, GL_FLOAT, 0, vertices); //………………………………………Vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
}

void drawEixos()
{
	glColor4f(RED);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(10, 0, 0);
	glEnd();

	glColor4f(GREEN);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 10, 0);
	glEnd();

	glColor4f(BLUE);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 10);
	glEnd();
}

void drawCube()
{
	glPushMatrix();
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
	glPopMatrix();
}

void drawWheel()
{
	GLUquadricObj *q = gluNewQuadric();
	// gluQuadricTexture(q, GL_TRUE);

	gluQuadricDrawStyle(q, GLU_FLAT);
	gluQuadricNormals(q, GLU_SMOOTH);

	glColor4f(BLACK);
	gluCylinder(q, .3, .3, .4, 20, 20);
	glColor4f(BLUE);
	gluDisk(q, 0, .3, 20, 20);
	gluDeleteQuadric(q);
	glColor4f(ORANGE);
	glPushMatrix();
	glScalef(0.1,.5,0.05);
	drawCube();
	glPopMatrix();
	glColor4f(ORANGE);
	glPushMatrix();
	glRotatef(90, 0,0,1);
	glScalef(0.1,.5,0.05);
	drawCube();
	glPopMatrix();
}

void drawWheels()
{
	GLfloat x_wheel = centrox + 1.1;
	GLfloat y_wheel = -0.5;
	GLfloat z_wheel = 1;

	// direita frente
	glPushMatrix();
	glTranslatef(x_wheel, y_wheel, z_wheel);
	glRotatef(-90, 0, 1, 0);
	glRotatef(rotWheels, 0, 0, 1);
	drawWheel();
	glPopMatrix();

	// direita tras
	glPushMatrix();
	glTranslatef(x_wheel, y_wheel, -z_wheel);
	glRotatef(-90, 0, 1, 0);
	glRotatef(rotWheels, 0, 0, 1);
	drawWheel();
	glPopMatrix();

	// esquerda frente
	glPushMatrix();
	glTranslatef(-x_wheel, y_wheel, z_wheel);
	glRotatef(90, 0, 1, 0);
	glRotatef(-rotWheels, 0, 0, 1);
	drawWheel();
	glPopMatrix();

	// esquerda tras
	glPushMatrix();
	glTranslatef(-x_wheel, y_wheel, -z_wheel);
	glRotatef(90, 0, 1, 0);
	glRotatef(-rotWheels, 0, 0, 1);
	drawWheel();
	glPopMatrix();
}

void drawWindows()
{
	glColor4f(BLUE);

	glPushMatrix();
	glTranslatef(.75, yWindows, 0);
	glScalef(0.1, 0.5, 1.2);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-.75, yWindows, 0);
	glScalef(0.1, 0.5, 1.2);
	drawCube();
	glPopMatrix();
}

void desenhaTexto(char* string, GLfloat x, GLfloat y)
{
	glRasterPos2f(x, y);
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
}

void drawScene()
{
	//==================================== MESA
	if (sempreRodar) rotacao = rotacao + 3;

	if (windows && yWindows <= 0.7) yWindows = yWindows + 0.01;
	
	else if (!windows && yWindows > 0.2) yWindows = yWindows - 0.01;

	if (wheelsRotation) rotWheels = rotWheels + 2.5;

	// ======== BASE
	glPushMatrix();
	glColor4f(RED);
	glRotatef(rotacao, 0.0, 1.0, 0.0);
	glTranslatef(centrox, altura, 0.0);
	glScalef(2, 1, 3);
	drawCube();
	glPopMatrix();

	// ======== CABIN
	glPushMatrix();
	glColor4f(ORANGE);
	glRotatef(rotacao, 0.0, 1.0, 0.0);
	glTranslatef(centrox, altura + .75, 0);
	glScalef(1.5, .6, 2);
	drawCube();
	glPopMatrix();

	// ========= WHEELS
	glPushMatrix();
	glRotatef(rotacao, 0.0, 1.0, 0.0);
	drawWheels();
	glPopMatrix();

	// ======== WINDOWS
	glPushMatrix();
	glRotatef(rotacao, 0.0, 1.0, 0.0);
	drawWindows();
	glPopMatrix();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, 3.0 * zC);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);

	//…………………………………………………………………………………………………………………………………………………………Objectos
	drawEixos();
	drawScene();

	//Vista contraria
	glViewport(0, 0, wScreen*0.25, hScreen*0.25);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)wScreen / hScreen, 0.1, 3.0 * zC);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(-obsP[0], obsP[1], -obsP[2], 0, 0, 0, 0, 1, 0);

	drawEixos();
	drawScene();

	glutSwapBuffers();
}

void Timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1);
}

//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 'R':
	case 'r':
		sempreRodar = !sempreRodar;
		glutPostRedisplay();
		break;

	case 'e':
	case 'E':
		rotacao = rotacao + 2.5;
		glutPostRedisplay();
		break;

	case 'd':
	case 'D':
		rotacao = rotacao - 2.5;
		glutPostRedisplay();
		break;

	case 'x':
	case 'X':
		rVisao = rVisao++;
		obsP[0] = rVisao * cos(aVisao);
		obsP[2] = rVisao * sin(aVisao);
		glutPostRedisplay();
		break;

	case 'y':
	case 'Y':
		rVisao = rVisao--;
		obsP[0] = rVisao * cos(aVisao);
		obsP[2] = rVisao * sin(aVisao);
		glutPostRedisplay();
		break;

	case 'o':
	case 'O':
		wheelsRotation = !wheelsRotation;
		glutPostRedisplay();
		break;

	case 'l':
	case 'L':
		windows = !windows;
		glutPostRedisplay();
		break;

	case '0':
		obsP[0] = rVisao * cos(aVisao);
		obsP[2] = rVisao * sin(aVisao);
		glutPostRedisplay();
		break;

	case '1':
		obsP[0] = 5;
		obsP[1] = 5;
		obsP[2] = 5;
		glutPostRedisplay();
		break;

		//--------------------------- Escape
	case 27:
		exit(0);
		break;
	}
}

void teclasNotAscii(int key, int x, int y)
{

	if (key == GLUT_KEY_UP)
		obsP[1] = (obsP[1] + 0.3);
	if (key == GLUT_KEY_DOWN)
		obsP[1] = (obsP[1] - 0.3);

	if (obsP[1] > yC)
		obsP[1] = yC;
	if (obsP[1] < -yC)
		obsP[1] = -yC;

	if (key == GLUT_KEY_LEFT)
		aVisao = (aVisao + 0.1);
	if (key == GLUT_KEY_RIGHT)
		aVisao = (aVisao - 0.1);

	obsP[0] = rVisao * cos(aVisao);
	obsP[2] = rVisao * sin(aVisao);

	glutPostRedisplay();
}

//======================================================= MAIN
//======================================================= MAIN
int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(300, 100);
	glutCreateWindow("Rodar-'r'|  |Observador:'SETAS'|  |Vidros-'L'|  |Rodar -'e/d'| |Rodas:'O'| ||");

	init();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}
