#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include "state.hpp"

using namespace std;

const string ESCAPE = "\033[";
const string CLEAR_SCREEN = ESCAPE + "2J";
const string ALT_SCREEN_ON = ESCAPE + "?1049h";
const string ALT_SCREEN_OFF = ESCAPE + "?1049l";
const string HIDE_CURSOR = ESCAPE + "?25l";
const string SHOW_CURSOR = ESCAPE + "?25h";

int termWidth, termHeight;
int bottomHeight;
int simHeight;
int statsWidth;
int cmdWidth;

struct termios orig_termios;

void disable_raw_mode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  struct termios raw = orig_termios;
  
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);
  
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 0;
  
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void move_cursor(int x, int y) {
  cout << ESCAPE << y << ";" << x << "H";
}

void move_cursor_cmd() {
  move_cursor(5, simHeight + 4);
}

void write_stats_tps() {
  move_cursor(cmdWidth + 8, simHeight + 3);
  cout << "      ";

  move_cursor(cmdWidth + 8, simHeight + 3);
  cout << State::get().tps;
}

void write_stats_particles() {
  move_cursor(cmdWidth + 14, simHeight + 4);
  cout << "      ";

  move_cursor(cmdWidth + 14, simHeight + 4);
  cout << State::get().activeParticles;
}

void write_stats_brush() {
  move_cursor(cmdWidth + 10, simHeight + 5);
  cout << "      ";

  move_cursor(cmdWidth + 10, simHeight + 5);
  cout << print_element(State::get().currentBrush);
}

void init_heights() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  termWidth = w.ws_col;
  termHeight = w.ws_row;

  // calculate box dimenstions
  bottomHeight = 7;
  simHeight = termHeight - bottomHeight;

  statsWidth = 25;
  cmdWidth = termWidth - statsWidth;
}

void draw_rectangle(int x, int y, int w, int h, const string& title) {
  // top
  move_cursor(x, y);
  cout << "┌";
  for (int i = 0; i < w - 2; ++i)
    cout << "─";
  cout << "┐";

  // title
  if (!title.empty() && w > title.length() + 4) {
    move_cursor(x + 2, y);
    cout << ' ' << title << ' ';
  }

  // sides
  for (int i = 1; i < h - 1; ++i) {
    move_cursor(x, y + i);
    cout << "│";

    move_cursor(x + w - 1, y + i);
    cout << "│";
  }
  
  // bottom
  move_cursor(x, y + h - 1);
  cout << "└";
  for (int i = 0; i < w - 2; ++i)
    cout << "─";
  cout << "┘";
}

int init_tui() {
  init_heights();

  if (termWidth < 50 || termHeight < 15) {
    cout << "Err: Terminal too small, resize and try again.";
    return -1;
  }

  cout << ALT_SCREEN_ON << HIDE_CURSOR << CLEAR_SCREEN;

  // draw layout
  draw_rectangle(1, 1, termWidth, simHeight, "");

  draw_rectangle(cmdWidth + 1, simHeight + 1, statsWidth, bottomHeight, "Stats");

  draw_rectangle(1, simHeight + 1, cmdWidth, bottomHeight, "Cmd");

  move_cursor(cmdWidth + 3, simHeight + 3); cout << "TPS: ";
  move_cursor(cmdWidth + 3, simHeight + 4); cout << "Particles: ";
  move_cursor(cmdWidth + 3, simHeight + 5); std::cout << "Brush: ";

  move_cursor(3, simHeight + 4);
  cout << "> ";

  cout << flush;
  enable_raw_mode();

  return 0;
}

void cleanup_tui() {
  cout << SHOW_CURSOR << ALT_SCREEN_OFF << flush;
  disable_raw_mode();
}

void write_cmd_buffer(const string& cmd) {
  move_cursor_cmd();

  int max_chars = cmdWidth - 5; 
  int spaces_to_clear = max_chars - cmd.length();
  for (int i = 0; i < spaces_to_clear; ++i)
      cout << " ";
}