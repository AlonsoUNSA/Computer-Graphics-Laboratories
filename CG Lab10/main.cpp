#include <GLUT/glut.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>

#include "include/simulacion.hpp"

namespace {
constexpr unsigned char TECLA_ESC = 27;

SimulacionBoids simulacion;
bool pausado = false;

int ancho_ventana = 1000;
int alto_ventana = 700;
float ancho_mundo = 140.0f;
float alto_mundo = 98.0f;

int tiempo_previo_ms = 0;

void dibujar_texto(float x, float y, const std::string &texto) {
  glRasterPos2f(x, y);
  for (char c : texto) {
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
  }
}

void dibujar_boid(const Boid &boid) {
  const float angulo_grados =
      std::atan2(boid.direccion.y, boid.direccion.x) * 180.0f / 3.14159265f;
  const float largo = 3.1f;
  const float ancho = 1.8f;

  glPushMatrix();
  glTranslatef(boid.posicion.x, boid.posicion.y, 0.0f);
  glRotatef(angulo_grados, 0.0f, 0.0f, 1.0f);

  glBegin(GL_TRIANGLES);
  glVertex2f(largo, 0.0f);
  glVertex2f(-largo * 0.75f, ancho);
  glVertex2f(-largo * 0.75f, -ancho);
  glEnd();

  glPopMatrix();
}

void actualizar_dimensiones_mundo() {
  const float aspecto =
      static_cast<float>(ancho_ventana) / static_cast<float>(alto_ventana);
  const float alto_base = 100.0f;
  alto_mundo = alto_base;
  ancho_mundo = alto_base * aspecto;
  simulacion.establecer_dimensiones(ancho_mundo, alto_mundo);
}

void inicializar_gl() {
  glDisable(GL_DEPTH_TEST);
  glClearColor(0.05f, 0.06f, 0.09f, 1.0f);
  glShadeModel(GL_SMOOTH);
}

void mostrar_escena() {
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-ancho_mundo * 0.5f, ancho_mundo * 0.5f, -alto_mundo * 0.5f,
          alto_mundo * 0.5f, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glColor3f(0.68f, 0.90f, 0.95f);
  for (const Boid &boid : simulacion.boids()) {
    dibujar_boid(boid);
  }

  glColor3f(0.95f, 0.95f, 0.95f);
  const float margen_izq = -ancho_mundo * 0.5f + 2.5f;
  float y = alto_mundo * 0.5f - 4.0f;

  dibujar_texto(margen_izq, y, "Boids (CG Lab10)");
  y -= 4.0f;
  dibujar_texto(margen_izq, y,
                "Cantidad [N/M]: " +
                    std::to_string(simulacion.cantidad_boids()));
  y -= 3.2f;
  dibujar_texto(margen_izq, y,
                "Radio vecindad [R/F]: " +
                    std::to_string(simulacion.radio_vecindad()));
  y -= 3.2f;
  dibujar_texto(margen_izq, y,
                "Separacion [Q/A]: " +
                    std::to_string(simulacion.peso_separacion()));
  y -= 3.2f;
  dibujar_texto(margen_izq, y,
                "Alineamiento [W/S]: " +
                    std::to_string(simulacion.peso_alineamiento()));
  y -= 3.2f;
  dibujar_texto(margen_izq, y,
                "Cohesion [E/D]: " +
                    std::to_string(simulacion.peso_cohesion()));
  y -= 3.2f;
  dibujar_texto(margen_izq, y, "Pausa [space] - Reiniciar [C] - Salir [ESC]");

  glutSwapBuffers();
}

void cambiar_tamano_ventana(GLsizei width, GLsizei height) {
  if (height == 0) {
    height = 1;
  }

  ancho_ventana = static_cast<int>(width);
  alto_ventana = static_cast<int>(height);

  glViewport(0, 0, width, height);
  actualizar_dimensiones_mundo();
}

void teclado(unsigned char key, int, int) {
  switch (key) {
  case TECLA_ESC:
    std::exit(0);
    break;
  case 'n':
  case 'N':
    simulacion.cambiar_cantidad_boids(+5);
    break;
  case 'm':
  case 'M':
    simulacion.cambiar_cantidad_boids(-5);
    break;
  case 'r':
  case 'R':
    simulacion.cambiar_radio_vecindad(+1.5f);
    break;
  case 'f':
  case 'F':
    simulacion.cambiar_radio_vecindad(-1.5f);
    break;
  case 'q':
  case 'Q':
    simulacion.cambiar_peso_separacion(+0.1f);
    break;
  case 'a':
  case 'A':
    simulacion.cambiar_peso_separacion(-0.1f);
    break;
  case 'w':
  case 'W':
    simulacion.cambiar_peso_alineamiento(+0.1f);
    break;
  case 's':
  case 'S':
    simulacion.cambiar_peso_alineamiento(-0.1f);
    break;
  case 'e':
  case 'E':
    simulacion.cambiar_peso_cohesion(+0.1f);
    break;
  case 'd':
  case 'D':
    simulacion.cambiar_peso_cohesion(-0.1f);
    break;
  case 'c':
  case 'C':
    simulacion.reiniciar(simulacion.cantidad_boids());
    break;
  case ' ':
    pausado = !pausado;
    break;
  default:
    std::printf("Tecla no asignada: %d\n", key);
    break;
  }

  glutPostRedisplay();
}

void animacion() {
  const int ahora_ms = glutGet(GLUT_ELAPSED_TIME);
  if (tiempo_previo_ms == 0) {
    tiempo_previo_ms = ahora_ms;
  }

  float dt = static_cast<float>(ahora_ms - tiempo_previo_ms) * 0.001f;
  tiempo_previo_ms = ahora_ms;
  if (dt > 0.05f) {
    dt = 0.05f;
  }

  if (!pausado) {
    simulacion.actualizar(dt);
  }

  glutPostRedisplay();
}
} // namespace

int main(int argc, char **argv) {
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(ancho_ventana, alto_ventana);
  glutInitWindowPosition(40, 40);
  glutCreateWindow("CG Lab10 - Boids 2D");

  inicializar_gl();
  actualizar_dimensiones_mundo();
  simulacion.reiniciar(60);

  glutDisplayFunc(&mostrar_escena);
  glutReshapeFunc(&cambiar_tamano_ventana);
  glutKeyboardFunc(&teclado);
  glutIdleFunc(&animacion);

  glutMainLoop();
  return 0;
}
