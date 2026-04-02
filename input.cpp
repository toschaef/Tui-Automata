#include "input.hpp"
#include <unistd.h>
#include <sstream>
#include "state.hpp"
#include "tui.hpp"

using namespace std;

void process_input(bool& running, string& cmd_buffer) {
  char c = '\0';

  while (read(STDIN_FILENO, &c, 1) == 1) {
    
    // mouse click
    if (c == '\033') {
      char seq[32];
      if (read(STDIN_FILENO, &seq[0], 1) == 1 && seq[0] == '[') {
        if (read(STDIN_FILENO, &seq[1], 1) == 1 && seq[1] == '<') {
          string mouse_data = "";
          char m_char;
          while (read(STDIN_FILENO, &m_char, 1) == 1) {
            mouse_data += m_char;
            if (m_char == 'M' || m_char == 'm') break;
          }

          int cb = 0, cx = 0, cy = 0;
          char end_char;

          for (char& ch : mouse_data) {
            if (ch == ';') ch = ' ';
          } 
          
          stringstream ss(mouse_data);
          ss >> cb >> cx >> cy >> end_char;

          int gridX = cx - 1;
          int gridY = (cy - 1) * 2; 

          if ((cb == 0 || cb == 32) && end_char == 'M') {
            if (cy <= simHeight) {
              Element brush = State::get().currentBrush;

              if (brush == Element::Stone) {
                State::get().set_pixel(gridX, gridY, brush);
                State::get().set_pixel(gridX, gridY + 1, brush);
              }
              else {
                State::get().set_pixel(gridX, gridY, brush);
              }
            }
          }
        }
      }
      continue;
    }
    // newline
    else if (c == '\r' || c == '\n') {
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