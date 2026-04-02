#include "input.hpp"
#include <unistd.h>
#include <sstream>
#include "state.hpp"
#include "tui.hpp"

using namespace std;

void process_input(bool& running, string& cmd_buffer) {
  char c = '\0';

  if (read(STDIN_FILENO, &c, 1) == 1) {
    
    // newline
    if (c == '\r' || c == '\n') {
      stringstream ss(cmd_buffer);
      string action;
      
      if (ss >> action) { 
        if (action == "quit" || action == "q") {
          running = false;
        } 
        else if (action == "brush") {
          string material;

          if (ss >> material) {
            if (material == "stone") State::get().currentBrush = Element::Stone;
            else if (material == "water") State::get().currentBrush = Element::Water;
            else if (material == "lava") State::get().currentBrush = Element::Lava;
            else if (material == "steam") State::get().currentBrush = Element::Steam;
          }
        }
        // else if (action == "clear") {}
        // else if (action == tps") {}
      }

      cmd_buffer = "";
    }
    // backspace
    else if (c == 127 || c == '\b') {
      if (!cmd_buffer.empty()) {
          cmd_buffer.pop_back(); 
      }
    }
    // printable chars
    else if (c >= 32 && c <= 126) {
      if (cmd_buffer.length() < (size_t)(cmdWidth - 6)) {
          cmd_buffer += c;
      }
    }
  }
}