/* ===================================================================================
	Departamento Eng. Informatica - FCTUC
	Computacao Grafica - 2022/23
	................................................ JHenriques / APerrotta
	Trabalho 3 - Visualizacao
======================================================================================= */

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
// A implementar pelos alunos
//     - Vertex Array cores
//     - Desenhar as faces esquerda e direita da mesa
//     - Movimento do observador
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


//================================================================================
//------------------------------------------------------------ Sistema Coordenadas 
GLint		wScreen = 700, hScreen = 600;		//.. janela - pixeis
GLfloat		SIZE = 10.0;	//.. Mundo  SIZE=coordenadas x=y=z


//========================================================= Animacao objeto Mesa
float	  centrox = 0;
float	  rotacao = 0;
GLint     sempreRodar = 0;
GLfloat   altura = 0;
GLfloat   incALT = 0.1;
GLint     faceVisivel = 0;

//============================================================= Observador
GLfloat  rVisao = 8, aVisao = -0.5 * PI, incVisao = 1;
GLfloat  obsP[] = { rVisao * cos(aVisao), 2.0, rVisao * sin(aVisao) };
float	 anguloZ = 35;


//===========================================================Variaveis e constantes
GLint     msec = 50;


//==================================================================== VERTEX ARAY
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


//=======================================================
//  <><><><><><><>             VERTEX ARRAY CORES  ???
//=======================================================


//=======================================================
//  <><><><><><><>             FACE  DIREITA  ????
//  <><><><><><><>             FACE  ESQUERDA ????
//=======================================================
static GLuint     cima[] = { 8, 11, 10, 9 };


//================================================================================
//=========================================================================== INIT
void initialize(void)
{
	glClearColor(BLACK);		//………………………………………………………………………………Apagar
	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Profundidade
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Interpolacao de cores	

	glEnable(GL_CULL_FACE);		//………………………………………………………………………………Faces visiveis
	glCullFace(GL_BACK);		//………………………………………………………………………………Mostrar so as da frente

	glVertexPointer(3, GL_FLOAT, 0, vertices);	//………………………………………Posicoes
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);		//………………………………………Normais
	glEnableClientState(GL_NORMAL_ARRAY);
	//  <><><><><><><>         Vertex Array - CORES ?????
	//  <><><><><><><>         Vertex Array - CORES ?????
	
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


//======================================
void drawScene() {

	//==================================== Animanacao MESA
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
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
	glPopMatrix();

	glColor4f(YELLOW);
	glTranslatef(0, tam + 0.3, 0.0);
	glutWireTeapot(0.4);
}




//====================================================== 
//====================================================== 
void display(void) {

	//================================================================= APaga 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		
	//================================================================= Viewport 2
	glViewport(0,0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 85, (float)wScreen / hScreen, 0.1, 9999);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//======================================================
	//  <><><><><><><>         OBSERVADOR NAO ESTÁ FIXO ????
	// gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);
	//======================================================
	 gluLookAt(5,3,5, 0, 0, 0, 0, 1, 0);

	
	//…………………………………………………………………………………………………………………………………………………………Objectos
	drawEixos();

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

	//------------------------------ translacao
	case 'A': case 'a':
		centrox = centrox + 0.5;
		glutPostRedisplay();
		break;
	case 'S': case 's':
		centrox = centrox - 0.5;
		glutPostRedisplay();
		break;
	//------------------------------ rotacao	
	case 'E': case 'e':
		rotacao = rotacao + 2.5;
		glutPostRedisplay();
		break;
	case 'D': case 'd':
		rotacao = rotacao - 2.5;
		glutPostRedisplay();
		break;


	//------------------------------ Cull Face
	case 'C': case 'c':
		//======================================
		//  <><><><><><> Alternar Face Visivel  ???
		//  <><><><><><> Ativa / Inativa  
		//======================================
		glutPostRedisplay();
		break;
		
		
	case 27:
		exit(0);
		break;
	}

}



void teclasNotAscii(int key, int x, int y) {
	
	//=========================================================
	//  <><><><><><>  Movimento do observador  ???
	//=========================================================
	if (key == GLUT_KEY_UP);	// Movimento para cima
	if (key == GLUT_KEY_DOWN);  // Movimento para baixo
	if (key == GLUT_KEY_LEFT);  // Movimento para a esquerda
	if (key == GLUT_KEY_RIGHT); // Movimento para a direira
	
	glutPostRedisplay();
}


//======================================================= MAIN
//======================================================= MAIN
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("{jh,avperrotta}@dei.uc.pt  ------  |CullFace - 'c'| |Observador:'SETAS'|  |Mesa- ´r´, 'a/s' 'e/d'  ");

	initialize();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}



