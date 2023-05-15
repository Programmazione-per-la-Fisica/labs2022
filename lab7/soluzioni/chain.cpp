#include "chain.hpp"

#include <cmath>
#include <stdexcept>
#include <vector>

// Opzionale, utile in fase di sviluppo
std::ostream &operator<<(std::ostream &os, ParticleState const &p) {
  os << "(m: " << p.m << ", x: " << p.x << ", v: " << p.v << ')';
  return os;
}

ParticleState Chain::solve(ParticleState const &state, double f,
                           double const delta_t) const {
  auto const a = f / state.m;
  auto const v = state.v + a * delta_t;
  auto const x = state.x + (state.v + 0.5 * a * delta_t) * delta_t;

  return ParticleState{state.m, x, v};
}

Chain::Chain(Hooke const &inter) : m_inter(inter){};

bool Chain::empty() { return Chain::m_particle_states.empty(); }

std::size_t Chain::size() const { return Chain::m_particle_states.size(); }

void Chain::push_back(ParticleState const &state) {
  if (!empty() && state.x <= Chain::m_particle_states.back().x) {
    throw std::runtime_error{
        "Added ParticleState x must be larger than x of rightmost chain's "
        "ParticleState"};
  }
  Chain::m_particle_states.push_back(state);
}

std::vector<ParticleState> const &Chain::state() const {
  return Chain::m_particle_states;
}

void Chain::evolve(double const delta_t) {
  auto state_it = Chain::m_particle_states.begin();
  auto state_it_next = std::next(state_it);
  auto state_last = std::prev(Chain::m_particle_states.end());

  double f_prev{0.0};

  for (; state_it != state_last; ++state_it, ++state_it_next) {
    double f = Chain::m_inter(*state_it, *state_it_next);
    *state_it = solve(*state_it, f - f_prev, delta_t);
    f_prev = f;
  }

  *state_last = solve(*state_last, -f_prev, delta_t);
}