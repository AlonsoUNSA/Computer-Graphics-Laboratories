#pragma once

#include "model/geometry.hpp"

#include <vector>

namespace cg {

enum class ShapeType {
  Point,
  Line,
  Polyline,
  Polygon,
};

struct GraphicObject {
  ShapeType type = ShapeType::Point;
  std::vector<Vec2> vertices;
  Color outlineColor = makeColor(1.0f, 1.0f, 1.0f);
  Color fillColor = makeColor(0.2f, 0.6f, 0.9f);
  bool filled = false;
};

class Scene {
public:
  std::size_t addObject(GraphicObject object);
  void clear();

  const std::vector<GraphicObject> &objects() const;
  std::vector<GraphicObject> &objects();
  std::size_t size() const;

  int selectedIndex() const;
  bool hasSelection() const;
  GraphicObject *selectedObject();
  const GraphicObject *selectedObject() const;
  void clearSelection();
  int selectAt(const Vec2 &point);
  int selectNext();

  void transformSelected(const Mat3 &transform);
  Vec2 selectedCentroid() const;

private:
  int hitTest(const Vec2 &point) const;

  std::vector<GraphicObject> objects_;
  int selectedIndex_ = -1;
};

} // namespace cg
