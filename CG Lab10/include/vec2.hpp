#ifndef CG_LAB10_VEC2_HPP
#define CG_LAB10_VEC2_HPP

#include <cmath>

struct Vec2 {
  float x = 0.0f;
  float y = 0.0f;

  Vec2() = default;
  Vec2(float x_, float y_) : x(x_), y(y_) {}

  Vec2 operator+(const Vec2 &otro) const {
    return Vec2{x + otro.x, y + otro.y};
  }
  Vec2 operator-(const Vec2 &otro) const {
    return Vec2{x - otro.x, y - otro.y};
  }
  Vec2 operator*(float escalar) const { return Vec2{x * escalar, y * escalar}; }
  Vec2 operator/(float escalar) const { return Vec2{x / escalar, y / escalar}; }

  Vec2 &operator+=(const Vec2 &otro) {
    x += otro.x;
    y += otro.y;
    return *this;
  }

  Vec2 &operator-=(const Vec2 &otro) {
    x -= otro.x;
    y -= otro.y;
    return *this;
  }

  Vec2 &operator*=(float escalar) {
    x *= escalar;
    y *= escalar;
    return *this;
  }
};

inline float magnitud(const Vec2 &v) {
  return std::sqrt(v.x * v.x + v.y * v.y);
}

inline Vec2 normalizar(const Vec2 &v) {
  const float m = magnitud(v);
  if (m <= 1e-6f) {
    return Vec2{0.0f, 0.0f};
  }
  return v / m;
}

inline Vec2 limitar_magnitud(const Vec2 &v, float limite) {
  const float m = magnitud(v);
  if (m > limite && m > 1e-6f) {
    return v * (limite / m);
  }
  return v;
}

#endif
