#include "model/scene.hpp"
#include "presenter/editor_presenter.hpp"
#include "view/opengl_view.hpp"

#include <GL/freeglut.h>

#include <cstdlib>
#include <memory>
#include <vector>

namespace {

struct Application {
  cg::Scene scene;
  cg::EditorPresenter presenter;
  cg::OpenGLView view;
  int width = 1200;
  int height = 800;

  Application() : presenter(scene) {
    scene.addObject({cg::ShapeType::Point,
                     {cg::makeVec2(90.0f, 90.0f)},
                     cg::makeColor(0.95f, 0.85f, 0.2f),
                     cg::makeColor(0.2f, 0.6f, 0.9f),
                     false});
    scene.addObject({cg::ShapeType::Point,
                     {cg::makeVec2(150.0f, 150.0f)},
                     cg::makeColor(0.95f, 0.4f, 0.2f),
                     cg::makeColor(0.2f, 0.6f, 0.9f),
                     false});
    scene.addObject(
        {cg::ShapeType::Line,
         {cg::makeVec2(220.0f, 120.0f), cg::makeVec2(360.0f, 180.0f)},
         cg::makeColor(0.2f, 0.95f, 0.7f),
         cg::makeColor(0.2f, 0.6f, 0.9f),
         false});
    scene.addObject(
        {cg::ShapeType::Line,
         {cg::makeVec2(400.0f, 190.0f), cg::makeVec2(520.0f, 140.0f)},
         cg::makeColor(0.4f, 0.8f, 1.0f),
         cg::makeColor(0.2f, 0.6f, 0.9f),
         false});

    cg::GraphicObject polylineA;
    polylineA.type = cg::ShapeType::Polyline;
    polylineA.vertices = {
        cg::makeVec2(100.0f, 300.0f), cg::makeVec2(160.0f, 350.0f),
        cg::makeVec2(240.0f, 330.0f), cg::makeVec2(320.0f, 390.0f)};
    polylineA.outlineColor = cg::makeColor(0.8f, 0.3f, 0.8f);
    scene.addObject(polylineA);

    cg::GraphicObject polygonA;
    polygonA.type = cg::ShapeType::Polygon;
    polygonA.vertices = {
        cg::makeVec2(390.0f, 320.0f), cg::makeVec2(480.0f, 380.0f),
        cg::makeVec2(520.0f, 340.0f), cg::makeVec2(500.0f, 270.0f)};
    polygonA.outlineColor = cg::makeColor(0.95f, 0.85f, 0.2f);
    polygonA.fillColor = cg::makeColor(0.2f, 0.35f, 0.85f);
    polygonA.filled = true;
    scene.addObject(polygonA);

    cg::GraphicObject polygonB;
    polygonB.type = cg::ShapeType::Polygon;
    polygonB.vertices = {
        cg::makeVec2(600.0f, 170.0f), cg::makeVec2(700.0f, 220.0f),
        cg::makeVec2(680.0f, 300.0f), cg::makeVec2(590.0f, 260.0f)};
    polygonB.outlineColor = cg::makeColor(0.2f, 0.9f, 0.4f);
    polygonB.fillColor = cg::makeColor(0.15f, 0.55f, 0.25f);
    polygonB.filled = true;
    scene.addObject(polygonB);

    cg::GraphicObject polygonC;
    polygonC.type = cg::ShapeType::Polygon;
    polygonC.vertices = {
        cg::makeVec2(760.0f, 340.0f), cg::makeVec2(860.0f, 400.0f),
        cg::makeVec2(920.0f, 340.0f), cg::makeVec2(880.0f, 250.0f),
        cg::makeVec2(790.0f, 250.0f)};
    polygonC.outlineColor = cg::makeColor(0.95f, 0.2f, 0.2f);
    polygonC.fillColor = cg::makeColor(0.7f, 0.2f, 0.15f);
    polygonC.filled = true;
    scene.addObject(polygonC);

    cg::GraphicObject transformedLine;
    transformedLine.type = cg::ShapeType::Line;
    transformedLine.vertices = {cg::makeVec2(280.0f, 520.0f),
                                cg::makeVec2(360.0f, 590.0f)};
    transformedLine.outlineColor = cg::makeColor(0.6f, 0.9f, 0.9f);
    transformedLine.vertices[0] =
        cg::Mat3::translation(40.0f, -20.0f).apply(transformedLine.vertices[0]);
    transformedLine.vertices[1] =
        cg::Mat3::translation(40.0f, -20.0f).apply(transformedLine.vertices[1]);
    scene.addObject(transformedLine);

    cg::GraphicObject transformedPolygon;
    transformedPolygon.type = cg::ShapeType::Polygon;
    transformedPolygon.vertices = {
        cg::makeVec2(560.0f, 520.0f), cg::makeVec2(650.0f, 560.0f),
        cg::makeVec2(630.0f, 650.0f), cg::makeVec2(540.0f, 620.0f)};
    transformedPolygon.outlineColor = cg::makeColor(0.8f, 0.6f, 0.2f);
    transformedPolygon.fillColor = cg::makeColor(0.4f, 0.3f, 0.1f);
    transformedPolygon.filled = true;
    const cg::Vec2 pivot = cg::centroid(transformedPolygon.vertices);
    const cg::Mat3 transform = cg::Mat3::translation(pivot.x, pivot.y) *
                               cg::Mat3::scale(1.1f, 0.9f) *
                               cg::Mat3::translation(-pivot.x, -pivot.y);
    for (cg::Vec2 &vertex : transformedPolygon.vertices) {
      vertex = transform.apply(vertex);
    }
    scene.addObject(transformedPolygon);
  }
};

std::unique_ptr<Application> app;

void displayCallback() {
  app->view.render(app->scene, app->presenter.draftObject(), app->width,
                   app->height);
  glutSwapBuffers();
}

void reshapeCallback(int width, int height) {
  app->width = width;
  app->height = height > 0 ? height : 1;
  app->view.reshape(app->width, app->height);
  glutPostRedisplay();
}

void mouseCallback(int button, int state, int x, int y) {
  app->presenter.onMouse(button, state, x, y, app->height);
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
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(app->width, app->height);
  glutCreateWindow("CG Lab 6 - Editor 2D");

  glutDisplayFunc(displayCallback);
  glutReshapeFunc(reshapeCallback);
  glutMouseFunc(mouseCallback);
  glutKeyboardFunc(keyboardCallback);
  glutSpecialFunc(specialCallback);

  app->view.reshape(app->width, app->height);
  glutMainLoop();
  return 0;
}
