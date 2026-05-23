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
// === FIGURA HUMANA ===
float figureAngleY = 0.0f; // rotación de toda la figura sobre su eje Y
float translationX = 0.0f; // posición horizontal de la figura

// Ángulos de articulaciones
float leftArmAngle = 0.0f;  // brazo izquierdo (hombro)
float rightArmAngle = 0.0f; // brazo derecho (hombro)
float leftLegAngle = 0.0f;  // pierna izquierda (cadera)
float rightLegAngle = 0.0f; // pierna derecha (cadera)
float walkAngle = 0.0f;     // ángulo base que oscila con el tiempo
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
void draw_head() {
  glColor3f(1.0f, 0.9f, 0.2f);
  glPushMatrix();
  glScalef(2.0f, 2.0f, 1.0f);
  glutSolidCube(1.0f);
  glPopMatrix();
}

void draw_torso() {
  glColor3f(0.3f, 0.5f, 0.9f);
  glPushMatrix();
  glScalef(3.0f, 4.0f, 1.0f);
  glutSolidCube(1.0f);
  glPopMatrix();
}

void draw_arm() {
  glColor3f(0.3f, 0.8f, 0.4f);
  glPushMatrix();
  glScalef(1.0f, 3.0f, 1.0f);
  glutSolidCube(1.0f);
  glPopMatrix();
}

void draw_leg() {
  glColor3f(0.9f, 0.4f, 0.2f);
  glPushMatrix();
  glScalef(1.2f, 3.5f, 1.0f);
  glutSolidCube(1.0f);
  glPopMatrix();
}
void draw_figure() {
  glPushMatrix(); // matriz de toda la figura
  glTranslatef(translationX, 0.0f, 0.0f);
  glRotatef(figureAngleY, 0.0f, 1.0f, 0.0f);

  // Posicionar figura en el centro de la escena
  glTranslatef(0.0f, 0.0f, 0.0f);

  // --- TORSO  ---
  draw_torso();

  // --- CABEZA ---
  glPushMatrix();
  glTranslatef(0.0f, 3.0f, 0.0f); // subir al tope del torso
  draw_head();
  glPopMatrix();

  // --- BRAZO IZQUIERDO ---
  glPushMatrix();
  glTranslatef(-2.0f, 1.0f, 0.0f);
  glRotatef(leftArmAngle, 1.0f, 0.0f, 0.0f); // eje X, no Z
  glTranslatef(0.0f, -1.5f, 0.0f);
  draw_arm();
  glPopMatrix();

  // --- BRAZO DERECHO ---
  glPushMatrix();
  glTranslatef(2.0f, 1.0f, 0.0f);
  glRotatef(rightArmAngle, 1.0f, 0.0f, 0.0f); // eje X, no Z
  glTranslatef(0.0f, -1.5f, 0.0f);
  draw_arm();
  glPopMatrix();

  // --- PIERNA IZQUIERDA ---
  glPushMatrix();
  glTranslatef(-0.8f, -2.0f, 0.0f);
  glRotatef(leftLegAngle, 1.0f, 0.0f, 0.0f); // eje X, no Z
  glTranslatef(0.0f, -1.75f, 0.0f);
  draw_leg();
  glPopMatrix();

  // --- PIERNA DERECHA ---
  glPushMatrix();
  glTranslatef(0.8f, -2.0f, 0.0f);
  glRotatef(rightLegAngle, 1.0f, 0.0f, 0.0f); // eje X, no Z
  glTranslatef(0.0f, -1.75f, 0.0f);
  draw_leg();
  glPopMatrix();

  glPopMatrix();
}
GLvoid window_display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  draw_figure();
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

// function called on each frame
GLvoid window_idle() {
  walkAngle += 2.0f;
  if (walkAngle >= 360.0f)
    walkAngle -= 360.0f;

  // sin() produce oscilación suave entre -1 y 1
  float swing = sin(walkAngle * M_PI / 180.0f) * 40.0f; // amplitud 40 grados

  // Brazos y piernas opuestos entre sí
  leftArmAngle = swing;
  rightArmAngle = -swing;
  leftLegAngle = -swing;
  rightLegAngle = swing;
  figureAngleY += 0.5f; // <-- NUEVO
  if (figureAngleY >= 90.0f) {

    figureAngleY = 90.0f;  // <-- NUEVO
    translationX += 0.05f; // <-- NUEVO
    if (translationX >= 80.0f)
      translationX = 80.0f; // <-- NUEVO
  }

  glutPostRedisplay();
}
