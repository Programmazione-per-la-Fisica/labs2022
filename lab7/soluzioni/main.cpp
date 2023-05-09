#include "chain.hpp"

#include <iomanip>
#include <iostream>

// Esegue la simulazione della catena, suddividendo "duration" in "steps"
// e registrando lo stato ogni volta che si compie un numero di step pari
// a "prescale"
// NOTA: come descritto nella traccia del laboratorio (*) l'utilizzo della
// funzione libera simulate è una conseguenza dei limiti relativi al metodo
// di integrazione che abbiamo utilizzato.
// Pertanto, in generale (es.: nei progetti d'esame) **non serve in alcun modo** 
// utilizzare il medesimo approccio.
// (*) https://github.com/Programmazione-per-la-Fisica/labs2022/tree/main/lab7#un-main-program-che-utilizza-la-classe-chain

auto simulate(Chain& chain, double duration, int steps, int prescale) {
  std::vector<std::vector<ParticleState>> v_states;

  double delta_t{duration / steps};

  for (int step{0}; step != steps; ++step) {
    if (step % prescale == 0) {
      v_states.push_back(chain.state());
    }
    chain.evolve(delta_t);
  }

  return v_states;
}

int main() {
  // Molla con:
  // k = 1 N/m
  // l = 1 cm
  Hooke const hooke{1.0, 0.01};

  // Catena con 10 punti materiali:
  // m: 50 g per ogni punto
  // x: un punto ogni 1.1 cm (partendo da 0.0)
  // v: tutti i punti hanno v_0 = 0 m/s
  Chain chain(hooke);

  double const p_m{0.05};

  double p_x{0.0};
  double delta_x{0.011};

  for (auto const p_v : {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}) {
    chain.push_back({p_m, p_x, p_v});
    p_x += delta_x;
  }

  // Esegue una simulazione:
  // - per una durata di 20 s
  // - suddividendola in 20000 intervalli di tempo
  // - registrando lo stato della catena ogni 100 intervalli di tempo
  double const duration{20.0};
  int const steps{20000};
  int const prescale{100};

  const auto v_states = simulate(chain, duration, steps, prescale);

  // Analisi:
  double sum_length{0.0};

  std::cout << "Report for each of the stored states:\n";
  std::cout << "  length  : center of mass x : center of mass v\n";

  for (const auto& state : v_states) {
    auto length = state.back().x - state.front().x;
    sum_length += length;

    double sum_m{0.0};
    double sum_mx{0.0};
    double sum_mv{0.0};

    for (const auto& pps : state) {
      sum_m += pps.m;
      sum_mx += pps.m * pps.x;
      sum_mv += pps.m * pps.v;
    }

    std::cout << std::setprecision(4) << std::fixed << std::setw(8) << length
              << "  : " << std::setw(14) << (sum_mx / sum_m) << "   : "
              << std::setw(14) << (sum_mv / sum_m) << '\n';
  };

  std::cout << "\nSummary:\n";
  std::cout << "Average length: " << (sum_length / v_states.size()) << '\n';
}
