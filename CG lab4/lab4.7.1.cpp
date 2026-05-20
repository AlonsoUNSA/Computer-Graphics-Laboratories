#include <GL/glut.h>
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

float teapotAngle = 0.0f;
float teapotTranslateX = 0.0f;
float teapotTranslateXSwitch = 0.1f;

void init_scene();
void render_scene();

GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

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

  glutIdleFunc(&window_idle);

  glutMainLoop();

  return 1;
}

GLvoid initGL() {
  GLfloat position[] = {0.0f, 5.0f, 10.0f, 0.0};

  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glShadeModel(GL_SMOOTH);

  glEnable(GL_DEPTH_TEST);

  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  glClearColor(RED, GREEN, BLUE, ALPHA);
}
void apply_translation(float tx, float ty) {
  GLfloat m[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, tx, ty, 0, 1};
  glMultMatrixf(m);
}

void apply_rotation(float deg) {
  float rad = deg * M_PI / 180.0f;
  float c = cos(rad);
  float s = sin(rad);

  GLfloat m[16] = {c, s, 0, 0, -s, c, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
  glMultMatrixf(m);
}

void apply_scale(float sx, float sy) {
  GLfloat m[16] = {sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
  glMultMatrixf(m);
}
void draw_teapot() {
  glPushMatrix();
  glColor3f(0.8f, 0.3f, 0.2f);
  apply_rotation(teapotAngle);
  apply_translation(4.0f, 0.0f);
  glutSolidTeapot(2.0);
  glPopMatrix();
}
GLvoid window_display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  draw_teapot();

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
  case ECHAP:
    exit(1);
    break;

  default:
    printf("La touche %d non active.\n", key);
    break;
  }
}

GLvoid window_idle() {
  teapotAngle += 0.5f;
  if (teapotAngle >= 360.0f) {
    teapotAngle -= 360.0f;
  }
  glutPostRedisplay();
}