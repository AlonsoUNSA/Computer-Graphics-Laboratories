#include "model/scene.hpp"

#include <algorithm>

namespace cg {

std::size_t Scene::addObject(GraphicObject object) {
  objects_.push_back(std::move(object));
  return objects_.size() - 1;
}

void Scene::clear() {
  objects_.clear();
  clearSelection();
}

const std::vector<GraphicObject> &Scene::objects() const { return objects_; }

std::vector<GraphicObject> &Scene::objects() { return objects_; }

std::size_t Scene::size() const { return objects_.size(); }

int Scene::selectedIndex() const { return selectedIndex_; }

bool Scene::hasSelection() const {
  return selectedIndex_ >= 0 &&
         selectedIndex_ < static_cast<int>(objects_.size());
}

GraphicObject *Scene::selectedObject() {
  if (!hasSelection()) {
    return nullptr;
  }
  return &objects_[static_cast<std::size_t>(selectedIndex_)];
}

const GraphicObject *Scene::selectedObject() const {
  if (!hasSelection()) {
    return nullptr;
  }
  return &objects_[static_cast<std::size_t>(selectedIndex_)];
}

void Scene::clearSelection() { selectedIndex_ = -1; }

int Scene::hitTest(const Vec2 &point) const {
  constexpr float pointThreshold = 10.0f;
  constexpr float lineThreshold = 7.0f;

  for (int index = static_cast<int>(objects_.size()) - 1; index >= 0; --index) {
    const GraphicObject &object = objects_[static_cast<std::size_t>(index)];
    if (object.vertices.empty()) {
      continue;
    }

    if (object.type == ShapeType::Point) {
      if (distance(object.vertices.front(), point) <= pointThreshold) {
        return index;
      }
      continue;
    }

    const bool closed = object.type == ShapeType::Polygon;
    const std::size_t segmentCount =
        closed ? object.vertices.size() : object.vertices.size() - 1;
    for (std::size_t i = 0; i < segmentCount; ++i) {
      const std::size_t next = (i + 1) % object.vertices.size();
      if (distancePointToSegment(point, object.vertices[i],
                                 object.vertices[next]) <= lineThreshold) {
        return index;
      }
    }

    if (closed && pointInPolygon(point, object.vertices)) {
      return index;
    }
  }

  return -1;
}

int Scene::selectAt(const Vec2 &point) {
  selectedIndex_ = hitTest(point);
  return selectedIndex_;
}

int Scene::selectNext() {
  if (objects_.empty()) {
    selectedIndex_ = -1;
    return selectedIndex_;
  }

  if (!hasSelection()) {
    selectedIndex_ = 0;
    return selectedIndex_;
  }

  selectedIndex_ = (selectedIndex_ + 1) % static_cast<int>(objects_.size());
  return selectedIndex_;
}

void Scene::transformSelected(const Mat3 &transform) {
  GraphicObject *object = selectedObject();
  if (object == nullptr) {
    return;
  }

  for (Vec2 &vertex : object->vertices) {
    vertex = transform.apply(vertex);
  }
}

Vec2 Scene::selectedCentroid() const {
  const GraphicObject *object = selectedObject();
  if (object == nullptr) {
    return {};
  }

  return centroid(object->vertices);
}

} // namespace cg
