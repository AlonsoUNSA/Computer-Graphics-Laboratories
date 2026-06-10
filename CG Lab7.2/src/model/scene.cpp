#include "model/scene.hpp"

#include <algorithm>
#include <cmath>

namespace cg {

std::string_view objectTypeName(ObjectType type) {
  switch (type) {
  case ObjectType::Cube:
    return "Cubo";
  case ObjectType::Sphere:
    return "Esfera";
  case ObjectType::Torus:
    return "Toro";
  case ObjectType::Teapot:
    return "Teapot";
  }
  return "Desconocido";
}

Object3D &Scene::addObject(ObjectType type, const Vec3 &position,
                           const Color &color) {
  Object3D object;
  object.id = nextId_++;
  object.type = type;
  object.position = position;
  object.color = color;
  objects_.push_back(object);
  return objects_.back();
}

bool Scene::removeSelected() {
  if (!hasSelection()) {
    return false;
  }

  objects_.erase(objects_.begin() + selectedIndex_);
  selectedIndex_ = -1;
  return true;
}

void Scene::clear() {
  objects_.clear();
  selectedIndex_ = -1;
}

const std::vector<Object3D> &Scene::objects() const { return objects_; }

std::vector<Object3D> &Scene::objects() { return objects_; }

std::size_t Scene::size() const { return objects_.size(); }

int Scene::selectedIndex() const { return selectedIndex_; }

bool Scene::hasSelection() const {
  return selectedIndex_ >= 0 &&
         selectedIndex_ < static_cast<int>(objects_.size());
}

void Scene::selectIndex(int index) {
  if (index < 0 || index >= static_cast<int>(objects_.size())) {
    clearSelection();
    return;
  }

  setSelection(index);
}

Object3D *Scene::selectedObject() {
  if (!hasSelection()) {
    return nullptr;
  }
  return &objects_[selectedIndex_];
}

const Object3D *Scene::selectedObject() const {
  if (!hasSelection()) {
    return nullptr;
  }
  return &objects_[selectedIndex_];
}

void Scene::clearSelection() { setSelection(-1); }

bool Scene::selectAt(int mouseX, int mouseY, int width, int height,
                     const CameraState &camera) {
  if (objects_.empty()) {
    clearSelection();
    return false;
  }

  float bestDistance = 24.0f;
  int bestIndex = -1;

  for (int index = 0; index < static_cast<int>(objects_.size()); ++index) {
    const auto projected =
        projectToScreen(objects_[index].position, camera, width, height);
    if (!projected.has_value()) {
      continue;
    }

    const float dx = projected->x - static_cast<float>(mouseX);
    const float dy = projected->y - static_cast<float>(mouseY);
    const float distance = std::sqrt(dx * dx + dy * dy);
    if (distance < bestDistance) {
      bestDistance = distance;
      bestIndex = index;
    }
  }

  setSelection(bestIndex);
  return hasSelection();
}

bool Scene::selectNext() {
  if (objects_.empty()) {
    clearSelection();
    return false;
  }

  if (!hasSelection()) {
    setSelection(0);
    return true;
  }

  const int nextIndex =
      (selectedIndex_ + 1) % static_cast<int>(objects_.size());
  setSelection(nextIndex);
  return true;
}

void Scene::translateSelected(const Vec3 &delta) {
  if (Object3D *object = selectedObject()) {
    object->position = object->position + delta;
  }
}

void Scene::rotateSelected(const Vec3 &delta) {
  if (Object3D *object = selectedObject()) {
    object->rotation = object->rotation + delta;
  }
}

void Scene::scaleSelected(const Vec3 &factor) {
  if (Object3D *object = selectedObject()) {
    object->scale =
        clamp({object->scale.x * factor.x, object->scale.y * factor.y,
               object->scale.z * factor.z},
              0.1f, 12.0f);
  }
}

void Scene::setSelection(int index) {
  selectedIndex_ = index;
  for (int current = 0; current < static_cast<int>(objects_.size());
       ++current) {
    objects_[current].selected = current == selectedIndex_;
  }
}

} // namespace cg