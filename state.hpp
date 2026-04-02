#pragma once
#include <vector>
#include "element.hpp"

class State {
public:
  int tps = 30;
  int activeParticles = 0;
  Element currentBrush = Element::Stone;

  int gridWidth = 0;
  int gridHeight = 0;
  std::vector<Element> grid;

  int mouseX = -1;
  int mouseY = -1;

  Element get_pixel(int x, int y) {
    if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) return Element::NONE;
    return grid[y * gridWidth + x];
  }

  void set_pixel(int x, int y, Element type) {
    if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
      grid[y * gridWidth + x] = type;
    }
  }

  static State& get() {
    static State instance;
    return instance;
  }

private:
  State() {} 
};
