# Laboratorios de Computación Gráfica

Este repositorio contiene varios ejercicios en C++ que utilizan OpenGL y GLUT/freeglut.

## Dependencias

En Linux, instala las herramientas y bibliotecas necesarias:

```bash
sudo apt update
sudo apt install build-essential freeglut3-dev mesa-common-dev libglu1-mesa-dev
```

Notas:

- build-essential incluye `g++`.
- freeglut3-dev instala los encabezados y bibliotecas de GLUT.
- mesa-common-dev y libglu1-mesa-dev proporcionan soporte para OpenGL y GLU.

## Compilación (genérica)

Para compilar cualquier archivo `.cpp` del proyecto, usa la siguiente sintaxis:

```bash
g++ -std=c++17 -O2 -o <nombre_ejecutable> <archivo.cpp> -lGL -lGLU -lglut
```

Ejemplo:

```bash
g++ -std=c++17 -O2 -o lab4 lab4.7.1.cpp -lGL -lGLU -lglut
```

Para compilar todos los archivos `.cpp` en un único ejecutable (si tiene sentido para tu proyecto):

```bash
g++ -std=c++17 -O2 -o programa *.cpp -lGL -lGLU -lglut
```

Para compilar cada `.cpp` en su propio ejecutable con el mismo nombre base:

```bash
for f in *.cpp; do g++ -std=c++17 -O2 -o "${f%.cpp}" "$f" -lGL -lGLU -lglut; done
```

Si tu sistema utiliza `freeglut` en lugar de `glut`, reemplaza `-lglut` por `-lfreeglut`.

## Ejecución

Después de compilar, ejecuta el binario generado:

```bash
./<nombre_ejecutable>
```

Ejemplo:

```bash
./lab4
```

## Controles y notas

- Escape (`Esc`) normalmente cierra la ventana de la aplicación.
- Si la compilación falla, revisa que las dependencias estén instaladas y que los archivos fuente no requieran flags adicionales.
- Si necesitas un flujo de compilación más avanzado, considera añadir un `Makefile` o `CMakeLists.txt`.

---

Si quieres, puedo añadir un `Makefile` o instrucciones específicas para ejecutar conjuntos de ejercicios.
