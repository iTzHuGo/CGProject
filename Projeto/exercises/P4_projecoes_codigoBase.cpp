/* ===================================================================================
	Departamento Eng. Informatica - FCTUC
	Computacao Grafica - 2022/23
	................................................ JHenriques / APerrotta
	Trabalho 4 - projecao / viewport
======================================================================================= */


//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
// A implementar pelos alunos
//     - Alternar tipo projecao: ortogonal/perpectiva
//     - Efeito zoom com projecao perspectiva
//     - Uso de dois viewports para visualizar a cena
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#define PI		 3.14159

//--------------------------------- Definir cores
#define BLUE     0.0, 0.0, 1.0, 1.0
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define ORANGE   1.0, 0.5, 0.1, 1.0
#define CYAN     0.0, 1.0, 1.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.3, 0.3, 0.3, 1.0


//------------------------------------------------------------ Sistema Coordenadas 
GLint		wScreen = 850, hScreen = 700;		//.. janela
GLfloat		SIZE = 10.0;	//.. Mundo  SIZE=coordenadas x=y=z


//========================================================= Objeto Mesa
float	  centrox = 0;
float	  rotacao = 0;
GLint     sempreRodar = 1;
GLfloat   altura = 0;
GLfloat   incALT = 0.03;



//============================================================= Observador
GLfloat  rVisao = 4, aVisao = -0.5 * PI, incVisao = 1;
GLfloat  obsP[] = { rVisao * cos(aVisao), 1.0, rVisao * sin(aVisao) };
GLfloat  obsT[] = { obsP[0] + rVisao * cos(aVisao), obsP[1], obsP[2] - rVisao * sin(aVisao) };
float	 anguloZ = 65;

//============================================================= Projecao  ?????

//<><><><> ------------- comutar entre o tipo de pro
int		 tipoProjeccao = 0;

//===========================================================Variaveis e constantes
GLint     msec = 10;


//================================================================================
//================================================ VERTEX ARAY
//------------------------------------------- coordenadas + normais + cores
GLfloat tam = 0.5;
static GLfloat vertices[] = {
	//…………………………………………………………………………………………………… x=tam (Esquerda)
		-tam,  -tam,  tam,	// 0 
		-tam,   tam,  tam,	// 1 
		-tam,   tam, -tam,	// 2 
		-tam,  -tam, -tam,	// 3 
		//…………………………………………………… Direita
   	     tam,  -tam,  tam,	// 4 
		 tam,   tam,  tam,	// 5 
		 tam,   tam, -tam,	// 6 
		 tam,  -tam, -tam,	// 7 
	 //……………………………………………………… (Cima
		 -tam,  tam,  tam,	// 8 
	     -tam,  tam, -tam,	// 9 
		  tam,  tam, -tam,	// 10 
		  tam,  tam,  tam,	// 11 
};
static GLfloat normais[] = {
	//…………………………………………………………………………………………………… x=tam (Esquerda)
	  -1.0,  0.0,  0.0,
	  -1.0,  0.0,  0.0,
	  -1.0,  0.0,  0.0,
	  -1.0,  0.0,  0.0,
	  //…………………………………………………………………………………………………… x=tam (Direita)
	  1.0,  0.0,  0.0,
	  1.0,  0.0,  0.0,
	  1.0,  0.0,  0.0,
	  1.0,  0.0,  0.0,
	  //…………………………………………………………………………………………………… y=tam (Cima)
	  0.0,  1.0,  0.0,
      0.0,  1.0,  0.0,
      0.0,  1.0,  0.0,
      0.0,  1.0,  0.0,
};
static GLfloat cores[] = {
	//…………………………………………………………………………………………………… x=tam (Esquerda) - Red
	  1.0,  0.0, 0.0,	// 0 
	  1.0,  0.0, 0.0,	// 1 
	  1.0,  1.0, 0.0,	// 2 
	  1.0,  1.0, 0.0,	// 3 
	  //…………………………………………………………………………………………………… y=tam (Cima) - Blue
	  1.0,  0.0, 1.0,	// 8 
	  1.0,  0.0, 1.0,	// 9 
	  0.0,  1.0, 1.0,	// 10 
	  0.0,  0.5, 1.0,	// 11 
		//…………………………………………………………………………………………………… x=2*tam (Direita) - Green
	  0.0,  1.0, 1.0,	// 4 
	  0.0,  1.0, 1.0,	// 5 
	  0.0,  1.0, 0.0,	// 6 
	  0.0,  1.0, 0.0	// 7 

};
static GLuint esquerda[] = { 0, 1,  2,  3 };
static GLuint  direita[] = { 4, 7,  6,  5 };
static GLuint     cima[] = { 8, 11, 10, 9 };


//================================================================================
//=========================================================================== INIT
void initialize(void)
{
	glClearColor(BLACK);		//………………………………………………………………………………Apagar
	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Profundidade
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Interpolacao de cores	

	//glEnable(GL_CULL_FACE);		//………………………………………………………………………………Faces visiveis
	//glCullFace(GL_BACK);		//………………………………………………………………………………Mostrar so as da frente

	glVertexPointer(3, GL_FLOAT, 0, vertices);	//………………………………………Posicoes
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);		//………………………………………Normais
	glEnableClientState(GL_NORMAL_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, cores);		//………………………………………Cores
	glEnableClientState(GL_COLOR_ARRAY);

}



void drawEixos()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo
	glColor4f(ORANGE);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.5 * SIZE, 0, 0);
	glEnd();
	glColor4f(GREEN);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0.5 * SIZE, 0);
	glEnd();
	glColor4f(BLUE);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 0.5 * SIZE);
	glEnd();
}

void drawObservador_Onde()
{
	glColor4f(GREEN);
	glPushMatrix();
	glTranslatef(obsP[0], obsP[1], obsP[2]);
	glutSolidCube(1.5);
	glPopMatrix();
}

void drawObservador_ParaOnde()
{
	glColor4f(YELLOW);
	glPushMatrix();
	glTranslatef(obsT[0], obsT[1], obsT[2]);
	glutSolidCube(1.3);
	glPopMatrix();
}


//======================================
void drawScene() {



	//==================================== MESAS
	if (sempreRodar) {
		rotacao = rotacao + 1;
		altura = altura + incALT;
		if (altura > 3) incALT = -incALT;
		if (altura < -1) incALT = -incALT;
	}
	glTranslatef(centrox, altura, 0.0);
	glRotatef(rotacao, 0.0, 1.0, 0.0);
	glScalef(2.5, 2.0, 2.5);
	glPushMatrix();
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
	glPopMatrix();

	glColor4f(YELLOW);
	glTranslatef(0, tam + 0.3, 0.0);
	glutWireTeapot(0.4);
}




//====================================================== 
//====================================================== 
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//=======================================================
	//<><><><><><><><><><><><><> Viewport 1 - MAPA   ???
	//=======================================================



	//================================================================= Viewport 2
	glViewport(0.30 * wScreen, 0.30 * hScreen, 0.70 * wScreen, 0.70 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, 1.0, 0.1, 9999);

	//=======================================================
	//<><><><><><><><><><><><><> Alternar tipo projecao  ???
	//=======================================================

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], obsT[0], obsT[1], obsT[2], 0, 1, 0);

	//…………………………………………………………………………………………………………………………………………………………Objectos
	drawEixos();
	if (tipoProjeccao == 1) drawObservador_Onde();
	drawScene();



	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Actualizacao
	glutSwapBuffers();
}


void Timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1);
}


//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y) {


	switch (key) {

	case 'R': case 'r':
		sempreRodar = !sempreRodar;
		glutPostRedisplay();
		break;
		
	case 'A':case 'a':
		centrox = centrox + 0.5;
		glutPostRedisplay();
		break;
	case 'S': case 's':
		centrox = centrox - 0.5;
		glutPostRedisplay();
		break;
		
	case 'e': case 'E':
		rotacao = rotacao + 2.5;
		glutPostRedisplay();
		break;
	case 'd': case 'D':
		rotacao = rotacao - 2.5;
		glutPostRedisplay();
		break;

	//=======================================================
	//<><><><><><><><><><><><><> Zoom  ???
	//=======================================================
	case 'z': 	case 'Z':
		glutPostRedisplay();
		break;
	case 'x':	case 'X':
		glutPostRedisplay();
		break;

	
	case 'p': 	case 'P':
	//<><><><><><><><><><><><><> tipo projecao
		tipoProjeccao = (tipoProjeccao + 1) % 2;
		glutPostRedisplay();
		break;

		//--------------------------- Escape
	case 27:
		exit(0);
		break;
	}

}



void teclasNotAscii(int key, int x, int y) {


	if (key == GLUT_KEY_UP) {
		obsP[0] = obsP[0] + incVisao * cos(aVisao);
		obsP[2] = obsP[2] - incVisao * sin(aVisao);
	}
	if (key == GLUT_KEY_DOWN) {
		obsP[0] = obsP[0] - incVisao * cos(aVisao);
		obsP[2] = obsP[2] + incVisao * sin(aVisao);
	}
	if (key == GLUT_KEY_LEFT)   aVisao = (aVisao + 0.1);
	if (key == GLUT_KEY_RIGHT) 	aVisao = (aVisao - 0.1);

	obsT[0] = obsP[0] + rVisao * cos(aVisao);
	obsT[2] = obsP[2] - rVisao * sin(aVisao);

	if (obsP[1] > SIZE)   obsP[1] = SIZE;
	if (obsP[1] < -SIZE)  obsP[1] = -SIZE;

	glutPostRedisplay();

}


//======================================================= MAIN
//======================================================= MAIN
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("{jh,avperrotta}@dei.uc.pt|  |Projeccao :'p'|  |Observador:'SETAS'|  |Mesa-'a/s' 'e/d' 'r'  ");

	initialize();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}



