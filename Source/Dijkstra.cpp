#include <array>
#include <chrono>
#include <map>
#include <queue>

#include "Headers/Global.hpp"
#include "Headers/Dijkstra.hpp"
#include "Headers/GetAdjacentCells.hpp"

DijkstraComparison::DijkstraComparison(gbl::Map<float>* i_distances) :
	distances(i_distances)
{
	
}

bool DijkstraComparison::operator()(const gbl::Position<>& i_left_cell, const gbl::Position<>& i_right_cell) const
{
	//We're comparing the cells based on their distance value.
	float left_cell_distance = distances->at(i_left_cell.first).at(i_left_cell.second);
	float right_cell_distance = distances->at(i_right_cell.first).at(i_right_cell.second);

	return left_cell_distance > right_cell_distance;
}

bool dijkstra_search(unsigned short& i_path_length, unsigned short& i_total_checks, std::chrono::microseconds& i_duration, std::map<gbl::Position<>, gbl::Position<>>& i_previous_cell, std::priority_queue<gbl::Position<>, std::vector<gbl::Position<>>, DijkstraComparison>& i_path_queue, gbl::Map<float>& i_distances, const gbl::Position<>& i_finish_position, const gbl::Position<>& i_start_position, gbl::Map<>& i_map)
{
	//I explained some of the things here in "Astar.hpp" since their code is pretty much similar.
	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

	for (unsigned short a = 0; a < gbl::PATHFINDING::CHECKS_PER_FRAME; a++)
	{
		if (1 == i_path_queue.empty())
		{
			i_duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time);

			return 1;
		}

		gbl::Position<> min_distance_cell;

		//I added this loop because there is a chance we can check the same cell multiple times since we're pushing cells in our queue to update it.
		do
		{
			min_distance_cell = i_path_queue.top();

			i_path_queue.pop();
		}
		while (gbl::MAP::Cell::Visited == i_map[min_distance_cell.first][min_distance_cell.second]);

		i_map[min_distance_cell.first][min_distance_cell.second] = gbl::MAP::Cell::Visited;

		i_total_checks++;

		if (min_distance_cell == i_finish_position)
		{
			gbl::Position<> path_cell = min_distance_cell;

			do
			{
				i_path_length++;

				i_map[path_cell.first][path_cell.second] = gbl::MAP::Cell::Path;

				path_cell = i_previous_cell.at(path_cell);
			}
			while (i_start_position != path_cell);

			i_duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time);

			return 1;
		}

		for (const gbl::Position<>& adjacent_cell : get_adjacent_cells(min_distance_cell, i_map))
		{
			if (gbl::MAP::Cell::Visited != i_map[adjacent_cell.first][adjacent_cell.second])
			{
				float distance = 1;

				if (abs(adjacent_cell.first - min_distance_cell.first) == abs(adjacent_cell.second - min_distance_cell.second))
				{
					distance = sqrt(2);
				}

				if (i_distances[adjacent_cell.first][adjacent_cell.second] > distance + i_distances[min_distance_cell.first][min_distance_cell.second])
				{
					i_distances[adjacent_cell.first][adjacent_cell.second] = distance + i_distances[min_distance_cell.first][min_distance_cell.second];

					i_previous_cell[adjacent_cell] = min_distance_cell;

					i_path_queue.push(adjacent_cell);
				}
			}
		}
	}

	i_duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time);

	return 0;
}

void dijkstra_reset(bool& i_finished, unsigned short& i_path_length, unsigned short& i_total_checks, std::chrono::microseconds& i_duration, std::map<gbl::Position<>, gbl::Position<>>& i_previous_cell, std::priority_queue<gbl::Position<>, std::vector<gbl::Position<>>, DijkstraComparison>& i_path_queue, gbl::Map<float>& i_distances, const gbl::Position<>& i_start_position, gbl::Map<>& i_map)
{
	i_finished = 0;

	i_path_length = 0;
	i_total_checks = 0;

	i_duration = std::chrono::microseconds(0);

	i_previous_cell.clear();

	std::priority_queue<gbl::Position<>, std::vector<gbl::Position<>>, DijkstraComparison>(DijkstraComparison(&i_distances)).swap(i_path_queue);

	i_path_queue.push(i_start_position);

	for (unsigned short a = 0; a < gbl::MAP::COLUMNS; a++)
	{
		for (unsigned short b = 0; b < gbl::MAP::ROWS; b++)
		{
			if (a == i_start_position.first && b == i_start_position.second)
			{
				i_distances[a][b] = 0;
			}
			else
			{
				//We're using FLT_MAX because there is no constant that represents infinity.
				i_distances[a][b] = FLT_MAX;
			}
		}
	}
}