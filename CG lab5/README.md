# CG Lab5 — Transformaciones y cámaras (OpenGL + GLUT)

Este directorio contiene varios ejercicios en C++ usando OpenGL/GLU y GLUT/freeglut. Los archivos incluidos son ejemplos de transformaciones, jerarquía y control de cámara.

## Archivos en esta carpeta

- `lab4.0.cpp` : Ejemplo de transformaciones básicas (cubos y proyección perspectiva/ortográfica).
- `lab5.0.cpp` : Escena con varios objetos y transformaciones; incluye controles para mover la cámara (`WASD` y `Q/E` para zoom).
- `lab5.2.cpp` : Variante con proyección ortográfica y objetos estáticos desplazados (práctica de transformaciones compuestas).
- `lab5.3.cpp` : Uso de `gluLookAt` con parámetros controlables por teclado (`WASD`, `Q/E`).
- `lab5.4.cpp` : Sistema simple tipo "sol-planeta-luna" que combina rotaciones y traslaciones (útil para practicar seguimiento de objetos por la cámara).
- `app/` : (si existe) carpeta con recursos auxiliares o ejecutables relacionados.

## Dependencias

Instala las bibliotecas necesarias en Linux:

```bash
sudo apt update
sudo apt install build-essential freeglut3-dev mesa-common-dev libglu1-mesa-dev
```

## Compilación

Compila cada ejercicio por separado desde esta carpeta:

```bash
g++ lab4.0.cpp -o lab4.0 -lGL -lGLU -lglut
g++ lab5.0.cpp -o lab5.0 -lGL -lGLU -lglut
g++ lab5.2.cpp -o lab5.2 -lGL -lGLU -lglut
g++ lab5.3.cpp -o lab5.3 -lGL -lGLU -lglut
g++ lab5.4.cpp -o lab5.4 -lGL -lGLU -lglut
```

Si tu sistema usa `freeglut` explícitamente, sustituye `-lglut` por `-lfreeglut`.

## Ejecución

Después de compilar, ejecuta el binario deseado:

```bash
./lab5.4
```

## Notas útiles

- Muchos ejercicios usan ángulos en grados con `glRotatef`. Ten en cuenta que las funciones `sin()`/`cos()` de C/C++ esperan radianes si las usas para cálculos manuales.
- Si quieres que la cámara siga un objeto que se mueve mediante una pila de transformaciones, obtén su posición en coordenadas mundo (calculando la composición de transformaciones o leyendo la matriz modelo) en lugar de recomputar la posición con fórmulas separadas.
- Controles comunes en estos archivos:
  - `Esc` : salir
  - `W`/`A`/`S`/`D` : mover cámara/eye en algunos ejemplos
  - `Q`/`E` : zoom en algunos ejemplos

Si quieres, puedo adaptar `lab5.4.cpp` para que la cámara siga la posición real de la Tierra (extraer la posición del modelo), ¿quieres que lo haga?
