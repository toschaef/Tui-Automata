#include "state.hpp"
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;

static high_resolution_clock::time_point frame_start;
static high_resolution_clock::time_point last_time;
static double tps_timer = 0.0;
static int frames_this_second = 0;

void init_timer() {
  last_time = high_resolution_clock::now();
}

void start_frame() {
  frame_start = high_resolution_clock::now();
}

void end_frame() {
  auto work_end = high_resolution_clock::now();
  duration<double, milli> work_time = work_end - frame_start;
  
  double target_ms = 1000.0 / State::get().targetTps; 

  if (work_time.count() < target_ms) {
    duration<double, milli> sleep_duration(target_ms - work_time.count());
    this_thread::sleep_for(sleep_duration);
  }

  auto current_time = high_resolution_clock::now();
  duration<double> total_frame_time = current_time - last_time; 
  last_time = current_time;

  tps_timer += total_frame_time.count();
  frames_this_second++;

  if (tps_timer >= 1.0) {
    State::get().actualTps = frames_this_second;
    
    frames_this_second = 0;
    tps_timer = 0.0;
  }
}