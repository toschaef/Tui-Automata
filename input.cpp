#include <unistd.h>
#include <sstream>
#include <cmath>
#include "state.hpp"
#include "tui.hpp"

using namespace std;

void process_input(bool& running, string& cmd_buffer) {
  static string input_queue = ""; 

  char buf[4096];
  int n;
  while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
    input_queue.append(buf, n);
  }

  while (!input_queue.empty()) {
    char c = input_queue[0];

    // mouse click
    if (c == '\033') {
      if (input_queue.length() >= 3 && input_queue[1] == '[' && input_queue[2] == '<') {
        size_t m_idx = input_queue.find_first_of("Mm", 3);
        
        if (m_idx == string::npos) {
          if (input_queue.length() > 32) input_queue.erase(0, 1);
          break;
        }

        string mouse_data = input_queue.substr(3, m_idx - 3);
        char end_char = input_queue[m_idx];
        input_queue.erase(0, m_idx + 1);

        for (char& ch : mouse_data) {
          if (ch == ';') ch = ' ';
        }
        
        stringstream ss(mouse_data);
        int cb = 0, cx = 0, cy = 0;
        ss >> cb >> cx >> cy;

        State::get().mouseX = cx;
        State::get().mouseY = cy;

        int gridX = cx - 2; 
        int gridY = (cy - 2) * 2; 

        if ((cb == 0 || cb == 32) && end_char == 'M') {
          if (gridX >= 0 && gridX < State::get().gridWidth && cy > 1 && cy < simHeight) {
            Element brush = State::get().currentBrush;
            if (cb == 0) draw_line(gridX, gridY, gridX, gridY, brush);
            else if (cb == 32) {
              if (State::get().lastGridX != -1) {
                draw_line(State::get().lastGridX, State::get().lastGridY, gridX, gridY, brush);
              } else {
                draw_line(gridX, gridY, gridX, gridY, brush);
              }
            }
            State::get().lastGridX = gridX;
            State::get().lastGridY = gridY;
          }
        } else {
          State::get().lastGridX = -1;
          State::get().lastGridY = -1;
        }
        continue; 
      } else {
        input_queue.erase(0, 1);
        continue;
      }
    }

    input_queue.erase(0, 1);

    // newline
    if (c == '\r' || c == '\n') {
      stringstream ss(cmd_buffer);
      string action;
      
      if (ss >> action) { 
        if (action == "quit" || action == "q") {
          running = false;
        }
        else if (action == "brush" || action == "b") {
          string material;

          if (ss >> material) {
            if (material == "stone") State::get().currentBrush = Element::Stone;
            else if (material == "water") State::get().currentBrush = Element::Water;
            else if (material == "lava") State::get().currentBrush = Element::Lava;
            else if (material == "steam") State::get().currentBrush = Element::Steam;
          }
        }
        else if (action == "tps" || action == "t") {
          int tps;

          if (ss >> tps) {
            State::get().targetTps = tps;
          }
        }
        // else if (action == "clear" || action == "c") { // fix this later
        //   State::get().grid.clear();
        // }
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