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
#define BLUE 0.0, 0.0, 1.0
#define RED 1.0, 0.0, 0.0
#define YELLOW 1.0, 1.0, 0.0
#define GREEN 0.0, 1.0, 0.0
#define ORANGE 1.0, 0.5, 0.1
#define WHITE 1.0, 1.0, 1.0
#define BLACK 0.0, 0.0, 0.0
#define GRAY 0.92, 0.92, 0.92
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
GLuint texture[3];

//------------------------------------------------------------ Skybox
GLUquadricObj* esfera = gluNewQuadric();

GLint material = 0;

void initMaterials(int material);

/*
	0: Emerald
	1: Jade
	2: Obsidian
	3: Pearl
	4: Ruby
	5: Turquoise
	6: Brass
	7: Bronze
	8: Chrome
	9: Copper
	10: Gold
	11: Silver
	12: blackPlastic
	13: cyanPlastic
	14: greenPlastic
	15: redPlastic
	16: whitePlastic
	17: yellowPlastic

*/

//…………………………………………………………………………………………………………………………………………… Luz pontual no TETO (eixo Y)
GLint   ligaTeto = 1;		 //:::   'T'  
GLfloat intensidadeT = 1.0;  //:::   'I'  
GLint   luzR = 1;		 	 //:::   'R'  
GLint   luzG = 1;			 //:::   'G'  
GLint   luzB = 1;			 //:::   'B'  
GLfloat localPos[4] = { 0.0, 10.0, 0.0, 1.0 };
GLfloat localCorAmb[4] = { 0, 0, 0, 0.0 };
GLfloat localCorDif[4] = { luzR, luzG, luzB, 1.0 };
GLfloat localCorEsp[4] = { luzR, luzG, luzB, 1.0 };


bool 		Focos[] = { 1,1 };		//.. Dois Focos ligados ou desligados
GLfloat		aberturaFoco = 30.0;	//.. angulo inicial do foco
GLfloat		anguloINC = 3.0;		//.. incremento
GLfloat		anguloMIN = 3.0;		//.. minimo
GLfloat		anguloMAX = 70.0;		//.. maximo


GLfloat Pos1[] = { -tam, tam - .5,  tam+1};
GLfloat Pos2[] = { tam, tam - .5,  tam+1};   

GLfloat Pos1_1[] = { -tam, tam - .5,  tam+1, 1.0f };   // Foco 1
GLfloat Pos2_1[] = { tam, tam - .5,  tam+1, 1.0f };   // Foco 2 

///…………………………………………………………………………………………………………………………………………… AMBIENTE - fixa
GLint   Dia = 1;     //:::   'N'  
GLfloat intensidadeDia = 2.0;
GLfloat luzGlobalCorAmb[4] = { intensidadeDia, intensidadeDia,intensidadeDia, 1.0 };   // 


GLint	  dim = 64;   //numero divisoes da grelha


//=================================================================================
//====================================== INIT =====================================
//=================================================================================

void defineLuzes()
{
	GLfloat Foco_direccao[] = { 0, -0.5, -1, 0 };	//……… -Z
	GLfloat Foco1_cor[] = { 0, 1,  0, 1 };	//……… Cor da luz 1
	GLfloat Foco2_cor[] = { 1, 0,  0, 1 };	//……… Cor da luz 2
	GLfloat Foco_ak = 1.0;
	GLfloat Foco_al = 0.05f;
	GLfloat Foco_aq = 0.0f;
	GLfloat Foco_Expon = 2.0;		// Foco, SPOT_Exponent

	//……………………………………………………………………………………………………………………………Foco Esquerda
	glLightfv(GL_LIGHT2, GL_POSITION, Pos1_1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Foco1_cor);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, Foco_ak);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, Foco_al);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, Foco_aq);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, aberturaFoco);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, Foco_direccao);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, Foco_Expon);

	//……………………………………………………………………………………………………………………………Foco Direita
	glLightfv(GL_LIGHT1, GL_POSITION, Pos2_1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Foco2_cor);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, Foco_ak);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, Foco_al);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, Foco_aq);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, aberturaFoco);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Foco_direccao);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, Foco_Expon);

}

void initLights(void) {
//…………………………………………………………………………………………………………………………………………… Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);

	//…………………………………………………………………………………………………………………………………………… Teto
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
}

void initTexturas()
{
	//----------------------------------------- topo
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	imag.LoadBmpFile("solar.bmp");
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
		
	// Fundo
	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	imag.LoadBmpFile("road.bmp");
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
	glClearColor(BLACK, 1.0);

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

//……………………………………………………………………………………………………………………………………………  ILUMINCCAO
void iluminacao() {
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
	if (ligaTeto)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
}

void updateLuz() {
	localCorAmb[0] = luzR * intensidadeT;
	localCorAmb[1] = luzG * intensidadeT;
	localCorAmb[2] = luzB * intensidadeT;
	localCorDif[0] = luzR * intensidadeT;
	localCorDif[1] = luzG * intensidadeT;
	localCorDif[2] = luzB * intensidadeT;
	localCorEsp[0] = luzR * intensidadeT;
	localCorEsp[1] = luzG * intensidadeT;
	localCorEsp[2] = luzB * intensidadeT;;
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
}

//=================================================================================
//====================================== DRAW =====================================
//=================================================================================

void drawEixos()
{
	glColor3f(RED);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(10, 0, 0);
	glEnd();

	glColor3f(GREEN);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 10, 0);
	glEnd();

	glColor3f(BLUE);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 10);
	glEnd();
}

void drawEsfera() {

    glColor3f(WHITE);
	initMaterials(16); //whiteplaastic

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

	//initMaterials(12); //blackPlastic

	glColor3f(BLACK);
	gluCylinder(q, .3, .3, .4, 20, 20);

	//initMaterials(5); //turquoise
	glColor3f(BLUE);
	gluDisk(q, 0, .3, 20, 20);
	gluDeleteQuadric(q);

	
	//initMaterials(9); // copper
	glColor3f(ORANGE);
	glPushMatrix();
		glScalef(0.1,.5,0.05);
		drawCube();
	glPopMatrix();

	glColor3f(ORANGE);
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
	glColor4f(0.0, 1.0, 1.0, 0.6);
	initMaterials(5); //cyanPlastic

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

void luzes(void)
{
	int				i, j;
	float			med_dim = (float)dim / 2;

	
	//============================================Esferasverde e vermelha
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	//…………………………………………………………………………………Verde
	if (Focos[0]) {
		glEnable(GL_LIGHT2);
		glPushMatrix();
		glColor4f(GREEN, 1);
		glTranslatef(-tam/2, 0, tam/2 -.25);
		glTranslatef(Pos1[0], Pos1[1], Pos1[2]);
		glutSolidSphere(0.1f, 100, 100);
		glPopMatrix();
	}
	//…………………………………………………………………………………Vermelha
	if (Focos[1]) {
		glEnable(GL_LIGHT1);
		glPushMatrix();
		glColor4f(RED, 1);
		glTranslatef(tam/2, 0, tam/2 - .25);
		glTranslatef(Pos2[0], Pos2[1], Pos2[2]);
		glutSolidSphere(0.1f, 100, 100);
		glPopMatrix();
	}


	//============================================ Grelha de polionos (dim*dim)
	//============================================ O ponto minimo   e (0,0), o maximo (2,2)
	//============================================ A textura minimo e (0,0), o maximo (1,1)
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	//----------------------------------------------- Luzes
	defineLuzes();

	//----------------------------------------------- Quadro Material - branco
	initMaterials(16);
	float corAmb[] = { 0.7,0.7,0.7,1 };/* 
	glMaterialfv(GL_BACK, GL_AMBIENT, corAmb);
	glMaterialfv(GL_BACK, GL_DIFFUSE, corAmb);
	glMaterialfv(GL_BACK, GL_SPECULAR, corAmb); */

	//----------------------------------------------- Textura - caracol
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	//----------------------------------------------- Dsenha malha poligonos
	glPushMatrix();
	glScalef(5, 5, 5);
	glTranslatef(0,-(tam -.34),0);
	glRotatef(90,0,1,0);
	glRotatef(90,1,0,0);
	glTranslatef(-1.0, -1.0, 0);  // meio do poligono 

	glNormal3f(0, 0, 1);          //normal 

	glBegin(GL_QUADS);
	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++) {
			glTexCoord2f((float)j / dim, (float)i / dim);
			glVertex3d((float)j / med_dim, (float)i / med_dim, 0);

			glTexCoord2f((float)(j + 1) / dim, (float)i / dim);
			glVertex3d((float)(j + 1) / med_dim, (float)i / med_dim, 0);

			glTexCoord2f((float)(j + 1) / dim, (float)(i + 1) / dim);
			glVertex3d((float)(j + 1) / med_dim, (float)(i + 1) / med_dim, 0);

			glTexCoord2f((float)j / dim, (float)(i + 1) / dim);
			glVertex3d((float)j / med_dim, (float)(i + 1) / med_dim, 0);
		}
	glEnd();
	glPopMatrix();
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
	glColor3f(RED);
	initMaterials(material); //material

	glRotatef(rotacao, 0.0, 1.0, 0.0);
	glTranslatef(centrox, altura, 0.0);
	glScalef(2, 1, 3);
	drawCube();
	glPopMatrix();

	// ======== CABIN
	glPushMatrix();
	glColor3f(ORANGE);
	initMaterials(7); //bronze


	glRotatef(rotacao, 0.0, 1.0, 0.0);
	glTranslatef(centrox, altura + .75, 0);
	glScalef(1.5, .6, 2);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);

	glColor3f(WHITE);
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

	//glDisable(GL_LIGHTING);

	glViewport(0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, 3.0 * zC);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);

	defineLuzes();

	initLights();

	glEnable(GL_LIGHTING);

	updateLuz();

	iluminacao();
	initMaterials(16);
	luzes();

	drawEsfera();

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
	
	drawScene();

	glDisable(GL_LIGHTING);

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
	/* case 'R':
	case 'r':
		sempreRodar = !sempreRodar;
		glutPostRedisplay();
		break;
 */
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

	case '8':
		obsP[0] = rVisao * cos(aVisao);
		obsP[2] = rVisao * sin(aVisao);
		glutPostRedisplay();
		break;

	case '9':
		obsP[0] = 5;
		obsP[1] = 5;
		obsP[2] = 5;
		glutPostRedisplay();
		break;

	case 't':case 'T':
		ligaTeto = !ligaTeto;
		updateLuz();
		glutPostRedisplay();
		break;

	case 'm': case 'M':
		material = (material + 1) % 18;
		initMaterials(material);
		glutPostRedisplay();
		break;

	case 'i': case 'I':
		intensidadeT = intensidadeT + 0.4;
		if (intensidadeT > 2.1) intensidadeT = 0;
		updateLuz();
		glutPostRedisplay();
		break;

	case 'r':case 'R':
		luzR = (luzR + 1) % 2;
		updateLuz();
		glutPostRedisplay();
		break;

	case 'g':case 'G':
		luzG = (luzG + 1) % 2;
		updateLuz();
		glutPostRedisplay();
		break;

	case 'b':case 'B':
		luzB = (luzB + 1) % 2;
		updateLuz();
		glutPostRedisplay();
		break;

//--------------------------- Dia/noite
	case 'n':	case 'N':
		Dia = !Dia;
		if (Dia) {
			luzGlobalCorAmb[0] = 1.0;
			luzGlobalCorAmb[1] = 1.0;
			luzGlobalCorAmb[2] = 1.0;	
		}
		else {
			luzGlobalCorAmb[0] = 0.0;
			luzGlobalCorAmb[1] = 0.0;
			luzGlobalCorAmb[2] = 0.0;
		}
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);
		glutPostRedisplay();
		break;

	case '1':
		Focos[0] = !Focos[0];
		if (Focos[0] == 0)
			glDisable(GL_LIGHT2);
		else
			glEnable(GL_LIGHT2);
		glutPostRedisplay();
		break;
		
	case '2':
		Focos[1] = !Focos[1];
		if (Focos[1] == 0)
			glDisable(GL_LIGHT1);
		else
			glEnable(GL_LIGHT1);
		glutPostRedisplay();
		break;

	case '3':
		dim = 2 * dim;
		if (dim > 256) dim = 256;
		glutPostRedisplay();
		break;

	case '4':
		dim = 0.5 * dim;
		if (dim < 1) dim = 1;
		glutPostRedisplay();
		break;
		
	case '5':
		aberturaFoco = aberturaFoco + anguloINC;
		if (aberturaFoco > anguloMAX)
			aberturaFoco = anguloMAX;
		glutPostRedisplay();
		break;

	case '6':
		aberturaFoco = aberturaFoco - anguloINC;
		if (aberturaFoco < anguloMIN)
			aberturaFoco = anguloMIN;
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
