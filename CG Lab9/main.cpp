#include <GLUT/glut.h> // o <GL/freeglut.h> según tu instalación
#include <OpenGL/glu.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27

GLuint texGrass;
GLuint texBrick;
GLuint texRoof;
GLuint texBark;
GLuint texLeaves;

float teapotAngle = 0.0f;
float teapotTranslateX = 0.0f;
float teapotTranslateXSwitch = 0.1f;
float cameraAngle = 0.0f;
const float cameraRadius = 65.0f;
const float cameraHeight = 40.0f;

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

GLuint loadTexture(const char *filename) {
  GLuint textureID;

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, channels;

  unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);

  if (data) {
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
  }

  stbi_image_free(data);

  return textureID;
}
void setFloorMaterial() {
  GLfloat ambient[] = {0.1f, 0.25f, 0.1f, 1.0f};
  GLfloat diffuse[] = {0.2f, 0.6f, 0.2f, 1.0f};
  GLfloat specular[] = {0.05f, 0.05f, 0.05f, 1.0f};
  GLfloat shininess = 8.0f;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
void setWallMaterial() {
  GLfloat ambient[] = {0.3f, 0.05f, 0.05f, 1.0f};
  GLfloat diffuse[] = {0.8f, 0.1f, 0.1f, 1.0f};
  GLfloat specular[] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat shininess = 16.0f;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
void setRoofMaterial() {
  GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat diffuse[] = {0.6f, 0.6f, 0.6f, 1.0f};
  GLfloat specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat shininess = 32.0f;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
void setTrunkMaterial() {
  GLfloat ambient[] = {0.2f, 0.1f, 0.05f, 1.0f};
  GLfloat diffuse[] = {0.55f, 0.27f, 0.07f, 1.0f};
  GLfloat specular[] = {0.1f, 0.1f, 0.1f, 1.0f};
  GLfloat shininess = 10.0f;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
void setLeavesMaterial() {
  GLfloat ambient[] = {0.0f, 0.2f, 0.0f, 1.0f};
  GLfloat diffuse[] = {0.0f, 0.6f, 0.0f, 1.0f};
  GLfloat specular[] = {0.1f, 0.3f, 0.1f, 1.0f};
  GLfloat shininess = 20.0f;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

GLvoid initGL() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);

  GLfloat light_position[] = {0.0f, 10.0f, 10.0f, 1.0f};
  GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
  GLfloat light_diffuse[] = {1.0f, 0.8f, 0.8f, 1.0f};
  GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glEnable(GL_TEXTURE_2D);

  texGrass = loadTexture("textures/grassTex.jpg");
  texBrick = loadTexture("textures/brickTex.jpg");
  texRoof = loadTexture("textures/roofTex.png");
  texBark = loadTexture("textures/treeBarkTex.jpg");
  texLeaves = loadTexture("textures/leavesTex.jpg");
}
void drawCube(float x) {
  glPushMatrix();

  // mover cubo en X
  glTranslatef(x, 0.0f, 0.0f);

  glBegin(GL_QUADS);

  // Frente - rojo
  // glColor3f(1, 0, 0);
  glVertex3f(-1, -1, 1);
  glVertex3f(1, -1, 1);
  glVertex3f(1, 1, 1);
  glVertex3f(-1, 1, 1);

  // Atrás - verde
  // glColor3f(0, 1, 0);
  glVertex3f(-1, -1, -1);
  glVertex3f(-1, 1, -1);
  glVertex3f(1, 1, -1);
  glVertex3f(1, -1, -1);

  // Arriba - azul
  // glColor3f(0, 0, 1);
  glVertex3f(-1, 1, -1);
  glVertex3f(-1, 1, 1);
  glVertex3f(1, 1, 1);
  glVertex3f(1, 1, -1);

  // Abajo - amarillo
  // glColor3f(1, 1, 0);
  glVertex3f(-1, -1, -1);
  glVertex3f(1, -1, -1);
  glVertex3f(1, -1, 1);
  glVertex3f(-1, -1, 1);

  // Derecha - magenta
  // glColor3f(1, 0, 1);
  glVertex3f(1, -1, -1);
  glVertex3f(1, 1, -1);
  glVertex3f(1, 1, 1);
  glVertex3f(1, -1, 1);

  // Izquierda - cyan
  // glColor3f(0, 1, 1);
  glVertex3f(-1, -1, -1);
  glVertex3f(-1, -1, 1);
  glVertex3f(-1, 1, 1);
  glVertex3f(-1, 1, -1);

  glEnd();

  glPopMatrix();
}
void drawFloor() {
  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, texGrass);
  glBegin(GL_QUADS);
  // glColor3f(0.5, 0.9, 0.5);
  glNormal3f(0.0f, 1.0f, 0.0f);
  glTexCoord2f(0, 0);
  glVertex3f(-20, -1, -20);
  glTexCoord2f(1, 0);
  glVertex3f(20, -1, -20);
  glTexCoord2f(1, 1);
  glVertex3f(20, -1, 20);
  glTexCoord2f(0, 1);
  glVertex3f(-20, -1, 20);
  glEnd();
  glPopMatrix();
}
void drawHouse() {

  glPushMatrix();
  setWallMaterial();
  glTranslatef(0, 0, 10);

  glBindTexture(GL_TEXTURE_2D, texBrick);
  glBegin(GL_QUADS);
  // Pared frontal
  // glColor3f(1, 0, 0); // Rojo

  glNormal3f(0, 0, 1);

  glTexCoord2f(0, 0);
  glVertex3f(-6, -1, 4);
  glTexCoord2f(1, 0);
  glVertex3f(6, -1, 4);
  glTexCoord2f(1, 1);
  glVertex3f(6, 6, 4);
  glTexCoord2f(0, 1);
  glVertex3f(-6, 6, 4);

  // Pared trasera
  // glColor3f(0, 1, 0); // Verde
  glNormal3f(0, 0, -1);
  glTexCoord2f(0, 0);
  glVertex3f(-6, -1, -4);

  glTexCoord2f(0, 1);
  glVertex3f(-6, 6, -4);

  glTexCoord2f(1, 1);
  glVertex3f(6, 6, -4);

  glTexCoord2f(1, 0);
  glVertex3f(6, -1, -4);

  // Pared lateral derecha
  // glColor3f(1, 0.5, 0.5); // Rojo claro
  glNormal3f(1, 0, 0);

  glTexCoord2f(0, 0);
  glVertex3f(6, -1, -4);

  glTexCoord2f(0, 1);
  glVertex3f(6, 6, -4);

  glTexCoord2f(1, 1);
  glVertex3f(6, 6, 4);

  glTexCoord2f(1, 0);
  glVertex3f(6, -1, 4);

  // pared lateral izquierda
  // glColor3f(1, 0.5, 0.5); // Rojo claro
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-6, -1, -4);
  glTexCoord2f(1, 0);
  glVertex3f(-6, -1, 4);
  glTexCoord2f(1, 1);
  glVertex3f(-6, 6, 4);
  glTexCoord2f(0, 1);
  glVertex3f(-6, 6, -4);
  glEnd();
  // Techo
  // triangulo frontal
  // glColor3f(1, 0.25, 0.25);
  glBegin(GL_TRIANGLES);
  glNormal3f(0, 0, 1);
  glTexCoord2f(0, 0);
  glVertex3f(-6, 6, 4);
  glTexCoord2f(1, 0);
  glVertex3f(6, 6, 4);
  glTexCoord2f(0.5, 1);
  glVertex3f(0, 10, 4);
  // triangulo trasero
  glNormal3f(0, 0, -1);
  glTexCoord2f(0, 0);
  glVertex3f(-6, 6, -4);
  glTexCoord2f(1, 0);
  glVertex3f(0, 10, -4);
  glTexCoord2f(0.5, 1);
  glVertex3f(6, 6, -4);
  glEnd();

  setRoofMaterial();
  // rectangulo lateral dere

  glBindTexture(GL_TEXTURE_2D, texRoof);
  glBegin(GL_QUADS);
  // color gris
  // glColor3f(0.5, 0.5, 0.5);
  // lado derecho del techo
  glNormal3f(0.555f, 0.832f, 0.0f);

  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(6, 6, -4);

  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0, 10, -4);

  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0, 10, 4);

  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(6, 6, 4);

  // lado izquierdo del techo
  glNormal3f(-0.555f, 0.832f, 0.0f);

  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-6, 6, -4);

  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-6, 6, 4);

  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0, 10, 4);

  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0, 10, -4);

  glEnd();
  glPopMatrix();
}

void drawTree() {
  // draw cinlinder
  glPushMatrix();
  // glColor3f(0.55, 0.27, 0.07); // Brown color for the trunk
  glTranslatef(0.0f, 1.0f, -10.0f);
  glRotatef(-90, 1, 0, 0);

  setTrunkMaterial();

  GLUquadric *quad = gluNewQuadric();
  glBindTexture(GL_TEXTURE_2D, texBark);
  gluQuadricTexture(quad, GL_TRUE);
  gluCylinder(quad, 2.0f, 2.0f, 12.0f, 32, 32); // Draw the cylinder
  gluDeleteQuadric(quad);

  // drawSphere
  // glColor3f(0.0f, 0.5f, 0.0f);     // Green color for the foliage
  glTranslatef(0.0f, 0.0f, 12.0f);

  setLeavesMaterial();

  GLUquadric *leaf = gluNewQuadric();
  gluQuadricTexture(leaf, GL_TRUE);
  glBindTexture(GL_TEXTURE_2D, texLeaves);
  gluSphere(leaf, 4.0f, 32, 32);
  gluDeleteQuadric(leaf);

  glPopMatrix();
}

GLvoid window_display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float cameraRadians = cameraAngle * 0.0174532925f;
  float cameraX = cameraRadius * cosf(cameraRadians);
  float cameraZ = cameraRadius * sinf(cameraRadians);
  gluLookAt(cameraX, cameraHeight, cameraZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

  // drawCube(-5);
  setWallMaterial();
  // drawCube(0);

  setFloorMaterial();
  drawFloor();

  drawHouse();

  setTrunkMaterial();
  drawTree();

  glutSwapBuffers();

  glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height) {
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect = height == 0 ? 1.0f : (float)width / (float)height;
  gluPerspective(45.0, aspect, 1.0, 200.0);

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
  teapotAngle += 2.5f;
  if (teapotAngle >= 360.0f) {
    teapotAngle -= 360.0f;
  }
  cameraAngle += 0.45f;
  if (cameraAngle >= 360.0f) {
    cameraAngle -= 360.0f;
  }
  if (teapotTranslateX >= 4.0f || teapotTranslateX <= -4.0f) {
    teapotTranslateXSwitch *= -1.0f;
  }
  teapotTranslateX += teapotTranslateXSwitch;

  glutPostRedisplay();
}
