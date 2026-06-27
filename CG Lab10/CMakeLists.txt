cmake_minimum_required(VERSION 3.16)
project(CG_Lab10_Boids LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(cg_lab10_boids
  main.cpp
  src/simulacion.cpp
)

target_include_directories(cg_lab10_boids PRIVATE include)

if(APPLE)
  find_library(OPENGL_FRAMEWORK OpenGL REQUIRED)
  find_library(GLUT_FRAMEWORK GLUT REQUIRED)

  target_link_libraries(cg_lab10_boids
    PRIVATE
      ${OPENGL_FRAMEWORK}
      ${GLUT_FRAMEWORK}
  )
else()
  find_package(OpenGL REQUIRED)
  find_package(GLUT REQUIRED)
  target_link_libraries(cg_lab10_boids PRIVATE OpenGL::GL GLUT::GLUT)
endif()
