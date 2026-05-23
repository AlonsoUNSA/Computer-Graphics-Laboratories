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

float yaw = 0.0f;    // izquierda/derecha
float pitch = 20.0f; // arriba/abajo

float distancia = 20.0f;

bool mouseLeftDown = false;

int lastMouseX;
int lastMouseY;

void init_scene();
void render_scene();

GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

// function called on each frame
GLvoid window_idle();
void mouseButton(int button, int state, int x, int y) {

  // botón izquierdo
  if (button == GLUT_LEFT_BUTTON) {

    if (state == GLUT_DOWN) {

      mouseLeftDown = true;

      lastMouseX = x;
      lastMouseY = y;
    } else {

      mouseLeftDown = false;
    }
  }

  // rueda arriba
  if (button == 3) {
    distancia -= 1.0f;
  }

  // rueda abajo
  if (button == 4) {
    distancia += 1.0f;
  }

  glutPostRedisplay();
}
void mouseMotion(int x, int y) {

  if (mouseLeftDown) {

    int dx = x - lastMouseX;
    int dy = y - lastMouseY;

    yaw += dx * 0.5f;
    pitch -= dy * 0.5f;

    lastMouseX = x;
    lastMouseY = y;

    glutPostRedisplay();
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowSize(800, 800);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Lab 3: Transformaciones");

  initGL();
  init_scene();

  glutDisplayFunc(&window_display);

  // registrar mouse
  glutMouseFunc(mouseButton);

  glutMotionFunc(mouseMotion);

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
  glColor3f(1, 1, 1);
  glTranslatef(-5, 0, 0);
  glutWireTeapot(1.0f);
  glTranslatef(5, 0, 0);
  glutWireSphere(0.5f, 10, 10);
  glTranslatef(5, 0, 0);
  glutWireTorus(0.5f, 1.5f, 10, 10);
  glPopMatrix();
}
void drawGizmo(float size = 5.0f) {

  glBegin(GL_LINES);

  // ----- EJE X -----
  glColor3f(1, 0, 0);

  glVertex3f(0, 0, 0);
  glVertex3f(size, 0, 0);

  // ----- EJE Y -----
  glColor3f(0, 1, 0);

  glVertex3f(0, 0, 0);
  glVertex3f(0, size, 0);

  // ----- EJE Z -----
  glColor3f(0, 0, 1);

  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, size);

  glEnd();
}
GLvoid window_display() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(60.0, 1.0, 1.0, 1000.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // grados -> radianes
  float radYaw = yaw * 3.14159f / 180.0f;
  float radPitch = pitch * 3.14159f / 180.0f;

  // posición cámara
  float camX = -1 * distancia * cos(radPitch) * sin(radYaw);

  float camY = distancia * sin(radPitch);

  float camZ = -1 * distancia * cos(radPitch) * cos(radYaw);

  gluLookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);

  drawThings();
  drawGizmo();

  glutSwapBuffers();
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

// function called on each frame
GLvoid window_idle() { glutPostRedisplay(); }
