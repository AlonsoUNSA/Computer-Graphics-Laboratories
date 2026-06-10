#pragma once

#include "model/scene.hpp"

#include <string>

namespace cg {

class EditorPresenter;

class OpenGLView {
public:
  void reshape(int width, int height) const;
  void render(const Scene &scene, const EditorPresenter &presenter, int width,
              int height) const;

private:
  void drawObject(const Object3D &object, bool selected) const;
  void drawAxisGizmo(float length) const;
  void drawGrid(float size, int divisions) const;
  void drawText(float x, float y, const std::string &text) const;
  void drawOverlay(const Scene &scene, const EditorPresenter &presenter,
                   int width, int height) const;
};

} // namespace cg