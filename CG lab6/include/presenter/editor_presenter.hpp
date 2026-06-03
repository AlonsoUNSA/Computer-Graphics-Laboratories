#pragma once

#include "model/scene.hpp"

namespace cg {

enum class ToolMode {
  Select,
  Point,
  Line,
  Polyline,
  Polygon,
};

class EditorPresenter {
public:
  explicit EditorPresenter(Scene &scene);

  void onMouse(int button, int state, int x, int y, int windowHeight);
  void onKeyboard(unsigned char key, int x, int y);
  void onSpecial(int key, int x, int y);

  ToolMode currentTool() const;
  const GraphicObject *draftObject() const;

private:
  Vec2 toWorld(int x, int y, int windowHeight) const;
  void setTool(ToolMode tool);
  void addPoint(const Vec2 &point);
  void addLine(const Vec2 &first, const Vec2 &second);
  void finalizeDraft();
  void applySelectedTransform(const Mat3 &transform);
  void seedDraftIfNeeded(ShapeType type);

  Scene &scene_;
  ToolMode currentTool_ = ToolMode::Select;
  GraphicObject draft_;
  bool hasDraft_ = false;
  bool fillEnabled_ = true;
  Color outlineColor_ = makeColor(0.95f, 0.95f, 0.95f);
  Color fillColor_ = makeColor(0.2f, 0.6f, 0.9f);
  float translateStep_ = 12.0f;
  float rotationStep_ = 12.0f;
  float scaleFactor_ = 1.1f;
};

} // namespace cg
