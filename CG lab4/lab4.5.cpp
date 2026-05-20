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
// === POLIGONO ===
float polyX[20], polyY[20]; // máximo 20 vértices
int numVertices = 0;        // cuántos lleva el usuario
bool polyReady = false;     // true cuando el usuario termina de ingresar

// valores hardcodeados de cada transformación
float tx = 5.0f, ty = 3.0f; // traslación
float rotDeg = 45.0f;       // rotación en grados
float sx = 2.0f, sy = 0.5f; // escalamiento

int activeTransform = 0; // 0=ninguna, 1=traslacion, 2=rotacion, 3=escala
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
void input_vertices() {
  cout << "=== INGRESO DE VERTICES ===" << endl;
  cout << "Ingresa los puntos 2D del poligono." << endl;
  cout << "Cuando termines escribe 'done'" << endl;

  numVertices = 0;
  string input;

  while (numVertices < 20) {
    cout << "Vertice " << (numVertices + 1) << " (x y) o 'done': ";
    cin >> input;

    if (input == "done") {
      if (numVertices < 3)
        cout << "Necesitas al menos 3 vertices!" << endl;
      else
        break;
    } else {
      polyX[numVertices] = stof(input); // leer x
      cin >> polyY[numVertices];        // leer y
      cout << "  -> Vertice agregado: (" << polyX[numVertices] << ", "
           << polyY[numVertices] << ")" << endl;
      numVertices++;
    }
  }

  polyReady = true;
  cout << "Poligono listo con " << numVertices << " vertices!" << endl;
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
void draw_polygon() {
  if (!polyReady)
    return;

  glPushMatrix();

  // Aplicar solo la transformación activa
  if (activeTransform == 1) {
    cout << "Traslacion: tx=" << tx << " ty=" << ty << endl;
    apply_translation(tx, ty);
  } else if (activeTransform == 2) {
    cout << "Rotacion: " << rotDeg << " grados" << endl;
    apply_rotation(rotDeg);
  } else if (activeTransform == 3) {
    cout << "Escala: sx=" << sx << " sy=" << sy << endl;
    apply_scale(sx, sy);
  }

  // Relleno
  glColor3f(0.4f, 0.8f, 1.0f);
  glBegin(GL_POLYGON);
  for (int i = 0; i < numVertices; i++)
    glVertex2f(polyX[i], polyY[i]);
  glEnd();

  // Contorno
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < numVertices; i++)
    glVertex2f(polyX[i], polyY[i]);
  glEnd();

  glPopMatrix();
}
GLvoid window_display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_LIGHTING);
  draw_polygon();
  glEnable(GL_LIGHTING);

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

void init_scene() {
  input_vertices(); // pedir vértices antes de abrir la ventana
}

GLvoid window_key(unsigned char key, int x, int y) {
  switch (key) {
  case '1': // traslación
    activeTransform = 1;
    glutPostRedisplay();
    break;

  case '2': // rotación
    activeTransform = 2;
    glutPostRedisplay();
    break;

  case '3': // escala
    activeTransform = 3;
    glutPostRedisplay();
    break;

  case '0': // sin transformación (polígono original)
    activeTransform = 0;
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

// function called on each frame
GLvoid window_idle() { glutPostRedisplay(); }
