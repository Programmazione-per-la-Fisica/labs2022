#ifndef CHAIN_HPP
#define CHAIN_HPP

#include <vector>
#include <ostream>

#include "hooke.hpp"

// Opzionale, utile in fase di sviluppo
std::ostream &operator<<(std::ostream &, ParticleState const &);

class Chain {
 private:
  Hooke m_inter;
  std::vector<ParticleState> m_particle_states;

  ParticleState solve(ParticleState const &, double, double const) const;

 public:
  Chain(Hooke const &);

  bool empty();

  std::size_t size() const;

  void push_back(ParticleState const &);

  std::vector<ParticleState> const &state() const;

  void evolve(double const);
};

#endif
