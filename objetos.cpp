#include <GL/glut.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>

#include "objetos.h"

void criarArvore(float posX, float posY, float posZ, int tipo){

    glColor3f (0.8f ,  0.6f ,  0.3f ) ;

    float tronco = 1;
    float alttronco = 2;

    // base baixo
    glBegin(GL_POLYGON);
    glVertex3i(posX+0,posY+0,posZ+0);
    glVertex3i(posX+tronco,posY+0,posZ+0);
    glVertex3i(posX+tronco,posY+0,posZ+tronco);
    glVertex3i(posX+0,posY+0,posZ+tronco);
    glEnd();

    // base cima
    glBegin(GL_POLYGON);
    glVertex3i(posX+0,posY+alttronco,posZ+0);
    glVertex3i(posX+tronco,posY+alttronco,posZ+0);
    glVertex3i(posX+tronco,posY+alttronco,posZ+tronco);
    glVertex3i(posX+0,posY+alttronco,posZ+tronco);
    glEnd();

    // lado frente
    glBegin(GL_POLYGON);
    glVertex3i(posX+0,posY+0,posZ+tronco);
    glVertex3i(posX+tronco,posY+0,posZ+tronco);
    glVertex3i(posX+tronco,posY+alttronco,posZ+tronco);
    glVertex3i(posX+0,posY+alttronco,posZ+tronco);
    glEnd();

    // lado tras
    glBegin(GL_POLYGON);
    glVertex3i(posX+0,posY+0,posZ+0);
    glVertex3i(posX+tronco,posY+0,posZ+0);
    glVertex3i(posX+tronco,posY+alttronco,posZ+0);
    glVertex3i(posX+0,posY+alttronco,posZ+0);
    glEnd();

    // lado esquerda
    glBegin(GL_POLYGON);
    glVertex3i(posX+0,posY+0,posZ+0);
    glVertex3i(posX+0,posY+0,posZ+tronco);
    glVertex3i(posX+0,posY+alttronco,posZ+tronco);
    glVertex3i(posX+0,posY+alttronco,posZ+0);
    glEnd();

    // lado direita
    glBegin(GL_POLYGON);
    glVertex3i(posX+tronco,posY+0,posZ+0);
    glVertex3i(posX+tronco,posY+0,posZ+tronco);
    glVertex3i(posX+tronco,posY+alttronco,posZ+tronco);
    glVertex3i(posX+tronco,posY+alttronco,posZ+0);
    glEnd();

    /////////////////////////////
    // copa da arvore ( cone )
    glColor3f (0.0f ,  0.6f ,  0.3f ) ;

    glPushMatrix();
	glTranslatef(posX+(tronco/2), alttronco-9, posZ+(tronco/2));
  	glRotatef(-90,1,0,0);

  	switch(tipo){
    case 1:
        glutSolidCone(alttronco*2/3,alttronco*2,80,8); // arvore 1
        break;
    case 2:
        glutSolidCone(alttronco*4/3,alttronco,80,8); // arvore 2
        break;
    case 3:
        glutSolidCone(alttronco,alttronco*3,80,8); // arvore 3
        break;
  	}

    glPopMatrix();

    // Executa os comandos OpenGL
	//glutSwapBuffers();
}

void criarPoste(float posX, float posY, float posZ){
    glPushMatrix();
    {
        GLfloat ambi[] = {0.1, 0.1, 0.1, 1.0};
		GLfloat diff[] = {0.3, 0.3, 0.3, 1.0};
		GLfloat spec[] = {0.2, 0.2, 0.2, 1.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambi);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);


		glTranslatef(posX, posY, posZ);


		glRotatef(270, 1.0, 0.0, 0.0);

        glPushMatrix();

            GLUquadricObj *quadratic;
            quadratic = gluNewQuadric();
            gluCylinder(quadratic,0.25,0.15,6.0,32,32);

        glPopMatrix();

        glPushMatrix();
            glTranslatef(0, 0, 0);
            GLUquadricObj  *sphere;
            sphere = gluNewQuadric();
			gluQuadricDrawStyle(sphere, GLU_FILL);
			gluQuadricTexture(sphere, true);
			gluQuadricNormals(sphere, GLU_SMOOTH);
			gluSphere(sphere, 0.15, 20, 20);
            gluDeleteQuadric(sphere);
        glPopMatrix();

        glPushMatrix();
           	glTranslatef(0,0,4.5);
         	glRotatef(50, 0, 1, 0);
            GLUquadricObj *quadratic2;
            quadratic2 = gluNewQuadric();
            gluCylinder(quadratic2,0.15,0.1,2.0,32,32);

        glPopMatrix();

        glPushMatrix();
           	glTranslatef(1.54, 0, 5.78);
           	GLUquadricObj  *sphere2;
          	sphere2 = gluNewQuadric();
			gluQuadricDrawStyle(sphere2, GLU_FILL);
			gluQuadricTexture(sphere2, true);
            gluQuadricNormals(sphere2, GLU_SMOOTH);
			gluSphere(sphere2, 0.1, 20, 20);
			gluDeleteQuadric(sphere2);
		glPopMatrix();

		glPushMatrix();
			glRotatef(90, 0 , 0, 1);
			glRotatef(50, 1 , 0, 0);
           	glTranslatef(0, 3.3	, 5);
           	glScalef(1.6, 0.20, 1.6);
           	GLUquadricObj  *sphere3;
           	sphere3 = gluNewQuadric();
			gluQuadricDrawStyle(sphere3, GLU_FILL);
			gluQuadricTexture(sphere3, true);
			gluQuadricNormals(sphere3, GLU_SMOOTH);
			gluSphere(sphere3, 0.5, 20, 20);
			gluDeleteQuadric(sphere3);
        glPopMatrix();
    }
    glPopMatrix();
}
