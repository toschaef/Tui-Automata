#include <iostream>
#include <thread>
#include <chrono>
#include "tui.hpp"
#include "state.hpp"

using namespace std;

int main() {
  if (init_tui() == -1) {
    return 1;
  }

  while(1) {
    write_stats_brush();
    write_stats_particles();
    write_stats_tps();

    cout << flush;

    std::this_thread::sleep_for(std::chrono::milliseconds(33));
  }

  cleanup_tui();

  return 0;
}
