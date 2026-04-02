#pragma once

extern int termWidth, termHeight;
extern int bottomHeight;
extern int simHeight;
extern int statsWidth;
extern int cmdWidth;

int init_tui();
void cleanup_tui();

void move_cursor(int x, int y);
void move_cursor_cmd();

void write_stats_tps();
void write_stats_particles();
void write_stats_brush();

void write_cmd_buffer(const std::string& cmd);
