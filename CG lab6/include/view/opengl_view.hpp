#pragma once

#include "model/algorithms.hpp"
#include "model/scene.hpp"

namespace cg {

class OpenGLView {
public:
  void reshape(int width, int height) const;
  void render(const Scene &scene, const GraphicObject *draft, int width,
              int height) const;

private:
  void drawObject(const GraphicObject &object, bool selected, bool draft) const;
  void drawPoint(const Vec2 &point, const Color &color, float size) const;
  void drawPolyline(const std::vector<Vec2> &vertices, const Color &color,
                    bool closed, float thickness) const;
  void drawFilledPolygon(const std::vector<Vec2> &vertices,
                         const Color &color) const;
  void drawHighlight(const GraphicObject &object) const;
};

} // namespace cg
