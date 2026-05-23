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

float t_angle = 0.0f;
float s_angle = 0.0f;

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
void draw_sun() {
  glPushMatrix();
  glColor3f(0.8f, 0.8f, 0.0f);
  glRotatef(s_angle, 0.0f, 0.0f, 1.0f);
  glutSolidSphere(4.0, 8, 8);
  glPopMatrix();
}
void draw_earth() {
  glPushMatrix();
  glColor3f(0.8f, 0.3f, 0.2f);
  glRotatef(t_angle, 0.0f, 0.0f, 1.0f);
  glTranslatef(10.0f, 0.0f, 0.0f);
  glRotatef(s_angle * 3, 0.0f, 0.0f, 1.0f);
  glutWireSphere(2, 20, 20);

  glRotatef(t_angle * 2.0, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, 0.0f, 4.0f);
  glRotatef(s_angle * 1.5, 1.0f, 0.0f, 0.0f);

  glColor3f(0.2f, 0.3f, 0.8f);
  glutWireSphere(1.0, 8, 8);
  glPopMatrix();
}
void draw_marte() {
  glPushMatrix();
  glColor3f(0.8f, 0.2f, 0.2f);
  glRotatef(t_angle * 1.6, 0.0f, 0.0f, 1.0f);
  glTranslatef(18.0f, 0.0f, 0.0f);
  glRotatef(s_angle, 0.0f, 0.0f, 1.0f);
  glutWireSphere(2, 20, 20);
  glPopMatrix();
}
GLvoid window_display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(90.0, 1, 1.0, 100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float rad = t_angle * 3.14159f / 180.0f;
  gluLookAt(10 * cos(rad) + translationX, 10 * sin(rad) + translationY,
            30 + zoom,                       // eye
            10 * cos(rad), 10 * sin(rad), 0, // point of view
            0, 1, 0);

  // Draw here!
  draw_sun();
  draw_earth();
  draw_marte();

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
    translationX -= 2.1f;
    break;
  case 'd':
    translationX += 2.1f;
    break;
  case 'w':
    translationY += 2.1f;
    break;
  case 's':
    translationY -= 2.1f;
    break;
  case 'q':
    zoom -= 2.1f;
    break;
  case 'e':
    zoom += 2.1f;
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
GLvoid window_idle() {
  t_angle += 0.5f;
  if (t_angle >= 360.0f) {
    t_angle -= 360.0f;
  }
  s_angle += 0.3f;
  if (s_angle >= 360.0f) {
    s_angle -= 360.0f;
  }

  glutPostRedisplay();
}