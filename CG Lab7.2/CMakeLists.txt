cmake_minimum_required(VERSION 3.16)

project(cg_lab7_editor3d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_path(FREEGLUT_INCLUDE_DIR
  NAMES GL/freeglut.h
  HINTS /opt/homebrew/opt/freeglut/include /usr/local/opt/freeglut/include
  PATHS /opt/homebrew/include /usr/local/include
)

find_library(FREEGLUT_LIBRARY
  NAMES glut freeglut
  HINTS /opt/homebrew/opt/freeglut/lib /usr/local/opt/freeglut/lib
  PATHS /opt/homebrew/lib /usr/local/lib
)

find_package(OpenGL REQUIRED)

if(NOT FREEGLUT_INCLUDE_DIR OR NOT FREEGLUT_LIBRARY)
  message(FATAL_ERROR "freeglut no encontrado. Instala freeglut con Homebrew: brew install freeglut")
endif()

add_executable(cg_lab7_editor3d
  src/main.cpp
  src/model/scene.cpp
  src/presenter/editor_presenter.cpp
  src/view/opengl_view.cpp
)

target_include_directories(cg_lab7_editor3d PRIVATE
  include
  ${FREEGLUT_INCLUDE_DIR}
)

target_link_libraries(cg_lab7_editor3d PRIVATE
  ${FREEGLUT_LIBRARY}
  OpenGL::GL
)

if(TARGET OpenGL::GLU)
  target_link_libraries(cg_lab7_editor3d PRIVATE OpenGL::GLU)
endif()

if(APPLE)
  target_compile_definitions(cg_lab7_editor3d PRIVATE GL_SILENCE_DEPRECATION)
endif()