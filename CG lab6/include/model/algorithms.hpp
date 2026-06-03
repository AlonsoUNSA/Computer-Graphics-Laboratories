#pragma once

#include "model/geometry.hpp"

#include <utility>
#include <vector>

namespace cg {

using RasterPoint = Vec2;
using ScanlineSpan = std::pair<RasterPoint, RasterPoint>;

std::vector<RasterPoint> rasterizeLineBresenham(const Vec2 &start,
                                                const Vec2 &end);
std::vector<ScanlineSpan> buildScanlineFill(const std::vector<Vec2> &polygon);

} // namespace cg
