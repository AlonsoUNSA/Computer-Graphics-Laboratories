# CG Lab10 - Simulacion de Boids (2D)

Este proyecto implementa una simulacion de comportamiento colectivo (Boids) en 2D usando OpenGL + GLUT, con mundo toroidal y parametros ajustables en tiempo real.

## Requisitos

- macOS
- CMake 3.16 o superior
- GLUT y OpenGL (frameworks del sistema)

## Compilacion (CMake)

```bash
cd "CG Lab10"
cmake -S . -B build
cmake --build build
```

## Ejecucion

```bash
./build/cg_lab10_boids
```

## Controles

- `N` / `M`: aumentar / disminuir numero de boids.
- `R` / `F`: aumentar / disminuir radio de vecindad.
- `Q` / `A`: aumentar / disminuir peso de separacion.
- `W` / `S`: aumentar / disminuir peso de alineamiento.
- `E` / `D`: aumentar / disminuir peso de cohesion.
- `ESPACIO`: pausar o reanudar simulacion.
- `C`: reiniciar boids manteniendo cantidad actual.
- `ESC`: salir.

## Parametros iniciales

- 60 boids iniciales.
- Radio de vecindad estable para observar flocking sin colapso.
- Peso de separacion mas alto que alineamiento y cohesion para evitar choques.
- Limites de velocidad minima y maxima para estabilidad visual.
- Bordes con mundo toroidal (sale por un lado y reaparece al opuesto).

## Explicacion del codigo

- `main.cpp`
  - Configura ventana GLUT y callbacks.
  - Dibuja cada boid como triangulo 2D orientado por su direccion.
  - Gestiona teclado y muestra HUD con valores actuales.
  - Calcula `dt` por frame y actualiza simulacion.

- `include/vec2.hpp`
  - Vector 2D basico (`Vec2`) y utilidades: magnitud, normalizacion y limite de magnitud.

- `include/boid.hpp`
  - Estructura `Boid` con posicion, velocidad y direccion.

- `include/simulacion.hpp` y `src/simulacion.cpp`
  - Clase `SimulacionBoids`.
  - Reglas de Reynolds:
    - Separacion: evita colisiones con vecinos cercanos.
    - Alineamiento: acerca velocidad al promedio vecinal.
    - Cohesion: dirige al boid hacia centro de masa local.
  - Combinacion de fuerzas con pesos ajustables.
  - Aplicacion de velocidad minima/maxima.
  - Envoltura toroidal de posiciones.

## Cobertura de requerimientos del laboratorio

1. Estructura de boid con posicion, velocidad y direccion: cumplido.
2. Poblacion inicial >= 50: cumplido (60).
3. Representacion visual orientada: cumplido (triangulos orientados).
4. Movimiento continuo y reglas separacion/alineamiento/cohesion: cumplido.
5. Combinacion con ponderaciones ajustables: cumplido.
6. Limites de velocidad minima y maxima: cumplido.
7. Comportamiento de bordes: cumplido (mundo toroidal).
8. Modificacion en tiempo real de parametros solicitados: cumplido.
