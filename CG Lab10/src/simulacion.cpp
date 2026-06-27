#include "../include/simulacion.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace {
float aleatorio_01() {
  return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

float aleatorio_rango(float min_val, float max_val) {
  return min_val + (max_val - min_val) * aleatorio_01();
}
} // namespace

SimulacionBoids::SimulacionBoids() { reiniciar(60); }

void SimulacionBoids::establecer_dimensiones(float ancho, float alto) {
  ancho_mundo_ = std::max(30.0f, ancho);
  alto_mundo_ = std::max(30.0f, alto);
}

void SimulacionBoids::reiniciar(int cantidad) {
  boids_.clear();
  cantidad = std::clamp(cantidad, cantidad_minima_, cantidad_maxima_);
  boids_.reserve(static_cast<size_t>(cantidad));

  for (int i = 0; i < cantidad; ++i) {
    boids_.push_back(crear_boid_aleatorio());
  }
}

void SimulacionBoids::cambiar_cantidad_boids(int delta) {
  int nueva_cantidad = cantidad_boids() + delta;
  nueva_cantidad =
      std::clamp(nueva_cantidad, cantidad_minima_, cantidad_maxima_);

  if (nueva_cantidad == cantidad_boids()) {
    return;
  }

  if (nueva_cantidad > cantidad_boids()) {
    const int a_agregar = nueva_cantidad - cantidad_boids();
    boids_.reserve(static_cast<size_t>(nueva_cantidad));
    for (int i = 0; i < a_agregar; ++i) {
      boids_.push_back(crear_boid_aleatorio());
    }
  } else {
    boids_.resize(static_cast<size_t>(nueva_cantidad));
  }
}

void SimulacionBoids::cambiar_radio_vecindad(float delta) {
  radio_vecindad_ = std::clamp(radio_vecindad_ + delta, 8.0f, 80.0f);
  radio_separacion_ =
      std::clamp(radio_vecindad_ * 0.45f, 5.0f, radio_vecindad_);
}

void SimulacionBoids::cambiar_peso_separacion(float delta) {
  peso_separacion_ = std::clamp(peso_separacion_ + delta, 0.0f, 4.0f);
}

void SimulacionBoids::cambiar_peso_alineamiento(float delta) {
  peso_alineamiento_ = std::clamp(peso_alineamiento_ + delta, 0.0f, 4.0f);
}

void SimulacionBoids::cambiar_peso_cohesion(float delta) {
  peso_cohesion_ = std::clamp(peso_cohesion_ + delta, 0.0f, 4.0f);
}

void SimulacionBoids::actualizar(float dt) {
  if (dt <= 0.0f) {
    return;
  }

  std::vector<Vec2> aceleraciones(boids_.size(), Vec2{});

  for (size_t i = 0; i < boids_.size(); ++i) {
    const Vec2 sep = calcular_separacion(static_cast<int>(i));
    const Vec2 ali = calcular_alineamiento(static_cast<int>(i));
    const Vec2 coh = calcular_cohesion(static_cast<int>(i));

    Vec2 aceleracion = sep * peso_separacion_ + ali * peso_alineamiento_ +
                       coh * peso_cohesion_;
    aceleracion = limitar_magnitud(aceleracion, fuerza_maxima_);
    aceleraciones[i] = aceleracion;
  }

  for (size_t i = 0; i < boids_.size(); ++i) {
    Boid &boid = boids_[i];
    boid.velocidad += aceleraciones[i] * dt;
    aplicar_limites_velocidad(boid.velocidad);

    boid.posicion += boid.velocidad * dt;
    aplicar_mundo_toroidal(boid.posicion);

    boid.direccion = normalizar(boid.velocidad);
  }
}

Vec2 SimulacionBoids::calcular_separacion(int indice) const {
  const Boid &actual = boids_[static_cast<size_t>(indice)];
  Vec2 acumulado{0.0f, 0.0f};
  int vecinos = 0;

  for (size_t j = 0; j < boids_.size(); ++j) {
    if (j == static_cast<size_t>(indice)) {
      continue;
    }

    const Vec2 diferencia = actual.posicion - boids_[j].posicion;
    const float distancia = magnitud(diferencia);
    if (distancia > 1e-6f && distancia < radio_separacion_) {
      // Mas cerca implica mayor repulsion.
      acumulado += normalizar(diferencia) / distancia;
      ++vecinos;
    }
  }

  if (vecinos == 0) {
    return Vec2{};
  }

  acumulado = acumulado / static_cast<float>(vecinos);
  return convertir_a_steering(acumulado, actual.velocidad);
}

Vec2 SimulacionBoids::calcular_alineamiento(int indice) const {
  const Boid &actual = boids_[static_cast<size_t>(indice)];
  Vec2 promedio_velocidad{0.0f, 0.0f};
  int vecinos = 0;

  for (size_t j = 0; j < boids_.size(); ++j) {
    if (j == static_cast<size_t>(indice)) {
      continue;
    }

    const float distancia = magnitud(actual.posicion - boids_[j].posicion);
    if (distancia < radio_vecindad_) {
      promedio_velocidad += boids_[j].velocidad;
      ++vecinos;
    }
  }

  if (vecinos == 0) {
    return Vec2{};
  }

  promedio_velocidad = promedio_velocidad / static_cast<float>(vecinos);
  return convertir_a_steering(promedio_velocidad, actual.velocidad);
}

Vec2 SimulacionBoids::calcular_cohesion(int indice) const {
  const Boid &actual = boids_[static_cast<size_t>(indice)];
  Vec2 centro_masa{0.0f, 0.0f};
  int vecinos = 0;

  for (size_t j = 0; j < boids_.size(); ++j) {
    if (j == static_cast<size_t>(indice)) {
      continue;
    }

    const float distancia = magnitud(actual.posicion - boids_[j].posicion);
    if (distancia < radio_vecindad_) {
      centro_masa += boids_[j].posicion;
      ++vecinos;
    }
  }

  if (vecinos == 0) {
    return Vec2{};
  }

  centro_masa = centro_masa / static_cast<float>(vecinos);
  const Vec2 hacia_centro = centro_masa - actual.posicion;
  return convertir_a_steering(hacia_centro, actual.velocidad);
}

Vec2 SimulacionBoids::convertir_a_steering(const Vec2 &direccion_deseada,
                                           const Vec2 &velocidad_actual) const {
  if (magnitud(direccion_deseada) <= 1e-6f) {
    return Vec2{};
  }

  const Vec2 velocidad_deseada =
      normalizar(direccion_deseada) * velocidad_maxima_;
  Vec2 fuerza = velocidad_deseada - velocidad_actual;
  fuerza = limitar_magnitud(fuerza, fuerza_maxima_);
  return fuerza;
}

void SimulacionBoids::aplicar_limites_velocidad(Vec2 &velocidad) const {
  const float rapidez = magnitud(velocidad);
  if (rapidez <= 1e-6f) {
    velocidad = Vec2{velocidad_minima_, 0.0f};
    return;
  }

  if (rapidez > velocidad_maxima_) {
    velocidad = normalizar(velocidad) * velocidad_maxima_;
  } else if (rapidez < velocidad_minima_) {
    velocidad = normalizar(velocidad) * velocidad_minima_;
  }
}

void SimulacionBoids::aplicar_mundo_toroidal(Vec2 &posicion) const {
  const float medio_ancho = ancho_mundo_ * 0.5f;
  const float medio_alto = alto_mundo_ * 0.5f;

  if (posicion.x > medio_ancho) {
    posicion.x = -medio_ancho;
  } else if (posicion.x < -medio_ancho) {
    posicion.x = medio_ancho;
  }

  if (posicion.y > medio_alto) {
    posicion.y = -medio_alto;
  } else if (posicion.y < -medio_alto) {
    posicion.y = medio_alto;
  }
}

Boid SimulacionBoids::crear_boid_aleatorio() const {
  const float medio_ancho = ancho_mundo_ * 0.5f;
  const float medio_alto = alto_mundo_ * 0.5f;

  const float angulo = aleatorio_rango(0.0f, 2.0f * static_cast<float>(M_PI));
  const Vec2 direccion{std::cos(angulo), std::sin(angulo)};
  const float rapidez = aleatorio_rango(velocidad_minima_, velocidad_maxima_);

  Boid boid;
  boid.posicion = Vec2{aleatorio_rango(-medio_ancho, medio_ancho),
                       aleatorio_rango(-medio_alto, medio_alto)};
  boid.velocidad = direccion * rapidez;
  boid.direccion = direccion;
  return boid;
}
