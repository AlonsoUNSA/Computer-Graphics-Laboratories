#pragma once

#include "model/geometry.hpp"

#include <string_view>
#include <vector>

namespace cg {

enum class ObjectType {
  Cube,
  Sphere,
  Torus,
  Teapot,
};

std::string_view objectTypeName(ObjectType type);

struct Object3D {
  int id = 0;
  ObjectType type = ObjectType::Cube;
  Vec3 position{0.0f, 0.0f, 0.0f};
  Vec3 rotation{0.0f, 0.0f, 0.0f};
  Vec3 scale{1.0f, 1.0f, 1.0f};
  Color color{0.8f, 0.8f, 0.8f};
  bool selected = false;
};

class Scene {
public:
  Object3D &addObject(ObjectType type, const Vec3 &position,
                      const Color &color);
  bool removeSelected();
  void clear();

  const std::vector<Object3D> &objects() const;
  std::vector<Object3D> &objects();
  std::size_t size() const;

  int selectedIndex() const;
  bool hasSelection() const;
  void selectIndex(int index);
  Object3D *selectedObject();
  const Object3D *selectedObject() const;
  void clearSelection();
  bool selectAt(int mouseX, int mouseY, int width, int height,
                const CameraState &camera);
  bool selectNext();

  void translateSelected(const Vec3 &delta);
  void rotateSelected(const Vec3 &delta);
  void scaleSelected(const Vec3 &factor);

private:
  void setSelection(int index);

  std::vector<Object3D> objects_;
  int selectedIndex_ = -1;
  int nextId_ = 1;
};

} // namespace cg