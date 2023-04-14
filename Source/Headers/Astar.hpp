#pragma once

float calculate_h_score(const gbl::Position<>& i_cell_0, const gbl::Position<>& i_cell_1);

bool astar_search(unsigned short& i_path_length, unsigned short& i_total_checks, std::chrono::microseconds& i_duration, std::map<gbl::Position<>, gbl::Position<>>& i_previous_cell, std::vector<gbl::Position<>>& i_path_vector, gbl::Map<float>& i_f_scores, gbl::Map<float>& i_g_scores, const gbl::Map<float>& i_h_scores, const gbl::Position<>& i_finish_position, const gbl::Position<>& i_start_position, gbl::Map<>& i_map);

void astar_reset(bool& i_finished, unsigned short& i_path_length, unsigned short& i_total_checks, std::chrono::microseconds& i_duration, std::map<gbl::Position<>, gbl::Position<>>& i_previous_cell, std::vector<gbl::Position<>>& i_path_vector, gbl::Map<float>& i_f_scores, gbl::Map<float>& i_g_scores, gbl::Map<float>& i_h_scores, const gbl::Position<>& i_finish_position, const gbl::Position<>& i_start_position, gbl::Map<>& i_map);