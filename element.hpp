#pragma once
#include <string>

enum class Element {
  Stone,
  Water,
  Lava,
  Steam,
};

inline std::string print_element(Element e) {
  switch(e) {
    case Element::Stone:
      return "Stone";
    case Element::Water:
      return "Water";
    case Element::Lava:
      return "Lava";
    case Element::Steam:
      return "Steam";
  }
}
