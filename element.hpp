#pragma once
#include <string>

enum class Element {
  Empty,
  Stone,
  Water,
  Lava,
  Steam,
  NONE, // inteneded for handling the borders
};

inline std::string print_element(Element e) {
  switch(e) {
    case Element::Empty:
      return "Empty";
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
