#ifndef CG_LAB10_SIMULACION_HPP
#define CG_LAB10_SIMULACION_HPP

#include <vector>

#include "boid.hpp"

class SimulacionBoids {
public:
  SimulacionBoids();

  void establecer_dimensiones(float ancho, float alto);
  void reiniciar(int cantidad);
  void actualizar(float dt);

  void cambiar_cantidad_boids(int delta);
  void cambiar_radio_vecindad(float delta);
  void cambiar_peso_separacion(float delta);
  void cambiar_peso_alineamiento(float delta);
  void cambiar_peso_cohesion(float delta);

  const std::vector<Boid> &boids() const { return boids_; }

  int cantidad_boids() const { return static_cast<int>(boids_.size()); }
  float radio_vecindad() const { return radio_vecindad_; }
  float peso_separacion() const { return peso_separacion_; }
  float peso_alineamiento() const { return peso_alineamiento_; }
  float peso_cohesion() const { return peso_cohesion_; }

private:
  Vec2 calcular_separacion(int indice) const;
  Vec2 calcular_alineamiento(int indice) const;
  Vec2 calcular_cohesion(int indice) const;
  Vec2 convertir_a_steering(const Vec2 &direccion_deseada,
                            const Vec2 &velocidad_actual) const;

  void aplicar_limites_velocidad(Vec2 &velocidad) const;
  void aplicar_mundo_toroidal(Vec2 &posicion) const;
  Boid crear_boid_aleatorio() const;

  std::vector<Boid> boids_;
  float ancho_mundo_ = 120.0f;
  float alto_mundo_ = 120.0f;

  float radio_vecindad_ = 24.0f;
  float radio_separacion_ = 12.0f;
  float peso_separacion_ = 1.8f;
  float peso_alineamiento_ = 1.0f;
  float peso_cohesion_ = 0.9f;

  float velocidad_minima_ = 18.0f;
  float velocidad_maxima_ = 52.0f;
  float fuerza_maxima_ = 28.0f;

  int cantidad_minima_ = 10;
  int cantidad_maxima_ = 300;
};

#endif
