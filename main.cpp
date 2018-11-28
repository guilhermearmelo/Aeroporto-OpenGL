//NOMES_DOS_INTEGRANTES:
//GUILHERME BASTOS GUIMARÃES       - 16.1.4069
//Guilherme Melo                   - 12.1.4457
//PEDRO GABRIEL EVANGELISTA TORRES - 14.2.4220
//Rafael Ferreira Vitor            - 14.1.4032


#include <GL/glut.h>

#include "BMPLoader.h"
//#include "BMPLoader.cpp"
#include <cmath>
#include <iostream>
#include "cstring"
using namespace std;

//VARIAVEIS GLOBAIS
int WindowHeight = 500;
int WindowWidth = 500;
GLUquadric *quad;
GLfloat angle = 0, fAspect;
GLfloat movx = 0, movy = 0, movz = 0;
GLfloat rotacaoLat = 0, rotacaoFront = 0;
int plane = 1;
float velocidade = 0;
int speed = 0;
GLuint objeto;
//Variaveis pro Linux

int tempoAtrito = 1000;
int tempoGravidade = 50;
int tempoEstabiliza = 50;
//Variaveis pro MAC
//*/
/*int tempoAtrito = 5000;
int tempoGravidade = 1000;
int tempoEstabiliza = 50;*/
//*


//VARIAVEIS MENU
int aviaoSelecionado = 10;
int cameraSelecionada = 13;




int camera_X = 0, camera_Y = 0, camera_Z = 0;

//CABEÇALHOS
void plantarArvores();
void DesenhaLateraisAmbiente(void);
void DesenhaCasa(float, float, float, float, float, float, int);
void criarArvore(float posX, float posY, float posZ, int tipo);
void gravidade(int);
void atrito(int);
void fisicas();
void aplicaFisicas();
void estabilizaAviao(int x);
void DefineIluminacao();
void GerenciaMouse(int, int, int, int);
void tecla(unsigned char, int, int);
void DesenhaBola(void);
void DesenhaAviao(void);
void DesenhaCenario(void);
void printtext(int, int, string);
void Simulator();
void init();
void iluminacao();
void reseta();







void gravidade(int x) {
	if (movy>0) {
		if (plane == 0 )
			movy -= 0.05;
	}
	else {
		movy = 0;
	}
	glutTimerFunc(tempoGravidade, gravidade, 10);
}

void atrito(int x) {

	if (movy == 0) {
		if (speed >= 10)
			speed -= 10;
		else
			speed = 0;
	}
	else {
		if (plane == 0 && speed >150)
			if (speed >= 10)
				speed -= 10;
			else
				speed = 0;

	}
	glutTimerFunc(tempoAtrito, atrito, 10);
}

void estabilizaAviao(int x) {
	if (movy>0) {
		if (plane == 0) {
			rotacaoLat>0 ? rotacaoLat -= 0.2 : rotacaoLat += 0.2;
			if (rotacaoFront>0) {
				rotacaoFront -= 0.2;
			}
			else {
				rotacaoFront = 0;
			}
		}
	}

	glutTimerFunc(tempoEstabiliza, estabilizaAviao, 10);
}

void fisicas() {
	glutTimerFunc(tempoGravidade, gravidade, 10);
	glutTimerFunc(tempoAtrito, atrito, 10);
	glutTimerFunc(tempoEstabiliza, estabilizaAviao, 10);
}

float porcentagem(float angle_aux, float angle_quadrante) {
	return (100 * (angle_aux - angle_quadrante)) / 9000;
}

void aplicaFisicas() {
	float movimentacao = (float)speed / 10000; // Linux
											   //float movimentacao = (float)speed / 4000; // Mac

	float porcentoDirecao = 0;


	if ((angle<0 ? 360 + angle : angle)<90) {
		if (angle != 0) {
			porcentoDirecao = porcentagem(angle>0 ? angle : 360 + angle, 0);
			movz -= movimentacao * (1.0 - porcentoDirecao);
			movx -= movimentacao * porcentoDirecao;
		}
		else
			movz -= movimentacao;
	}
	else if ((angle<0 ? 360 + angle : angle)<180) {
		if (angle != 90) {
			porcentoDirecao = porcentagem(angle>0 ? angle : 360 + angle, 90);
			movx -= movimentacao * (1.0 - porcentoDirecao);
			movz += movimentacao * porcentoDirecao;
		}
		else
			movx -= movimentacao;
	}
	else if ((angle<0 ? 360 + angle : angle)<270) {
		if (angle != 180) {
			porcentoDirecao = porcentagem(angle>0 ? angle : 360 + angle, 180);
			movz += movimentacao * (1.0 - porcentoDirecao);
			movx += movimentacao * porcentoDirecao;
		}
		else
			movz += movimentacao;
	}
	else if ((angle<0 ? 360 + angle : angle)<360) {
		if (angle != 270) {
			porcentoDirecao = porcentagem(angle>0 ? angle : 360 + angle, 270);
			movx += movimentacao * (1.0 - porcentoDirecao);
			movz -= movimentacao * porcentoDirecao;
		}
		else
			movx += movimentacao;
	}
	else if (angle == 360) {
		movz -= movimentacao;
	}
}

void reseta() {
	movx = 0;
	movy = 0;
	movz = 0;
	speed = 0;
	angle = 0;
	rotacaoLat = 0, rotacaoFront = 0;
	plane = 1;
	cameraSelecionada = 13;
}

void DefineIluminacao(void) {
	GLfloat luzAmbiente[4] = { 0.2,0.2,0.2,1.0 };
	GLfloat luzDifusa[4] = { 0.7,0.7,0.7,1.0 };	   // "cor"
	GLfloat luzEspecular[4] = { 1.0, 1.0, 1.0, 1.0 };// "brilho"
	GLfloat posicaoLuz[4] = { 0.0, 50.0, 50.0, 1.0 };

	// Capacidade de brilho do material
	GLfloat especularidade[4] = { 1.0,1.0,1.0,1.0 };
	GLint especMaterial = 60;

	// Define a refletância do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);
}

void GerenciaMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (angle >= 5) {
			angle -= 5;
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (angle <= 130) {
			angle += 5;
		}
	}
	glutPostRedisplay();
}

void tecla(unsigned char tecla, int x, int y) {
	switch (tecla) {
	case 's':
		movz += 0.1;
		break;
	case 'w':
		//speed+=20;

		if (plane == 1) {
			if (speed<70)
				speed++;
			//movz-=velocidade;
		}
		else {

			if (speed<400)
				speed++;

			//movz-=velocidade;
		}
		if (speed>200) {
			movy += 0.2;

		}
		if (rotacaoFront < 25.0 && speed>150 && plane == 0) {
			rotacaoFront += 0.2;
		}

		/*if(movz <= -5 && plane == 0)
		movy += 0.1;*/
		break;
	case 'v':
		movy += 0.1;
		break;
	case 'b':
		movy -= 0.1;
		break;
	case 'a': // alçar voo
		if (angle >= 360)
			angle = 0;
		angle += 0.8;
		if (rotacaoLat < 30.0 && movy>0.0) {
			rotacaoLat += 1;
			movx -= 0.1;
		}

		break;
	case 'd': // alçar voo
		if (angle <= -360)
			angle = 0;
		angle -= 0.8;
		if (rotacaoLat > -30.0 && movy>0.0) {
			rotacaoLat -= 1;
			movx += 0.1;
		}

		break;
	case 'q':
		if (plane == 0)
			plane = 1;
		else if (plane == 1)
			plane = 0;
		break;
	case 'F':
		exit(0);
		break;
	case 'f':
		exit(0);
		break;
	case 'W':
		camera_Z -= 1;
		break;
	case 'A':
		camera_X -= 1;
		break;
	case 'D':
		camera_X += 1;
		break;
	case 'S':
		camera_Z += 1;
		break;
	}
}

void DesenhaBola(void) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(0.0f, -1.0f, -7.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 2);

	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	//glRotatef(-100, 0.0f, 1.0f, 0.0f);
	glTranslatef(movx, movy, movz);
	// Tamanho da esfera
	glScalef(0.2, 0.2, 0.2);

	gluQuadricTexture(quad, 2);
	//position
	glTranslatef(0, 0, 0);
	gluSphere(quad, 2, 20, 20);

	glPopMatrix();
}

void DesenhaAviao(void) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	DefineIluminacao();
	glPushMatrix();
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	switch (aviaoSelecionado) {
	case 10:
		glBindTexture(GL_TEXTURE_2D, 5);
		break;
	case 11:
		glBindTexture(GL_TEXTURE_2D, 16);
		break;
	case 12:
		glBindTexture(GL_TEXTURE_2D, 17);
		break;

	}

	//glColor3f(0.0f, 0.0f, 1.0f);

	glScalef(0.1, 0.1, 0.1);
	//glTranslatef(movx, movy, movz);
	if (cameraSelecionada == 13) {
		glTranslatef(0.0, -9.0, -50.0);

		//cameraSelecionada=14;
	}
	else {
		glTranslatef(0.0, -9.0, -20.0);
		//cameraSelecionada=13;
	}


	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glRotatef(rotacaoLat, 0.0f, 0.0f, 1.0f);
	glRotatef(rotacaoFront, 1.0f, 0.0f, 0.0f);

	//cone referente ao corpo do avião
	glPushMatrix();
	glutSolidCone(1.0, 9.0, 20, 4);
	glPopMatrix();

	//cone referente ao bico do avião
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	glutSolidCone(1, 3, 5, 20);
	glPopMatrix();

	//cone referente a uma asa

	glPushMatrix();
	glRotatef(70, 0, 1, 0);
	glutSolidCone(1, 7, 50, 20);
	glPopMatrix();

	//cone referente a outra asa
	glPushMatrix();
	glRotatef(-70, 0, 1, 0);
	glutSolidCone(1, 7, 50, 20);
	glPopMatrix();

	//cone referente a parte da cauda de um lado
	glPushMatrix();
	glTranslatef(0, 0, 7);
	glRotatef(-70, 0, 1, 0);
	glutSolidCone(0.3, 3, 60, 1);
	glPopMatrix();

	//cone referente a parte da cauda do outro lado
	glPushMatrix();
	glTranslatef(0, 0, 7);
	glRotatef(70, 0, 1, 0);
	glutSolidCone(0.3, 3, 60, 1);
	glPopMatrix();

	//cone referente a parte de cima da cauda
	glPushMatrix();
	glTranslatef(0, 0, 7);
	glRotatef(-70, 1, 0, 0);
	glutSolidCone(0.3, 2, 60, 1);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glDisable(GL_TEXTURE_GEN_T);
	glPopMatrix();
}

void DesenhaCasa(float scalex, float scaley, float scalez, float translatex, float translatey, float translatez, int texture) {
	glTranslatef(translatex, translatey, translatez);
	glScalef(scalex, scaley, scalez);

	// Frente
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(5, -5, -5);
	glTexCoord2f(1, 0); glVertex3f(5, 5, -5);
	glTexCoord2f(1, 1); glVertex3f(-5, 5, -5);
	glTexCoord2f(0, 1); glVertex3f(-5, -5, -5);
	glEnd();

	// TRASEIRA
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(5, -5, 5);
	glTexCoord2f(1, 0); glVertex3f(5, 5, 5);
	glTexCoord2f(1, 1); glVertex3f(-5, 5, 5);
	glTexCoord2f(0, 1); glVertex3f(-5, -5, 5);
	glEnd();

	// DIREITA
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(5, -5, -5);
	glTexCoord2f(1, 0); glVertex3f(5, 5, -5);
	glTexCoord2f(1, 1); glVertex3f(5, 5, 5);
	glTexCoord2f(0, 1); glVertex3f(5, -5, 5);
	glEnd();

	// ESQUERDA
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(-5, -5, 5);
	glTexCoord2f(1, 0); glVertex3f(-5, 5, 5);
	glTexCoord2f(1, 1); glVertex3f(-5, 5, -5);
	glTexCoord2f(0, 1); glVertex3f(-5, -5, -5);
	glEnd();

	// TOPO
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(5, 5, 5);
	glTexCoord2f(1, 0); glVertex3f(5, 5, -5);
	glTexCoord2f(1, 1); glVertex3f(-5, 5, -5);
	glTexCoord2f(0, 1); glVertex3f(-5, 5, 5);
	glEnd();
}

void DesenhaLateraisAmbiente(void) {
	//Textura Topo
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 12);

	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(-2000.0, 1000.0, 1000.0);
	glTexCoord2f(1, 0); glVertex3f(-2000.0, 1000.0, -3000.0);
	glTexCoord2f(1, 1); glVertex3f(2000.0, 1000.0, -3000.0);
	glTexCoord2f(0, 1); glVertex3f(2000.0, 1000.0, 1000.0);
	glEnd();

	//Textura Lado Esquerdo
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 9);

	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(-2000.0, -9.0, 1000.0);
	glTexCoord2f(0, 1); glVertex3f(-2000.0, 1000.0, 1000.0);
	glTexCoord2f(1, 1); glVertex3f(-2000.0, 1000.0, -3000.0);
	glTexCoord2f(1, 0); glVertex3f(-2000.0, -9.0, -3000.0);
	glEnd();

	//Textura Lado Direito
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 10);

	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(2000.0, -9.0, -3000.0);
	glTexCoord2f(0, 1); glVertex3f(2000.0, 1000.0, -3000.0);
	glTexCoord2f(1, 1); glVertex3f(2000.0, 1000.0, 1000.0);
	glTexCoord2f(1, 0); glVertex3f(2000.0, -9.0, 1000.0);
	glEnd();

	//Textura Frente
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 7);

	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(-2000.0, -9.0, -3000.0);
	glTexCoord2f(0, 1); glVertex3f(-2000.0, 1000.0, -3000.0);
	glTexCoord2f(1, 1); glVertex3f(2000.0, 1000.0, -3000.0);
	glTexCoord2f(1, 0); glVertex3f(2000.0, -9.0, -3000.0);
	glEnd();

	//Textura Atrás
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 11);

	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(-2000.0, -9.0, 1000.0);
	glTexCoord2f(0, 1); glVertex3f(-2000.0, 1000.0, 1000.0);
	glTexCoord2f(1, 1); glVertex3f(2000.0, 1000.0, 1000.0);
	glTexCoord2f(1, 0); glVertex3f(2000.0, -9.0, 1000.0);
	glEnd();
}

void plantarArvores() {
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glPopMatrix();
	glLoadIdentity();
	//glEnable(GL_TEXTURE_GEN_S);
	//glEnable(GL_TEXTURE_GEN_T);
	//glBindTexture(GL_TEXTURE_2D, 3);

	for (int eixoz = -300; eixoz <= -200; eixoz += 20) {
		for (int eixox = 20; eixox <= 60; eixox += 5) {
			int tArvore = (eixox + (-1 * eixoz)) % 3 + 1;
			criarArvore(eixox, -9, eixoz, tArvore);
		}
	}

	for (int eixoz = -480; eixoz <= -380; eixoz += 20) {
		for (int eixox = 20; eixox <= 60; eixox += 5) {
			int tArvore = (eixox + (-1 * eixoz)) % 3 + 1;
			criarArvore(eixox, -9, eixoz, tArvore);
		}
	}

	for (int eixoz = -800; eixoz <= -540; eixoz += 20) {
		for (int eixox = 20; eixox <= 60; eixox += 5) {
			int tArvore = (eixox + (-1 * eixoz)) % 3 + 1;
			criarArvore(eixox, -9, eixoz, tArvore);
		}
	}
	//glClear(GL_ACCUM_BUFFER_BIT);
	//glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	//glDisable(GL_TEXTURE_GEN_T);
	//glPopMatrix();
}

void criarArvore(float posX, float posY, float posZ, int tipo) {
	//glMatrixMode(GL_MODELVIEW);
	//glEnable(GL_TEXTURE_2D);
	//glClearDepth(1.0f);	// Depth Buffer Setup
	//glEnable(GL_DEPTH_TEST);	// Enables Depth Testing
	//glDepthFunc(GL_LEQUAL);	// The Type Of Depth Testing To Do

	//glPushMatrix();
	float tronco = 1;
	float alttronco = 2;
	//glTranslatef(0.0,0.0,-10.0);

	// base baixo
	//glColor3f (0.8f ,  0.6f ,  0.3f ) ;
	glBindTexture(GL_TEXTURE_2D, 14);
	glBegin(GL_POLYGON);
	glVertex3i(posX + 0, posY + 0, posZ + 0);
	glVertex3i(posX + tronco, posY + 0, posZ + 0);
	glVertex3i(posX + tronco, posY + 0, posZ + tronco);
	glVertex3i(posX + 0, posY + 0, posZ + tronco);
	glEnd();

	// base cima
	//glColor3f (0.8f ,  0.6f ,  0.3f ) ;
	glBindTexture(GL_TEXTURE_2D, 14);
	glBegin(GL_POLYGON);
	glVertex3i(posX + 0, posY + alttronco, posZ + 0);
	glVertex3i(posX + tronco, posY + alttronco, posZ + 0);
	glVertex3i(posX + tronco, posY + alttronco, posZ + tronco);
	glVertex3i(posX + 0, posY + alttronco, posZ + tronco);
	glEnd();

	// lado frente
	//glColor3f (0.8f ,  0.6f ,  0.3f ) ;
	glBindTexture(GL_TEXTURE_2D, 14);
	glBegin(GL_POLYGON);
	glVertex3i(posX + 0, posY + 0, posZ + tronco);
	glVertex3i(posX + tronco, posY + 0, posZ + tronco);
	glVertex3i(posX + tronco, posY + alttronco, posZ + tronco);
	glVertex3i(posX + 0, posY + alttronco, posZ + tronco);
	glEnd();

	// lado tras
	//glColor3f (0.8f ,  0.6f ,  0.3f ) ;
	glBindTexture(GL_TEXTURE_2D, 14);
	glBegin(GL_POLYGON);
	glVertex3i(posX + 0, posY + 0, posZ + 0);
	glVertex3i(posX + tronco, posY + 0, posZ + 0);
	glVertex3i(posX + tronco, posY + alttronco, posZ + 0);
	glVertex3i(posX + 0, posY + alttronco, posZ + 0);
	glEnd();

	// lado esquerda
	//glColor3f (0.8f ,  0.6f ,  0.3f ) ;
	glBindTexture(GL_TEXTURE_2D, 14);
	glBegin(GL_POLYGON);
	glVertex3i(posX + 0, posY + 0, posZ + 0);
	glVertex3i(posX + 0, posY + 0, posZ + tronco);
	glVertex3i(posX + 0, posY + alttronco, posZ + tronco);
	glVertex3i(posX + 0, posY + alttronco, posZ + 0);
	glEnd();

	// lado direita
	//glColor3f (0.8f ,  0.6f ,  0.3f ) ;
	glBindTexture(GL_TEXTURE_2D, 14);
	glBegin(GL_POLYGON);
	glVertex3i(posX + tronco, posY + 0, posZ + 0);
	glVertex3i(posX + tronco, posY + 0, posZ + tronco);
	glVertex3i(posX + tronco, posY + alttronco, posZ + tronco);
	glVertex3i(posX + tronco, posY + alttronco, posZ + 0);
	glEnd();

	/////////////////////////////
	// copa da arvore ( cone )
	glPushMatrix();
	//glColor3f (0.0f ,  0.6f ,  0.3f ) ;
	glBindTexture(GL_TEXTURE_2D, 15);
	glTranslatef(posX + (tronco / 2), alttronco - 9, posZ + (tronco / 2));
	glRotatef(-90, 1, 0, 0);

	switch (tipo) {
	case 1:
		glutSolidCone(alttronco * 2 / 3, alttronco * 2, 80, 8); // arvore 1
		break;
	case 2:
		glutSolidCone(alttronco * 4 / 3, alttronco, 80, 8); // arvore 2
		break;
	case 3:
		glutSolidCone(alttronco, alttronco * 3, 80, 8); // arvore 3
		break;
	}
	glPopMatrix();
}

void DesenhaCenario(void) {
	//glMatrixMode(GL_MODELVIEW);

	//glPushMatrix();
	//glTranslatef(0.0f, 0.0f, -7.0f);
	glEnable(GL_TEXTURE_2D);
	//lBindTexture(GL_TEXTURE_2D, 1);

	glScalef(0.2, 0.2, 0.2);

	DesenhaLateraisAmbiente();

	// MAR
	glBindTexture(GL_TEXTURE_2D, 3);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(-2000.0, -9.0, 1000.0);
	glTexCoord2f(1, 0); glVertex3f(-2000.0, -9.0, -3000.0);
	glTexCoord2f(1, 1); glVertex3f(2000.0, -9.0, -3000.0);
	glTexCoord2f(0, 1); glVertex3f(2000.0, -9.0, 1000.0);
	glEnd();

	// PISTA DE DECOLAGEM
	glBindTexture(GL_TEXTURE_2D, 1);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(-20.0, -8.9, 100.0);
	glTexCoord2f(1, 0); glVertex3f(-20.0, -8.9, -1000.0);
	glTexCoord2f(1, 1); glVertex3f(20.0, -8.9, -1000.0);
	glTexCoord2f(0, 1); glVertex3f(20.0, -8.9, 100.0);
	glEnd();

	// GRAMA DA PISTA
	glBindTexture(GL_TEXTURE_2D, 8);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(20, -8.8, -800.0);
	glTexCoord2f(1, 0); glVertex3f(60.0, -8.8, -800.0);
	glTexCoord2f(1, 1); glVertex3f(60.0, -8.8, -200.0);
	glTexCoord2f(0, 1); glVertex3f(20.0, -8.8, -200.0);
	glEnd();

	// PISTA DE ACESSO 1
	glBindTexture(GL_TEXTURE_2D, 1);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(20, -8.7, -540.0);
	glTexCoord2f(1, 0); glVertex3f(60.0, -8.7, -540.0);
	glTexCoord2f(1, 1); glVertex3f(60.0, -8.7, -480.0);
	glTexCoord2f(0, 1); glVertex3f(20.0, -8.7, -480.0);
	glEnd();

	// PISTA DE ACESSO 2
	glBindTexture(GL_TEXTURE_2D, 1);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(20, -8.7, -380.0);
	glTexCoord2f(1, 0); glVertex3f(60.0, -8.7, -380.0);
	glTexCoord2f(1, 1); glVertex3f(60.0, -8.7, -300.0);
	glTexCoord2f(0, 1); glVertex3f(20.0, -8.7, -300.0);
	glEnd();

	//Estacionamento de aviao
	glBindTexture(GL_TEXTURE_2D, 1);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3f(60, -8.7, -200.0);
	glTexCoord2f(1, 0); glVertex3f(260.0, -8.7, -200.0);
	glTexCoord2f(1, 1); glVertex3f(260.0, -8.7, -800.0);
	glTexCoord2f(0, 1); glVertex3f(60.0, -8.7, -800.0);
	glEnd();

	//Desenhando Casas do Aeroporto
	//void DesenhaCasa(float scalex, float scaley, float scalez,
	//float translatex, float translatey, float translatez, int texture)

	DesenhaCasa(4, 4, 30, 180, 0, -600, 13);

	criarArvore(0,0,0,1);
	plantarArvores();

	//glPopMatrix();
}

void printtext(int x, int y, string String) {//(x,y) is from the bottom left of the window
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, WindowHeight, 0, WindowWidth, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glRasterPos2i(x, y);
	for (int i = 0; i<String.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, String[i]);
	}
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Simulator(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	if (cameraSelecionada == 13) {
		gluLookAt(movx, movy, movz + 5, movx, movy, movz, 0, 1, 0);
	}
	else {
		gluLookAt(movx, movy - 1, movz + 5, movx, movy - 1, movz, 0, 1, 0);
	}
	DesenhaCenario();


	DesenhaAviao();

	char stringFinal[50] = { "Velocidade: " };


	char string[64];
	char  buf[BUFSIZ];
	sprintf(buf, "%d", speed);

	strcat(stringFinal, buf);
	printtext(10, WindowHeight - 20, stringFinal);
	printtext(10, WindowHeight - 40, plane == 0 ? "Modo: Voo" : "Modo: Taxiar / Planar");

	aplicaFisicas();
	glutSwapBuffers();

}

void CarregarTexturas() {
	// Textura da pista
	BMPClass bmp;
	BMPLoad("texture/pista.bmp", bmp);
	glBindTexture(GL_TEXTURE_2D, 1);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.width, bmp.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.bytes);


	// Textura da bola
	BMPClass bmp2;
	BMPLoad("texture/map.bmp", bmp2);
	glBindTexture(GL_TEXTURE_2D, 2);  // numero 2 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp2.width, bmp2.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp2.bytes);

	// Textura do mar
	BMPClass bmp3;
	BMPLoad("texture/mar.bmp", bmp3);
	glBindTexture(GL_TEXTURE_2D, 3);  // numero 3 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp3.width, bmp3.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp3.bytes);

	//Textura 1 dos predios
	BMPClass bmp4;
	BMPLoad("texture/brickwall.bmp", bmp4);
	glBindTexture(GL_TEXTURE_2D, 4);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp4.width, bmp4.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp4.bytes);

	//Textura do aviao
	BMPClass bmp5;
	BMPLoad("texture/camouflage.bmp", bmp5);
	glBindTexture(GL_TEXTURE_2D, 5);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp5.width, bmp5.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp5.bytes);

	//Textura 2 dos predios
	BMPClass bmp6;
	BMPLoad("texture/wall.bmp", bmp6);
	glBindTexture(GL_TEXTURE_2D, 6);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp6.width, bmp6.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp6.bytes);

	//Textura da paisagem frente
	BMPClass bmp7;
	BMPLoad("texture/sky1.bmp", bmp7);
	glBindTexture(GL_TEXTURE_2D, 7);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp7.width, bmp7.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp7.bytes);

	//Textura da grama
	BMPClass bmp8;
	BMPLoad("texture/grass.bmp", bmp8);
	glBindTexture(GL_TEXTURE_2D, 8);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp8.width, bmp8.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp8.bytes);


	//Textura da paisagem lado esquerdo
	BMPClass bmp9;
	BMPLoad("texture/sky5.bmp", bmp9);
	glBindTexture(GL_TEXTURE_2D, 9);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp9.width, bmp9.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp9.bytes);

	//Textura da paisagem lado direito
	BMPClass bmp10;
	BMPLoad("texture/sky2.bmp", bmp10);
	glBindTexture(GL_TEXTURE_2D, 10);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp10.width, bmp10.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp10.bytes);

	//Textura da paisagem atrás
	BMPClass bmp11;
	BMPLoad("texture/sky4.bmp", bmp11);
	glBindTexture(GL_TEXTURE_2D, 11);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp11.width, bmp11.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp11.bytes);

	//Textura da paisagem céu
	BMPClass bmp12;
	BMPLoad("texture/sky3.bmp", bmp12);
	glBindTexture(GL_TEXTURE_2D, 12);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp12.width, bmp12.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp12.bytes);

	//Textura dos Predios
	BMPClass bmp13;
	BMPLoad("texture/glassbuilding.bmp", bmp13);
	glBindTexture(GL_TEXTURE_2D, 13);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp13.width, bmp13.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp13.bytes);

	//Textura do Tronco da Arvore
	BMPClass bmp14;
	BMPLoad("texture/tronco.bmp", bmp14);
	glBindTexture(GL_TEXTURE_2D, 14);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp14.width, bmp14.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp14.bytes);

	//Textura do Tronco da Arvore
	BMPClass bmp15;
	BMPLoad("texture/folha.bmp", bmp15);
	glBindTexture(GL_TEXTURE_2D, 15);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp15.width, bmp15.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp15.bytes);


	//Textura do aviao branco
	BMPClass bmp16;
	BMPLoad("texture/branca.bmp", bmp16);
	glBindTexture(GL_TEXTURE_2D, 16);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp16.width, bmp16.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp16.bytes);

	//Textura do aviao metal
	BMPClass bmp17;
	BMPLoad("texture/metal.bmp", bmp17);
	glBindTexture(GL_TEXTURE_2D, 17);  // numero 1 escolhido para identificar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST OU GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp17.width, bmp17.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp17.bytes);

}

void iluminacao() {
	GLfloat luzAmbiente[4] = { 0.2,0.2,0.2,1.0 };
	GLfloat luzDifusa[4] = { 0.7,0.7,0.7,1.0 };    // "cor"
	GLfloat luzEspecular[4] = { 1.0, 1.0, 1.0, 1.0 };// "brilho"
	GLfloat posicaoLuz[4] = { 100.0, 100.0, -1000.0, 1.0 };

	// Capacidade de brilho do material
	GLfloat especularidade[4] = { 1.0,1.0,1.0,1.0 };
	GLint especMaterial = 60;

	// Especifica que a cor de fundo da janela será preta
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Define a refletância do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
}

void init(void) {
	iluminacao();

	fisicas();
	/*  define a cor de background da janela       */
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glViewport(0, 0, WindowHeight, WindowWidth);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(35, 640.0 / 640.0, 1.0, 1000.0);



	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	//quad = gluNewQuadric();

	CarregarTexturas();
}

void selecionaaviao(int op) {
	aviaoSelecionado = op;
	reseta();
}
void selecionacamera(int op) {
	cameraSelecionada = op;


}

void menu(int op) {
	switch (op) {
	case 9:
		exit(0);
		break;
	}
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WindowHeight, WindowWidth);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Aeroporto CG");
	init();



	int sub1 = glutCreateMenu(selecionaaviao);
	glutAddMenuEntry("Aviao 1", 10);
	glutAddMenuEntry("Aviao 2", 11);
	glutAddMenuEntry("Aviao 3", 12);


	int sub2 = glutCreateMenu(selecionacamera);
	glutAddMenuEntry("Visao 1", 13);
	glutAddMenuEntry("Visao 2", 14);

	glutCreateMenu(menu);
	glutAddSubMenu("Selecionar Aviao", sub1);
	glutAddSubMenu("Selecionar Camera", sub2);
	glutAddMenuEntry("Sair", 9);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//loadObj("objetos/piper.obj");

	glutDisplayFunc(Simulator);
	glutIdleFunc(Simulator);

	glutKeyboardFunc(tecla);
	glutMouseFunc(GerenciaMouse);

	glutMainLoop();
	return 0;
}
