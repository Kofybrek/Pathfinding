#pragma once

class DijkstraComparison
{
	//I don't think using a smart pointer here is beneficial. If you disagree, you're absolutely right!
	gbl::Map<float>* distances;
public:
	DijkstraComparison(gbl::Map<float>* i_distances = 0);

	//I've never used operator functions before. I took this code from StackOverflow.
	bool operator()(const gbl::Position<>& i_left_cell, const gbl::Position<>& i_right_cell) const;
};

bool dijkstra_search(unsigned short& i_path_length, unsigned short& i_total_checks, std::chrono::microseconds& i_duration, std::map<gbl::Position<>, gbl::Position<>>& i_previous_cell, std::priority_queue<gbl::Position<>, std::vector<gbl::Position<>>, DijkstraComparison>& i_path_queue, gbl::Map<float>& i_distances, const gbl::Position<>& i_finish_position, const gbl::Position<>& i_start_position, gbl::Map<>& i_map);

void dijkstra_reset(bool& i_finished, unsigned short& i_path_length, unsigned short& i_total_checks, std::chrono::microseconds& i_duration, std::map<gbl::Position<>, gbl::Position<>>& i_previous_cell, std::priority_queue<gbl::Position<>, std::vector<gbl::Position<>>, DijkstraComparison>& i_path_queue, gbl::Map<float>& i_distances, const gbl::Position<>& i_start_position, gbl::Map<>& i_map);