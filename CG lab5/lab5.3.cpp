#include <GL/glut.h> // o <GL/freeglut.h> según tu instalación
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27

float translationX = 0.0f;
float translationY = 0.0f;
float zoom = 0.0f;

void init_scene();
void render_scene();

GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

// function called on each frame
GLvoid window_idle();

int main(int argc, char **argv) {
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

GLvoid initGL() {
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

void drawThings() {
  glPushMatrix();
  glTranslatef(-5, 0, 0);
  glutWireTeapot(1.0f);
  glTranslatef(5, 0, 0);
  glutWireSphere(0.5f, 10, 10);
  glTranslatef(5, 0, 0);
  glutWireTorus(0.5f, 1.5f, 10, 10);
  glPopMatrix();
}
GLvoid window_display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0, 1, 1.0, 100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(translationX, translationY, 10 + zoom, 0, 0, 0, 0, 1, 0);

  glTranslatef(0, 0, -5);
  drawThings();

  glutSwapBuffers();

  glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height) {
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

  glMatrixMode(GL_MODELVIEW);
}

void init_scene() {}

GLvoid window_key(unsigned char key, int x, int y) {
  switch (key) {
  case 'a':
    translationX -= 0.1f;
    break;
  case 'd':
    translationX += 0.1f;
    break;
  case 'w':
    translationY += 0.1f;
    break;
  case 's':
    translationY -= 0.1f;
    break;
  case 'q':
    zoom -= 0.1f;
    break;
  case 'e':
    zoom += 0.1f;
    break;

  case ECHAP:
    exit(1);
    break;

  default:
    printf("La touche %d non active.\n", key);
    break;
  }
}

// function called on each frame
GLvoid window_idle() { glutPostRedisplay(); }
