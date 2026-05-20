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

float craneX = 0.0f;

float angleAlpha = 90.0f;
float angleBeta = 0.0f;

float ropeLength = 5.0f;

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
void draw_base() {
  glColor3f(0.5f, 0.7f, 1.0f);
  glPushMatrix();
  glScalef(6.0f, 0.8f, 1.0f);
  glutSolidCube(1.0f);
  glPopMatrix();
}

void draw_column() {
  glColor3f(0.6f, 0.5f, 0.9f);
  glPushMatrix();
  glTranslatef(0.0f, 4.0f, 0.0f);
  glScalef(1.0f, 8.0f, 1.0f);
  glutSolidCube(1.0f);
  glPopMatrix();
}

void draw_arm() {
  glColor3f(0.6f, 0.9f, 0.6f);
  glPushMatrix();
  glTranslatef(3.0f, 0.0f, 0.0f);
  glScalef(6.0f, 0.8f, 1.0f);
  glutSolidCube(1.0f);
  glPopMatrix();
}
void draw_rope_and_load() {
  glColor3f(0.3f, 0.3f, 0.3f);

  glPushMatrix();
  glRotatef(-angleBeta, 0.0f, 0.0f, 1.0f);
  glRotatef(-(angleAlpha - 90.0f), 0.0f, 0.0f, 1.0f);

  glDisable(GL_LIGHTING);
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, -ropeLength, 0.0f);
  glEnd();
  glEnable(GL_LIGHTING);

  glTranslatef(0.0f, -ropeLength, 0.0f);
  glColor3f(0.9f, 0.4f, 0.4f);
  glutSolidCube(1.2f);

  glPopMatrix();
}
void draw_crane() {
  glPushMatrix();

  glTranslatef(craneX, -10.0f, 0.0f);
  draw_base();

  glTranslatef(0.0f, 0.4f, 0.0f);
  glRotatef(angleAlpha - 90.0f, 0.0f, 0.0f, 1.0f);

  draw_column();

  glTranslatef(0.0f, 8.0f, 0.0f);
  glRotatef(angleBeta, 0.0f, 0.0f, 1.0f);

  draw_arm();

  glTranslatef(6.0f, 0.0f, 0.0f);
  draw_rope_and_load();

  glPopMatrix();
}
GLvoid window_display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  draw_crane();

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
  case 'q':
    angleAlpha += 2.0f;
    if (angleAlpha > 170.0f)
      angleAlpha = 170.0f;
    glutPostRedisplay();
    break;

  case 'e':
    angleAlpha -= 2.0f;
    if (angleAlpha < 10.0f)
      angleAlpha = 10.0f;
    glutPostRedisplay();
    break;
  case 'r':
    angleBeta += 2.0f;
    if (angleBeta > 60.0f)
      angleBeta = 60.0f;
    glutPostRedisplay();
    break;

  case 'f':
    angleBeta -= 2.0f;
    if (angleBeta < -60.0f)
      angleBeta = -60.0f;
    glutPostRedisplay();
    break;
  case 'u':
    ropeLength += 0.5f;
    if (ropeLength > 12.0f)
      ropeLength = 12.0f;
    glutPostRedisplay();
    break;

  case 'j':
    ropeLength -= 0.5f;
    if (ropeLength < 1.0f)
      ropeLength = 1.0f;
    glutPostRedisplay();
    break;
  case 'a':
    craneX -= 0.5f;
    if (craneX < -15.0f)
      craneX = -15.0f;
    glutPostRedisplay();
    break;

  case 'd':
    craneX += 0.5f;
    if (craneX > 15.0f)
      craneX = 15.0f;
    glutPostRedisplay();
    break;
  case ECHAP:
    exit(1);
    break;

  default:
    printf("La touche %d non active.\n", key);
    break;
  }
}

GLvoid window_idle() {
  teapotAngle += 2.5f;
  if (teapotAngle >= 360.0f) {
    teapotAngle -= 360.0f;
  }
  if (teapotTranslateX >= 4.0f || teapotTranslateX <= -4.0f) {
    teapotTranslateXSwitch *= -1.0f;
  }
  teapotTranslateX += teapotTranslateXSwitch;

  glutPostRedisplay();
}
