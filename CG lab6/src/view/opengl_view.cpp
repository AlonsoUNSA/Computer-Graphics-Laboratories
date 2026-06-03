#include "view/opengl_view.hpp"

#include <GL/freeglut.h>

namespace cg {

void OpenGLView::reshape(int width, int height) const {
  if (height <= 0) {
    height = 1;
  }

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, static_cast<double>(width), 0.0, static_cast<double>(height),
          -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void OpenGLView::drawPoint(const Vec2 &point, const Color &color,
                           float size) const {
  glPointSize(size);
  glColor3f(color.r, color.g, color.b);
  glBegin(GL_POINTS);
  glVertex2f(point.x, point.y);
  glEnd();
}

void OpenGLView::drawPolyline(const std::vector<Vec2> &vertices,
                              const Color &color, bool closed,
                              float thickness) const {
  if (vertices.size() < 2) {
    return;
  }

  glLineWidth(thickness);
  glColor3f(color.r, color.g, color.b);
  glBegin(GL_POINTS);

  const std::size_t segmentCount =
      closed ? vertices.size() : vertices.size() - 1;
  for (std::size_t i = 0; i < segmentCount; ++i) {
    const std::size_t next = (i + 1) % vertices.size();
    const std::vector<RasterPoint> pixels =
        rasterizeLineBresenham(vertices[i], vertices[next]);
    for (const RasterPoint &pixel : pixels) {
      glVertex2f(pixel.x, pixel.y);
    }
  }

  glEnd();
}

void OpenGLView::drawFilledPolygon(const std::vector<Vec2> &vertices,
                                   const Color &color) const {
  const std::vector<ScanlineSpan> spans = buildScanlineFill(vertices);

  glColor3f(color.r, color.g, color.b);
  glBegin(GL_LINES);
  for (const ScanlineSpan &span : spans) {
    glVertex2f(span.first.x, span.first.y);
    glVertex2f(span.second.x, span.second.y);
  }
  glEnd();
}

void OpenGLView::drawHighlight(const GraphicObject &object) const {
  glColor3f(1.0f, 0.95f, 0.2f);
  glLineWidth(3.0f);
  glPointSize(10.0f);

  if (object.type == ShapeType::Point) {
    glBegin(GL_POINTS);
    glVertex2f(object.vertices.front().x, object.vertices.front().y);
    glEnd();
    return;
  }

  const bool closed = object.type == ShapeType::Polygon;
  glBegin(closed ? GL_LINE_LOOP : GL_LINE_STRIP);
  for (const Vec2 &vertex : object.vertices) {
    glVertex2f(vertex.x, vertex.y);
  }
  glEnd();
}

void OpenGLView::drawObject(const GraphicObject &object, bool selected,
                            bool draft) const {
  if (object.vertices.empty()) {
    return;
  }

  if (object.type == ShapeType::Point) {
    drawPoint(object.vertices.front(),
              selected ? makeColor(1.0f, 0.95f, 0.2f) : object.outlineColor,
              draft ? 9.0f : (selected ? 12.0f : 8.0f));
    return;
  }

  if (object.type == ShapeType::Polygon && object.filled) {
    drawFilledPolygon(object.vertices, object.fillColor);
  }

  const bool closed = object.type == ShapeType::Polygon;
  drawPolyline(object.vertices,
               selected ? makeColor(1.0f, 0.95f, 0.2f) : object.outlineColor,
               closed, draft ? 2.0f : (selected ? 3.0f : 1.5f));

  if (selected) {
    drawHighlight(object);
  }
}

void OpenGLView::render(const Scene &scene, const GraphicObject *draft,
                        int width, int height) const {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, static_cast<double>(width), 0.0, static_cast<double>(height),
          -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0.08f, 0.09f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  for (const GraphicObject &object : scene.objects()) {
    drawObject(object, false, false);
  }

  if (draft != nullptr) {
    drawObject(*draft, false, true);
  }
}

} // namespace cg
