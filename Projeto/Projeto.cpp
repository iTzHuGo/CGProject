/*=================================================================================
	Hugo Sobral de Barros - 2020234332
	Projeto - Meta 2
=================================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL\glut.h>
#include "RgbImage.h"

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

//--------------------------------- Variaveis e constantes

float centrox = 0;
float rotacao = 0;
float rotWheels = 0;
float yWindows = 0.7;

GLfloat tam = 0.5;
static GLfloat vertices[] = {
	//--------------------------------- Esquerda
	-tam, -tam, tam,  // 0
	-tam, tam, tam,	  // 1
	-tam, tam, -tam,  // 2
	-tam, -tam, -tam, // 3
					  //--------------------- Direita
	tam, -tam, tam,	  // 4
	tam, tam, tam,	  // 5
	tam, tam, -tam,	  // 6
	tam, -tam, -tam,  // 7
					  //--------------------- Cima
	-tam, tam, tam,	  // 8
	-tam, tam, -tam,  // 9
	tam, tam, -tam,	  // 10
	tam, tam, tam,	  // 11
					  //--------------------- Tras
	-tam, tam, -tam,  // 12
	tam, tam, -tam,	  // 13
	tam, -tam, -tam,  // 14
	-tam, -tam, -tam, // 15
					  //--------------------- Frente
	-tam, tam, tam,	  // 16
	tam, tam, tam,	  // 17
	tam, -tam, tam,	  // 18
	-tam, -tam, tam,  // 19
					  //--------------------- Baixo
	-tam, -tam, tam,  // 20
	-tam, -tam, -tam, // 21
	tam, -tam, -tam,  // 22
	tam, -tam, tam,	  // 23
};

static GLfloat texturas[] = {
	//------------------------------- Cima
	0, 0,
	1, 0,
	0, 1,
	1, 1,
};

static GLuint esquerda[] = {0, 1, 2, 3};
static GLuint direita[] = {4, 7, 6, 5};
static GLuint cima[] = {8, 11, 10, 9};
static GLuint tras[] = {12, 13, 14, 15};
static GLuint frente[] = {19, 18, 17, 16};
static GLuint baixo[] = {20, 23, 22, 21};

//------------------------------------------------------------ Sistema Coordenadas + objectos
GLint wScreen = 800, hScreen = 600;		 //.. janela
GLfloat xC = 100.0, yC = 100.0, zC = 100.0; //.. Mundo
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

//------------------------------------------------------------ Texturas
RgbImage imag;
GLuint texture[2];

//------------------------------------------------------------ Skybox
GLUquadricObj* esfera = gluNewQuadric();



//=================================================================================
//====================================== INIT =====================================
//=================================================================================

void initTexturas()
{
	//----------------------------------------- topo
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	imag.LoadBmpFile("mari.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
		
	// Fundo
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	imag.LoadBmpFile("2.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

}

void init(void)
{
	//------------------------------Apagar
	glClearColor(WHITE);

	//------------------------------Interpolacao de cores
	glShadeModel(GL_SMOOTH);

	//------------------------------Texturas
	initTexturas();

	//------------------------------Profundidade
	glEnable(GL_DEPTH_TEST);
	
	//------------------------------Blending
	glEnable(GL_BLEND);


	//------------------------------Vertex arrays
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
}

GLvoid resize(GLsizei width, GLsizei height)
{
	wScreen = width;
	hScreen = height;
	glViewport(0, 0, wScreen, hScreen);
	glutPostRedisplay();	
}

//=================================================================================
//====================================== DRAW =====================================
//=================================================================================

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

void drawEsfera() {
    glColor4f(WHITE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    gluQuadricDrawStyle(esfera, GLU_FILL);
    gluQuadricNormals(esfera, GLU_SMOOTH);
    gluQuadricTexture(esfera, GL_TRUE);
    gluSphere(esfera, 60.0, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
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
	glColor4f(0.0, 1.0, 1.0, 0.5);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
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

	glBlendFunc(GL_ONE, GL_ZERO);
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
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);

	glColor4f(WHITE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glNormal3d(0, 1, 0);

	glBegin(GL_QUADS);


		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(tam, tam, -tam);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(tam, tam, tam);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-tam, tam, tam);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-tam, tam, -tam);

	glEnd();

	glDisable(GL_TEXTURE_2D);

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

	drawEsfera();

	drawScene();

	//Vista contraria
	glViewport(0, 0, wScreen*0.25, hScreen*0.25);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)wScreen / hScreen, 0.1, 3.0 * zC);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(-obsP[0], obsP[1], -obsP[2], 0, 0, 0, 0, 1, 0);
	
	drawScene();

	glutSwapBuffers();
}

void Timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1);
}

//------------------------------------------------------------ Eventos
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

//=================================================================================
//====================================== MAIN =====================================
//=================================================================================
int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(300, 100);
	glutCreateWindow("Rodar-'r'|  |Observador:'SETAS'|  |Vidros-'L'|  |Rodar -'e/d'| |Rodas:'O'| ||");

	init();

	glutSpecialFunc(teclasNotAscii);
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}
