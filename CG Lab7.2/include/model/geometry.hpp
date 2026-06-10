#pragma once

#include <algorithm>
#include <cmath>
#include <optional>

namespace cg {

struct Vec2 {
  float x = 0.0f;
  float y = 0.0f;
};

struct Vec3 {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
};

struct Color {
  float r = 1.0f;
  float g = 1.0f;
  float b = 1.0f;
};

struct CameraState {
  Vec3 position{0.0f, 0.0f, 8.0f};
  Vec3 target{0.0f, 0.0f, 0.0f};
  Vec3 up{0.0f, 1.0f, 0.0f};
  float fovY = 45.0f;
  float nearPlane = 0.1f;
  float farPlane = 100.0f;
};

constexpr float kPi = 3.14159265358979323846f;

inline Vec2 makeVec2(float x, float y) { return {x, y}; }

inline Vec3 makeVec3(float x, float y, float z) { return {x, y, z}; }

inline Color makeColor(float r, float g, float b) { return {r, g, b}; }

inline Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

inline Vec3 operator-(const Vec3 &lhs, const Vec3 &rhs) {
  return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

inline Vec3 operator*(const Vec3 &value, float scalar) {
  return {value.x * scalar, value.y * scalar, value.z * scalar};
}

inline Vec3 operator/(const Vec3 &value, float scalar) {
  return {value.x / scalar, value.y / scalar, value.z / scalar};
}

inline float dot(const Vec3 &lhs, const Vec3 &rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

inline Vec3 cross(const Vec3 &lhs, const Vec3 &rhs) {
  return {lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z,
          lhs.x * rhs.y - lhs.y * rhs.x};
}

inline float length(const Vec3 &value) { return std::sqrt(dot(value, value)); }

inline Vec3 normalize(const Vec3 &value) {
  const float len = length(value);
  if (len <= 0.00001f) {
    return {0.0f, 0.0f, 0.0f};
  }
  return value / len;
}

inline Vec3 clamp(const Vec3 &value, float minValue, float maxValue) {
  return {std::clamp(value.x, minValue, maxValue),
          std::clamp(value.y, minValue, maxValue),
          std::clamp(value.z, minValue, maxValue)};
}

inline std::optional<Vec2> projectToScreen(const Vec3 &point,
                                           const CameraState &camera, int width,
                                           int height) {
  const Vec3 forward = normalize(camera.target - camera.position);
  const Vec3 right = normalize(cross(forward, camera.up));
  const Vec3 cameraUp = cross(right, forward);
  const Vec3 relative = point - camera.position;

  const float viewX = dot(relative, right);
  const float viewY = dot(relative, cameraUp);
  const float viewZ = dot(relative, forward);

  if (viewZ <= camera.nearPlane) {
    return std::nullopt;
  }

  const float safeHeight = static_cast<float>(std::max(1, height));
  const float safeWidth = static_cast<float>(std::max(1, width));
  const float aspect = safeWidth / safeHeight;
  const float tangent = std::tan((camera.fovY * kPi / 180.0f) * 0.5f);
  const float ndcX = viewX / (viewZ * tangent * aspect);
  const float ndcY = viewY / (viewZ * tangent);

  if (std::abs(ndcX) > 1.5f || std::abs(ndcY) > 1.5f) {
    return std::nullopt;
  }

  return Vec2{(ndcX + 1.0f) * 0.5f * safeWidth,
              (1.0f - ndcY) * 0.5f * safeHeight};
}

} // namespace cg