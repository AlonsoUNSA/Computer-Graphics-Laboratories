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

int activeTransform = 0; // 0=ninguna, 1=traslacion, 2=rotacion, 3=escala
void init_scene();
void render_scene();

// === TEAPOT ANIMADO ===
int phase = 0;            // fase actual de la animación
float teapotRot = 0.0f;   // rotación acumulada
float teapotX = 0.0f;     // traslación en X
float teapotScale = 1.0f; // escala actual

// límites de cada fase
const float MAX_ROT = 360.0f;
const float MAX_X = 12.0f;
const float MAX_SCALE = 3.0f;

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
void apply_translation(float tx, float ty) {
  GLfloat m[16] = {
      1,  0,  0, 0, // columna 1
      0,  1,  0, 0, // columna 2
      0,  0,  1, 0, // columna 3
      tx, ty, 0, 1  // columna 4: traslación
  };
  glMultMatrixf(m);
}

void apply_rotation(float deg) {
  float rad = deg * M_PI / 180.0f;
  float c = cos(rad);
  float s = sin(rad);

  GLfloat m[16] = {
      c,  s, 0, 0, // columna 1
      -s, c, 0, 0, // columna 2
      0,  0, 1, 0, // columna 3
      0,  0, 0, 1  // columna 4
  };
  glMultMatrixf(m);
}

void apply_scale(float sx, float sy) {
  GLfloat m[16] = {
      sx, 0,  0, 0, // columna 1
      0,  sy, 0, 0, // columna 2
      0,  0,  1, 0, // columna 3
      0,  0,  0, 1  // columna 4
  };
  glMultMatrixf(m);
}
void apply_combined(float tx, float tz, float rotY, float sc) {
  //  Matriz de traslación
  GLfloat mT[16] = {1, 0, 0, 0, 
                    0, 1, 0, 0, 
                    0, 0, 1, 0, 
                    tx, 0, tz, 1};

  //  Matriz de rotación en Y
  float rad = rotY * M_PI / 180.0f;
  float c = cos(rad), s = sin(rad);
  GLfloat mR[16] = {c, 0, -s, 0, 
                    0, 1, 0, 0, 
                    s, 0, c, 0, 
                    0, 0, 0, 1};

  //  Matriz de escala en X y Z
  GLfloat mS[16] = {sc, 0, 0, 0, 
                    0, sc, 0, 0, 
                    0, 0, sc, 0, 
                    0, 0, 0, 1};

  // T * R * S
  glMultMatrixf(mT);
  glMultMatrixf(mR);
  glMultMatrixf(mS);
}
void draw_teapot_animated() {
  glPushMatrix();
  apply_combined(teapotX, 0.0f, teapotRot, teapotScale);
  glColor3f(0.7f, 0.7f, 0.7f);
  glutSolidTeapot(2.0f);
  glPopMatrix();
}

GLvoid window_display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  draw_teapot_animated(); // <-- agrega esta línea

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
  switch (phase) {
  case 0: // rotar sobre sí mismo
    teapotRot += 2.0f;
    if (teapotRot >= MAX_ROT) {
      teapotRot = MAX_ROT;
    }
    teapotX += 0.1f;
    if (teapotX >= MAX_X) {
      teapotX = MAX_X;
    }
    teapotScale += 0.02f;
    if (teapotScale >= MAX_SCALE) {
      teapotScale = MAX_SCALE;
      phase = 3;
    }
    break;

  case 1: // trasladarse a la derecha
    break;

  case 2: // escalar más grande
    break;

  case 3: // rotar en sentido contrario
    teapotRot -= 2.0f;
    if (teapotRot <= 0.0f) {
      teapotRot = 0.0f;
    }
    teapotX -= 0.1f;
    if (teapotX <= 0.0f) {
      teapotX = 0.0f;
    }
    teapotScale -= 0.02f;
    if (teapotScale <= 1.0f) {
      teapotScale = 1.0f;
      phase = 0;
    }
    break;

  case 4: // volver a la izquierda
    break;

  case 5: // reducir escala al original
    break;
  }

  glutPostRedisplay();
}
