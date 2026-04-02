#include <iostream>
#include <thread>
#include <chrono>
#include "tui.hpp"
#include "state.hpp"
#include "input.hpp"
#include "timer.hpp"

using namespace std;

int main() {
  if (init_tui() == -1) {
    return 1;
  }

  bool running = true;
  string cmd_buffer = "";

  init_timer();

  while(running) {
    start_frame();

    process_input(running, cmd_buffer);

    write_stats_brush();
    write_stats_particles();
    write_stats_tps();

    State::get().activeParticles++;

    draw_grid();

    write_cmd_buffer(cmd_buffer);
    cout << flush;

    end_frame();
  }

  cleanup_tui();

  return 0;
}
