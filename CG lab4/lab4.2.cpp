#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>   // o <GL/freeglut.h> según tu instalación

using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27

float teapotAngle = 0.0f;
float teapotTranslateX = 0.0f;
float teapotTranslateXSwitch = 0.1f;

void init_scene();
void render_scene();

GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

// function called on each frame
GLvoid window_idle();

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Lab 3: Transformaciones");

	initGL();
	init_scene();

	glutDisplayFunc(&window_display);

	glutReshapeFunc(&window_reshape);

	glutKeyboardFunc(&window_key);

	// function called on each frame

	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}

GLvoid initGL()
{
	GLfloat position[] = {0.0f, 5.0f, 10.0f, 0.0};

	// enable light: try without it
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	// light 0 "on": try without it
	glEnable(GL_LIGHT0);

	// shading model : try GL_FLAT
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	// enable material : try without it
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(RED, GREEN, BLUE, ALPHA);
}
void draw_teapot()
{
    glPushMatrix();

		glTranslatef(0.0f, 9.0f, 0.0f);
	  glRotatef(teapotAngle, 0.0f, 0.0f, -1.0f);
	  glRotatef(teapotAngle, 0.0f,  -1.0f,0.0f);
    glBegin(GL_QUADS);

    // Cara frontal
    glColor3f(1,0,0);

    glVertex3f(-1,-1, 1);
    glVertex3f( 1,-1, 1);
    glVertex3f( 1, 1, 1);
    glVertex3f(-1, 1, 1);

    // Cara trasera
    glColor3f(0,1,0);

    glVertex3f(-1,-1,-1);
    glVertex3f(-1, 1,-1);
    glVertex3f( 1, 1,-1);
    glVertex3f( 1,-1,-1);
    glEnd();
    glPopMatrix();
}
void draw_square()
{
		glPushMatrix();
	  glRotatef(teapotAngle, 0.0f, 1.0f, 0.0f);
		glColor3f(0.2f, 0.8f, 0.3f);
		glutSolidCube(3.0);

    glBegin(GL_QUADS);
    // Cara frontal
    glColor3f(1,0,0);

    glVertex3f(1,8, 0);
    glVertex3f( 1,1, 0);
    glVertex3f( 2, 1, 0);
    glVertex3f(2, 8, 0);
    glEnd();
		glPopMatrix();
}
GLvoid window_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw here!
	draw_teapot();
	draw_square();
	glutSwapBuffers();

	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

	glMatrixMode(GL_MODELVIEW);
}

void init_scene()
{
}

GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ECHAP:
		exit(1);
		break;

	default:
		printf("La touche %d non active.\n", key);
		break;
	}
}

// function called on each frame
GLvoid window_idle()
{
	teapotAngle += 5.0f;
	if (teapotAngle >= 360.0f)
	{
		teapotAngle -= 360.0f;
	}
	glutPostRedisplay();
}
