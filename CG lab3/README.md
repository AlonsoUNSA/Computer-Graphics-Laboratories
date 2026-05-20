# Lab 3: Transformaciones

Este repositorio contiene cuatro ejercicios en C++ usando OpenGL y GLUT/freeglut.

## Dependencias

En Linux, instala primero las herramientas y bibliotecas necesarias:

```bash
sudo apt update
sudo apt install build-essential freeglut3-dev mesa-common-dev libglu1-mesa-dev
```

Notas:

- `build-essential` incluye `g++` y las utilidades básicas de compilación.
- `freeglut3-dev` instala los encabezados y bibliotecas de GLUT.
- `mesa-common-dev` y `libglu1-mesa-dev` proporcionan soporte para OpenGL y GLU.

## Compilación

Cada ejercicio se compila como un ejecutable independiente. Desde la carpeta del proyecto, ejecuta:

```bash
g++ lab3.1.cpp -o lab3.1 -lGL -lGLU -lglut
g++ lab3.2.cpp -o lab3.2 -lGL -lGLU -lglut
g++ lab3.3.cpp -o lab3.3 -lGL -lGLU -lglut
g++ lab3.4.cpp -o lab3.4 -lGL -lGLU -lglut
```

Si tu distribución usa `freeglut` directamente, reemplaza `-lglut` por `-lfreeglut`.

## Ejecutables generados

- `lab3.1.cpp` -> `lab3.1`
- `lab3.2.cpp` -> `lab3.2`
- `lab3.3.cpp` -> `lab3.3`
- `lab3.4.cpp` -> `lab3.4`

## Ejecución

Después de compilar, ejecuta cada ejercicio con el binario correspondiente:

```bash
./lab3.1
./lab3.2
./lab3.3
./lab3.4
```

## Controles

- `Esc`: cierra la ventana.
- Si la ventana no aparece o la compilación falla, verifica que las dependencias de OpenGL/GLUT estén instaladas correctamente.
