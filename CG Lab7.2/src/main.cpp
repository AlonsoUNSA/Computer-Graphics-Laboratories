#include "model/scene.hpp"
#include "presenter/editor_presenter.hpp"
#include "view/opengl_view.hpp"

#include <GL/freeglut.h>

#include <cstdlib>
#include <memory>

namespace {

struct Application {
  cg::Scene scene;
  cg::EditorPresenter presenter;
  cg::OpenGLView view;
  int width = 1280;
  int height = 800;

  Application() : presenter(scene) {
    scene.addObject(cg::ObjectType::Cube, cg::makeVec3(-3.0f, 0.0f, 0.0f),
                    cg::makeColor(0.95f, 0.35f, 0.25f));
    scene.addObject(cg::ObjectType::Sphere, cg::makeVec3(-1.0f, 0.0f, 0.0f),
                    cg::makeColor(0.2f, 0.8f, 0.95f));
    scene.addObject(cg::ObjectType::Torus, cg::makeVec3(1.5f, 0.0f, 0.0f),
                    cg::makeColor(0.35f, 0.9f, 0.45f));
    scene.addObject(cg::ObjectType::Teapot, cg::makeVec3(3.5f, 0.0f, 0.0f),
                    cg::makeColor(0.95f, 0.8f, 0.25f));
  }
};

std::unique_ptr<Application> app;

void displayCallback() {
  app->view.render(app->scene, app->presenter, app->width, app->height);
  glutSwapBuffers();
}

void reshapeCallback(int width, int height) {
  app->width = width;
  app->height = height > 0 ? height : 1;
  app->view.reshape(app->width, app->height);
  glutPostRedisplay();
}

void mouseCallback(int button, int state, int x, int y) {
  app->presenter.onMouse(button, state, x, y, app->width, app->height);
  glutPostRedisplay();
}

void motionCallback(int x, int y) {
  app->presenter.onMotion(x, y, app->width, app->height);
  glutPostRedisplay();
}

void keyboardCallback(unsigned char key, int x, int y) {
  if (key == 27) {
    std::exit(0);
  }

  app->presenter.onKeyboard(key, x, y);
  glutPostRedisplay();
}

void specialCallback(int key, int x, int y) {
  app->presenter.onSpecial(key, x, y);
  glutPostRedisplay();
}

} // namespace

int main(int argc, char **argv) {
  app = std::make_unique<Application>();

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(app->width, app->height);
  glutCreateWindow("CG Lab 7 - Editor 3D");

  glEnable(GL_DEPTH_TEST);

  glutDisplayFunc(displayCallback);
  glutReshapeFunc(reshapeCallback);
  glutMouseFunc(mouseCallback);
  glutMotionFunc(motionCallback);
  glutPassiveMotionFunc(motionCallback);
  glutKeyboardFunc(keyboardCallback);
  glutSpecialFunc(specialCallback);

  app->view.reshape(app->width, app->height);
  glutMainLoop();
  return 0;
}