#pragma once
#include "element.hpp"

class State {
public:
  int tps = 30;
  int activeParticles = 0;
  Element currentBrush = Element::Stone;

  static State& get() {
    static State instance;
    return instance;
  }

private:
  State() {} 
};
