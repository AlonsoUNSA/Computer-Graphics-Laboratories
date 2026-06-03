#include "model/algorithms.hpp"

#include <algorithm>
#include <cmath>

namespace cg {

std::vector<RasterPoint> rasterizeLineBresenham(const Vec2 &start,
                                                const Vec2 &end) {
  std::vector<RasterPoint> points;

  int x0 = static_cast<int>(std::round(start.x));
  int y0 = static_cast<int>(std::round(start.y));
  const int x1 = static_cast<int>(std::round(end.x));
  const int y1 = static_cast<int>(std::round(end.y));

  const int dx = std::abs(x1 - x0);
  const int sx = x0 < x1 ? 1 : -1;
  const int dy = -std::abs(y1 - y0);
  const int sy = y0 < y1 ? 1 : -1;
  int error = dx + dy;

  while (true) {
    points.push_back(makeVec2(static_cast<float>(x0), static_cast<float>(y0)));
    if (x0 == x1 && y0 == y1) {
      break;
    }

    const int doubledError = 2 * error;
    if (doubledError >= dy) {
      error += dy;
      x0 += sx;
    }
    if (doubledError <= dx) {
      error += dx;
      y0 += sy;
    }
  }

  return points;
}

std::vector<ScanlineSpan> buildScanlineFill(const std::vector<Vec2> &polygon) {
  std::vector<ScanlineSpan> spans;
  if (polygon.size() < 3) {
    return spans;
  }

  float minY = polygon.front().y;
  float maxY = polygon.front().y;
  for (const Vec2 &vertex : polygon) {
    minY = std::min(minY, vertex.y);
    maxY = std::max(maxY, vertex.y);
  }

  const int startY = static_cast<int>(std::floor(minY));
  const int endY = static_cast<int>(std::ceil(maxY));

  for (int y = startY; y <= endY; ++y) {
    std::vector<float> intersections;
    for (std::size_t i = 0; i < polygon.size(); ++i) {
      const std::size_t next = (i + 1) % polygon.size();
      const Vec2 &a = polygon[i];
      const Vec2 &b = polygon[next];
      const bool crosses = (a.y <= y && b.y > y) || (b.y <= y && a.y > y);
      if (!crosses) {
        continue;
      }

      const float t = (y - a.y) / (b.y - a.y);
      intersections.push_back(a.x + t * (b.x - a.x));
    }

    std::sort(intersections.begin(), intersections.end());
    for (std::size_t index = 0; index + 1 < intersections.size(); index += 2) {
      spans.emplace_back(
          makeVec2(intersections[index], static_cast<float>(y)),
          makeVec2(intersections[index + 1], static_cast<float>(y)));
    }
  }

  return spans;
}

} // namespace cg
