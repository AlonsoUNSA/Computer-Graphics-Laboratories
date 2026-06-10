#include "presenter/editor_presenter.hpp"

#include <GL/freeglut.h>

#include <algorithm>
#include <cmath>
#include <sstream>

namespace cg {

EditorPresenter::EditorPresenter(Scene &scene) : scene_(scene) {}

void EditorPresenter::onMouse(int button, int state, int x, int y,
                              int windowWidth, int windowHeight) {
  lastMouseX_ = x;
  lastMouseY_ = y;

  if (button == GLUT_RIGHT_BUTTON) {
    rightButtonDown_ = state == GLUT_DOWN;
    return;
  }

  if (button == 3 && state == GLUT_DOWN) {
    orbitDistance_ = std::max(2.5f, orbitDistance_ - 0.5f);
    updateCameraTarget();
    return;
  }

  if (button == 4 && state == GLUT_DOWN) {
    orbitDistance_ = std::min(40.0f, orbitDistance_ + 0.5f);
    updateCameraTarget();
    return;
  }

  if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) {
    return;
  }

  switch (currentTool_) {
  case ToolMode::Select:
    scene_.selectAt(x, y, windowWidth, windowHeight, camera_);
    statusLine_ = scene_.hasSelection() ? "Modo: seleccion de objeto activo"
                                        : "Modo: seleccion - sin objeto";
    break;
  case ToolMode::CreateCube:
    spawnObject(ObjectType::Cube, x, y, windowWidth, windowHeight);
    break;
  case ToolMode::CreateSphere:
    spawnObject(ObjectType::Sphere, x, y, windowWidth, windowHeight);
    break;
  case ToolMode::CreateTorus:
    spawnObject(ObjectType::Torus, x, y, windowWidth, windowHeight);
    break;
  case ToolMode::CreateTeapot:
    spawnObject(ObjectType::Teapot, x, y, windowWidth, windowHeight);
    break;
  }
}

void EditorPresenter::onMotion(int x, int y, int windowWidth,
                               int windowHeight) {
  if (!rightButtonDown_) {
    return;
  }

  updateTrackball(x, y, windowWidth, windowHeight);
}

void EditorPresenter::onKeyboard(unsigned char key, int, int) {
  switch (key) {
  case '1':
    setTool(ToolMode::Select);
    break;
  case '2':
    setTool(ToolMode::CreateCube);
    break;
  case '3':
    setTool(ToolMode::CreateSphere);
    break;
  case '4':
    setTool(ToolMode::CreateTorus);
    break;
  case '5':
    setTool(ToolMode::CreateTeapot);
    break;
  case 'x':
  case 'X':
    if (scene_.removeSelected()) {
      statusLine_ = "Objeto eliminado";
    }
    break;
  case '+':
  case '=':
    scene_.scaleSelected(
        {1.0f + scaleStep_, 1.0f + scaleStep_, 1.0f + scaleStep_});
    break;
  case '-':
  case '_':
    scene_.scaleSelected(
        {1.0f - scaleStep_, 1.0f - scaleStep_, 1.0f - scaleStep_});
    break;
  case '[':
    scene_.rotateSelected({0.0f, -rotateStep_, 0.0f});
    break;
  case ']':
    scene_.rotateSelected({0.0f, rotateStep_, 0.0f});
    break;
  case 'c':
  case 'C':
    scene_.selectNext();
    break;
  case 'w':
  case 'W':
    moveCamera({0.0f, 0.0f, -moveStep_});
    break;
  case 's':
  case 'S':
    moveCamera({0.0f, 0.0f, moveStep_});
    break;
  case 'a':
  case 'A':
    moveCamera({-moveStep_, 0.0f, 0.0f});
    break;
  case 'd':
  case 'D':
    moveCamera({moveStep_, 0.0f, 0.0f});
    break;
  case 'q':
  case 'Q':
    moveCamera({0.0f, moveStep_, 0.0f});
    break;
  case 'e':
  case 'E':
    moveCamera({0.0f, -moveStep_, 0.0f});
    break;
  case '.':
    camera_.fovY = std::max(25.0f, camera_.fovY - 2.0f);
    statusLine_ = "FOV reducido";
    break;
  case ',':
    camera_.fovY = std::min(85.0f, camera_.fovY + 2.0f);
    statusLine_ = "FOV aumentado";
    break;
  default:
    break;
  }
}

void EditorPresenter::onSpecial(int key, int, int) {
  switch (key) {
  case GLUT_KEY_LEFT:
    scene_.translateSelected({-moveStep_, 0.0f, 0.0f});
    break;
  case GLUT_KEY_RIGHT:
    scene_.translateSelected({moveStep_, 0.0f, 0.0f});
    break;
  case GLUT_KEY_UP:
    scene_.translateSelected({0.0f, moveStep_, 0.0f});
    break;
  case GLUT_KEY_DOWN:
    scene_.translateSelected({0.0f, -moveStep_, 0.0f});
    break;
  case GLUT_KEY_PAGE_UP:
    scene_.translateSelected({0.0f, 0.0f, -moveStep_});
    break;
  case GLUT_KEY_PAGE_DOWN:
    scene_.translateSelected({0.0f, 0.0f, moveStep_});
    break;
  default:
    break;
  }
}

ToolMode EditorPresenter::currentTool() const { return currentTool_; }

const CameraState &EditorPresenter::camera() const { return camera_; }

std::string EditorPresenter::statusLine() const {
  std::ostringstream output;
  output << statusLine_ << " | Objetos: " << scene_.size();
  if (const Object3D *selected = scene_.selectedObject()) {
    output << " | Activo: #" << selected->id << ' '
           << objectTypeName(selected->type);
  }
  return output.str();
}

void EditorPresenter::setTool(ToolMode tool) {
  currentTool_ = tool;
  switch (tool) {
  case ToolMode::Select:
    statusLine_ = "Modo: seleccion";
    break;
  case ToolMode::CreateCube:
    statusLine_ = "Modo: crear cubo";
    break;
  case ToolMode::CreateSphere:
    statusLine_ = "Modo: crear esfera";
    break;
  case ToolMode::CreateTorus:
    statusLine_ = "Modo: crear toro";
    break;
  case ToolMode::CreateTeapot:
    statusLine_ = "Modo: crear teapot";
    break;
  }
}

void EditorPresenter::spawnObject(ObjectType type, int x, int y,
                                  int windowWidth, int windowHeight) {
  const Vec3 position = screenToWorldPlane(x, y, windowWidth, windowHeight);
  const Color palette[] = {
      makeColor(0.95f, 0.35f, 0.25f), makeColor(0.2f, 0.8f, 0.95f),
      makeColor(0.35f, 0.9f, 0.45f), makeColor(0.95f, 0.8f, 0.25f)};
  const int paletteIndex = static_cast<int>(type);
  scene_.addObject(type, position, palette[paletteIndex]);
  scene_.selectIndex(static_cast<int>(scene_.size()) - 1);
  statusLine_ = std::string("Creado: ") + std::string(objectTypeName(type));
}

Vec3 EditorPresenter::screenToWorldPlane(int x, int y, int windowWidth,
                                         int windowHeight) const {
  const float normalizedX = static_cast<float>(x) / std::max(1, windowWidth);
  const float normalizedY = static_cast<float>(y) / std::max(1, windowHeight);
  return {((normalizedX - 0.5f) * 10.0f), ((0.5f - normalizedY) * 7.0f), 0.0f};
}

void EditorPresenter::updateTrackball(int x, int y, int windowWidth,
                                      int windowHeight) {
  const float deltaX = static_cast<float>(x - lastMouseX_);
  const float deltaY = static_cast<float>(y - lastMouseY_);
  lastMouseX_ = x;
  lastMouseY_ = y;

  orbitYaw_ += deltaX * 0.35f;
  orbitPitch_ = std::clamp(orbitPitch_ + deltaY * 0.35f, -80.0f, 80.0f);
  updateCameraTarget();
  statusLine_ = "Camara trackball activa";
  (void)windowWidth;
  (void)windowHeight;
}

void EditorPresenter::updateCameraTarget() {
  const float yawRadians = orbitYaw_ * kPi / 180.0f;
  const float pitchRadians = orbitPitch_ * kPi / 180.0f;
  const float cosinePitch = std::cos(pitchRadians);

  camera_.position = {
      camera_.target.x + orbitDistance_ * std::cos(yawRadians) * cosinePitch,
      camera_.target.y + orbitDistance_ * std::sin(pitchRadians),
      camera_.target.z + orbitDistance_ * std::sin(yawRadians) * cosinePitch,
  };
}

void EditorPresenter::moveCamera(const Vec3 &delta) {
  camera_.position = camera_.position + delta;
  camera_.target = camera_.target + delta;
  statusLine_ = "Camara libre actualizada";
}

} // namespace cg