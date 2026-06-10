#include "view/opengl_view.hpp"

#include "presenter/editor_presenter.hpp"

#include <GL/freeglut.h>

#include <algorithm>

namespace cg {

namespace {

const char *toolName(ToolMode tool) {
  switch (tool) {
  case ToolMode::Select:
    return "Seleccion";
  case ToolMode::CreateCube:
    return "Crear cubo";
  case ToolMode::CreateSphere:
    return "Crear esfera";
  case ToolMode::CreateTorus:
    return "Crear toro";
  case ToolMode::CreateTeapot:
    return "Crear teapot";
  }
  return "Desconocido";
}

} // namespace

void OpenGLView::reshape(int width, int height) const {
  glViewport(0, 0, width, height);
}

void OpenGLView::render(const Scene &scene, const EditorPresenter &presenter,
                        int width, int height) const {
  glClearColor(0.08f, 0.09f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  const CameraState &camera = presenter.camera();
  gluPerspective(camera.fovY, static_cast<double>(width) / std::max(1, height),
                 camera.nearPlane, camera.farPlane);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(camera.position.x, camera.position.y, camera.position.z,
            camera.target.x, camera.target.y, camera.target.z, camera.up.x,
            camera.up.y, camera.up.z);

  drawGrid(12.0f, 12);
  drawAxisGizmo(2.2f);

  for (const Object3D &object : scene.objects()) {
    drawObject(object, object.selected);
  }

  drawOverlay(scene, presenter, width, height);
}

void OpenGLView::drawObject(const Object3D &object, bool selected) const {
  glPushMatrix();
  glTranslatef(object.position.x, object.position.y, object.position.z);
  glRotatef(object.rotation.x, 1.0f, 0.0f, 0.0f);
  glRotatef(object.rotation.y, 0.0f, 1.0f, 0.0f);
  glRotatef(object.rotation.z, 0.0f, 0.0f, 1.0f);
  glScalef(object.scale.x, object.scale.y, object.scale.z);

  glColor3f(object.color.r, object.color.g, object.color.b);

  switch (object.type) {
  case ObjectType::Cube:
    glutSolidCube(1.0);
    break;
  case ObjectType::Sphere:
    glutSolidSphere(0.55, 24, 24);
    break;
  case ObjectType::Torus:
    glutSolidTorus(0.18, 0.42, 16, 32);
    break;
  case ObjectType::Teapot:
    glutSolidTeapot(0.48);
    break;
  }

  if (selected) {
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glutWireCube(1.25);
    glLineWidth(1.0f);
  }

  glPopMatrix();
}

void OpenGLView::drawAxisGizmo(float length) const {
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);

  glColor3f(1.0f, 0.2f, 0.2f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(length, 0.0f, 0.0f);

  glColor3f(0.2f, 1.0f, 0.2f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, length, 0.0f);

  glColor3f(0.2f, 0.4f, 1.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, length);

  glEnd();
}

void OpenGLView::drawGrid(float size, int divisions) const {
  const float step = size / std::max(1, divisions);
  const float halfSize = size * 0.5f;

  glColor3f(0.2f, 0.22f, 0.26f);
  glBegin(GL_LINES);
  for (int index = -divisions; index <= divisions; ++index) {
    const float coordinate = index * step * 0.5f;
    glVertex3f(coordinate, 0.0f, -halfSize);
    glVertex3f(coordinate, 0.0f, halfSize);
    glVertex3f(-halfSize, 0.0f, coordinate);
    glVertex3f(halfSize, 0.0f, coordinate);
  }
  glEnd();
}

void OpenGLView::drawText(float x, float y, const std::string &text) const {
  glRasterPos2f(x, y);
  for (char character : text) {
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, character);
  }
}

void OpenGLView::drawOverlay(const Scene &scene,
                             const EditorPresenter &presenter, int width,
                             int height) const {
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0.0, width, 0.0, height);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glColor3f(1.0f, 1.0f, 1.0f);
  drawText(18.0f, static_cast<float>(height - 24),
           std::string("Lab 7 - Editor 3D | Modo: ") +
               toolName(presenter.currentTool()));
  drawText(18.0f, static_cast<float>(height - 42), presenter.statusLine());

  std::string info = "Objetos en escena: " + std::to_string(scene.size());
  if (const Object3D *selected = scene.selectedObject()) {
    info += " | Seleccionado #" + std::to_string(selected->id) + " ";
    info += std::string(objectTypeName(selected->type));
  }
  drawText(18.0f, static_cast<float>(height - 60), info);

  drawText(18.0f, static_cast<float>(height - 86),
           "1 seleccion | 2 cubo | 3 esfera | 4 toro | 5 teapot | X elimina");
  drawText(18.0f, static_cast<float>(height - 104),
           "WASD/QE camara | flechas mover objeto | [ ] rotar | +/- escalar");

  glEnable(GL_DEPTH_TEST);

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

} // namespace cg