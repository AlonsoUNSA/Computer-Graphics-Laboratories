#include "presenter/editor_presenter.hpp"

#include <GL/freeglut.h>

namespace cg {

EditorPresenter::EditorPresenter(Scene &scene) : scene_(scene) {}

Vec2 EditorPresenter::toWorld(int x, int y, int windowHeight) const {
  return makeVec2(static_cast<float>(x), static_cast<float>(windowHeight - y));
}

void EditorPresenter::setTool(ToolMode tool) {
  currentTool_ = tool;
  if (tool != ToolMode::Polyline && tool != ToolMode::Polygon &&
      tool != ToolMode::Line) {
    hasDraft_ = false;
    draft_ = {};
  }
}

ToolMode EditorPresenter::currentTool() const { return currentTool_; }

const GraphicObject *EditorPresenter::draftObject() const {
  return hasDraft_ ? &draft_ : nullptr;
}

void EditorPresenter::addPoint(const Vec2 &point) {
  GraphicObject object;
  object.type = ShapeType::Point;
  object.vertices = {point};
  object.outlineColor = outlineColor_;
  scene_.addObject(object);
}

void EditorPresenter::addLine(const Vec2 &first, const Vec2 &second) {
  GraphicObject object;
  object.type = ShapeType::Line;
  object.vertices = {first, second};
  object.outlineColor = outlineColor_;
  scene_.addObject(object);
}

void EditorPresenter::seedDraftIfNeeded(ShapeType type) {
  if (!hasDraft_) {
    draft_ = GraphicObject{};
    draft_.type = type;
    draft_.outlineColor = outlineColor_;
    draft_.fillColor = fillColor_;
    draft_.filled = fillEnabled_;
    hasDraft_ = true;
  }
}

void EditorPresenter::finalizeDraft() {
  if (!hasDraft_) {
    return;
  }

  if (draft_.type == ShapeType::Polyline && draft_.vertices.size() >= 2) {
    scene_.addObject(draft_);
  } else if (draft_.type == ShapeType::Polygon && draft_.vertices.size() >= 3) {
    draft_.filled = fillEnabled_;
    scene_.addObject(draft_);
  }

  hasDraft_ = false;
  draft_ = {};
}

void EditorPresenter::applySelectedTransform(const Mat3 &transform) {
  if (!scene_.hasSelection()) {
    return;
  }

  const Vec2 pivot = scene_.selectedCentroid();
  const Mat3 composed = Mat3::translation(pivot.x, pivot.y) * transform *
                        Mat3::translation(-pivot.x, -pivot.y);
  scene_.transformSelected(composed);
}

void EditorPresenter::onMouse(int button, int state, int x, int y,
                              int windowHeight) {
  if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) {
    return;
  }

  const Vec2 point = toWorld(x, y, windowHeight);

  switch (currentTool_) {
  case ToolMode::Select:
    scene_.selectAt(point);
    break;
  case ToolMode::Point:
    addPoint(point);
    break;
  case ToolMode::Line:
    if (!hasDraft_) {
      seedDraftIfNeeded(ShapeType::Line);
    }
    draft_.vertices.push_back(point);
    if (draft_.vertices.size() == 2) {
      addLine(draft_.vertices[0], draft_.vertices[1]);
      hasDraft_ = false;
      draft_ = {};
    }
    break;
  case ToolMode::Polyline:
    seedDraftIfNeeded(ShapeType::Polyline);
    draft_.vertices.push_back(point);
    break;
  case ToolMode::Polygon:
    seedDraftIfNeeded(ShapeType::Polygon);
    draft_.vertices.push_back(point);
    break;
  }
}

void EditorPresenter::onKeyboard(unsigned char key, int, int) {
  switch (key) {
  case '1':
    setTool(ToolMode::Select);
    break;
  case '2':
    setTool(ToolMode::Point);
    break;
  case '3':
    setTool(ToolMode::Line);
    break;
  case '4':
    setTool(ToolMode::Polyline);
    break;
  case '5':
    setTool(ToolMode::Polygon);
    break;
  case 13:
    finalizeDraft();
    break;
  case 27:
    hasDraft_ = false;
    draft_ = {};
    scene_.clearSelection();
    break;
  case 'f':
  case 'F':
    fillEnabled_ = !fillEnabled_;
    break;
  case 'w':
  case 'W':
    applySelectedTransform(Mat3::translation(0.0f, translateStep_));
    break;
  case 's':
  case 'S':
    applySelectedTransform(Mat3::translation(0.0f, -translateStep_));
    break;
  case 'a':
  case 'A':
    applySelectedTransform(Mat3::translation(-translateStep_, 0.0f));
    break;
  case 'd':
  case 'D':
    applySelectedTransform(Mat3::translation(translateStep_, 0.0f));
    break;
  case 'q':
  case 'Q':
    applySelectedTransform(Mat3::rotationDegrees(rotationStep_));
    break;
  case 'e':
  case 'E':
    applySelectedTransform(Mat3::rotationDegrees(-rotationStep_));
    break;
  case '+':
  case '=':
    applySelectedTransform(Mat3::scale(scaleFactor_, scaleFactor_));
    break;
  case '-':
  case '_':
    applySelectedTransform(
        Mat3::scale(1.0f / scaleFactor_, 1.0f / scaleFactor_));
    break;
  case 'c':
  case 'C':
    scene_.selectNext();
    break;
  default:
    break;
  }
}

void EditorPresenter::onSpecial(int key, int, int) {
  switch (key) {
  case GLUT_KEY_UP:
    applySelectedTransform(Mat3::translation(0.0f, translateStep_));
    break;
  case GLUT_KEY_DOWN:
    applySelectedTransform(Mat3::translation(0.0f, -translateStep_));
    break;
  case GLUT_KEY_LEFT:
    applySelectedTransform(Mat3::translation(-translateStep_, 0.0f));
    break;
  case GLUT_KEY_RIGHT:
    applySelectedTransform(Mat3::translation(translateStep_, 0.0f));
    break;
  default:
    break;
  }
}

} // namespace cg
