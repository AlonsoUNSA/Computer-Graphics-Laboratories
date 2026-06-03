#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <vector>

namespace cg {

struct Vec2 {
  float x = 0.0f;
  float y = 0.0f;
};

struct Color {
  float r = 1.0f;
  float g = 1.0f;
  float b = 1.0f;
};

inline Vec2 makeVec2(float x, float y) { return Vec2{x, y}; }

inline Color makeColor(float r, float g, float b) { return Color{r, g, b}; }

inline Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs) {
  return {lhs.x - rhs.x, lhs.y - rhs.y};
}

inline Vec2 operator*(const Vec2 &value, float scalar) {
  return {value.x * scalar, value.y * scalar};
}

inline Vec2 operator/(const Vec2 &value, float scalar) {
  return {value.x / scalar, value.y / scalar};
}

inline Vec2 &operator+=(Vec2 &lhs, const Vec2 &rhs) {
  lhs.x += rhs.x;
  lhs.y += rhs.y;
  return lhs;
}

inline float dot(const Vec2 &lhs, const Vec2 &rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

inline float cross(const Vec2 &lhs, const Vec2 &rhs) {
  return lhs.x * rhs.y - lhs.y * rhs.x;
}

inline float lengthSquared(const Vec2 &value) { return dot(value, value); }

inline float distanceSquared(const Vec2 &lhs, const Vec2 &rhs) {
  return lengthSquared(lhs - rhs);
}

inline float distance(const Vec2 &lhs, const Vec2 &rhs) {
  return std::sqrt(distanceSquared(lhs, rhs));
}

struct Mat3 {
  std::array<std::array<float, 3>, 3> m{};

  Mat3() { m = {{{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}}; }

  static Mat3 identity() { return Mat3{}; }

  static Mat3 translation(float tx, float ty) {
    Mat3 result = identity();
    result.m[0][2] = tx;
    result.m[1][2] = ty;
    return result;
  }

  static Mat3 scale(float sx, float sy) {
    Mat3 result = identity();
    result.m[0][0] = sx;
    result.m[1][1] = sy;
    return result;
  }

  static Mat3 rotationDegrees(float degrees) {
    const float radians = degrees * 3.1415926535f / 180.0f;
    const float cosine = std::cos(radians);
    const float sine = std::sin(radians);

    Mat3 result = identity();
    result.m[0][0] = cosine;
    result.m[0][1] = -sine;
    result.m[1][0] = sine;
    result.m[1][1] = cosine;
    return result;
  }

  Mat3 operator*(const Mat3 &rhs) const {
    Mat3 result;
    for (int row = 0; row < 3; ++row) {
      for (int col = 0; col < 3; ++col) {
        result.m[row][col] = 0.0f;
        for (int k = 0; k < 3; ++k) {
          result.m[row][col] += m[row][k] * rhs.m[k][col];
        }
      }
    }
    return result;
  }

  Vec2 apply(const Vec2 &value) const {
    const float x = m[0][0] * value.x + m[0][1] * value.y + m[0][2];
    const float y = m[1][0] * value.x + m[1][1] * value.y + m[1][2];
    const float w = m[2][0] * value.x + m[2][1] * value.y + m[2][2];
    if (std::fabs(w) < 1e-6f) {
      return {x, y};
    }
    return {x / w, y / w};
  }
};

inline Vec2 centroid(const std::vector<Vec2> &vertices) {
  if (vertices.empty()) {
    return {};
  }

  Vec2 sum{};
  for (const Vec2 &vertex : vertices) {
    sum += vertex;
  }
  return sum / static_cast<float>(vertices.size());
}

inline Vec2 clampVec2(const Vec2 &value, float minValue, float maxValue) {
  return {std::clamp(value.x, minValue, maxValue),
          std::clamp(value.y, minValue, maxValue)};
}

inline float distancePointToSegment(const Vec2 &point, const Vec2 &start,
                                    const Vec2 &end) {
  const Vec2 segment = end - start;
  const float segmentLengthSq = lengthSquared(segment);
  if (segmentLengthSq < 1e-6f) {
    return distance(point, start);
  }

  float t = dot(point - start, segment) / segmentLengthSq;
  t = std::clamp(t, 0.0f, 1.0f);
  const Vec2 projection = start + segment * t;
  return distance(point, projection);
}

inline bool pointInPolygon(const Vec2 &point,
                           const std::vector<Vec2> &polygon) {
  if (polygon.size() < 3) {
    return false;
  }

  bool inside = false;
  for (std::size_t i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++) {
    const Vec2 &a = polygon[i];
    const Vec2 &b = polygon[j];
    const bool intersects =
        ((a.y > point.y) != (b.y > point.y)) &&
        (point.x < (b.x - a.x) * (point.y - a.y) /
                           ((b.y - a.y) == 0.0f ? 1.0f : (b.y - a.y)) +
                       a.x);
    if (intersects) {
      inside = !inside;
    }
  }

  return inside;
}

} // namespace cg
