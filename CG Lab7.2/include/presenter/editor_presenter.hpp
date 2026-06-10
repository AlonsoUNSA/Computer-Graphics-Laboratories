#pragma once

#include "model/scene.hpp"

#include <string>

namespace cg {

enum class ToolMode {
  Select,
  CreateCube,
  CreateSphere,
  CreateTorus,
  CreateTeapot,
};

class EditorPresenter {
public:
  explicit EditorPresenter(Scene &scene);

  void onMouse(int button, int state, int x, int y, int windowWidth,
               int windowHeight);
  void onMotion(int x, int y, int windowWidth, int windowHeight);
  void onKeyboard(unsigned char key, int x, int y);
  void onSpecial(int key, int x, int y);

  ToolMode currentTool() const;
  const CameraState &camera() const;
  std::string statusLine() const;

private:
  void setTool(ToolMode tool);
  void spawnObject(ObjectType type, int x, int y, int windowWidth,
                   int windowHeight);
  Vec3 screenToWorldPlane(int x, int y, int windowWidth,
                          int windowHeight) const;
  void updateTrackball(int x, int y, int windowWidth, int windowHeight);
  void updateCameraTarget();
  void moveCamera(const Vec3 &delta);

  Scene &scene_;
  ToolMode currentTool_ = ToolMode::Select;
  CameraState camera_;
  std::string statusLine_ = "Modo: seleccion";
  bool rightButtonDown_ = false;
  int lastMouseX_ = 0;
  int lastMouseY_ = 0;
  float orbitYaw_ = 45.0f;
  float orbitPitch_ = 20.0f;
  float orbitDistance_ = 10.0f;
  float moveStep_ = 0.5f;
  float rotateStep_ = 10.0f;
  float scaleStep_ = 0.1f;
};

} // namespace cg