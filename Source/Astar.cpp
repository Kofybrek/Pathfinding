#include <array>
#include <chrono>
#include <map>
#include <vector>

#include "Headers/Global.hpp"
#include "Headers/Astar.hpp"
#include "Headers/GetAdjacentCells.hpp"

float calculate_h_score(const gbl::Position<>& i_cell_0, const gbl::Position<>& i_cell_1)
{
	float distance_x = abs(i_cell_1.first - i_cell_0.first);
	float distance_y = abs(i_cell_1.second - i_cell_0.second);

	//This is the minimal distance it takes to move from cell_0 to cell_1 if we move in 8 directions and ignore every obstacle.
	//I don't recommend using other types of distance calculations because then Astar doesn't find the shortest path.
	return std::max(distance_x, distance_y) + std::min(distance_x, distance_y) * (sqrt(2) - 1);
}

bool astar_search(unsigned short& i_path_length, unsigned short& i_total_checks, std::chrono::microseconds& i_duration, std::map<gbl::Position<>, gbl::Position<>>& i_previous_cell, std::vector<gbl::Position<>>& i_path_vector, gbl::Map<float>& i_f_scores, gbl::Map<float>& i_g_scores, const gbl::Map<float>& i_h_scores, const gbl::Position<>& i_finish_position, const gbl::Position<>& i_start_position, gbl::Map<>& i_map)
{
	//Since we're running all 3 algorithms at the same time, we can't just start a stopwatch, run the algorithm and stop the stopwatch.
	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

	for (unsigned short a = 0; a < gbl::PATHFINDING::CHECKS_PER_FRAME; a++)
	{
		if (1 == i_path_vector.empty())
		{
			i_duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time);

			return 1;
		}

		std::vector<gbl::Position<>>::iterator min_f_cell_iterator = i_path_vector.begin();

		//Here we're finding the cell with the lowest f score.
		gbl::Position<> min_f_cell;

		for (std::vector<gbl::Position<>>::iterator a = 1 + i_path_vector.begin(); a != i_path_vector.end(); a++)
		{
			if (i_f_scores[a->first][a->second] < i_f_scores[min_f_cell_iterator->first][min_f_cell_iterator->second])
			{
				min_f_cell_iterator = a;
			}
		}

		min_f_cell = *min_f_cell_iterator;

		//I just realized that there is no way this will ever be true.
		//Because we will never add a cell that has an f score equal to FLT_MAX in our path_vector.
		if (FLT_MAX == i_f_scores[min_f_cell.first][min_f_cell.second])
		{
			i_duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time);

			return 1;
		}

		i_path_vector.erase(min_f_cell_iterator);

		i_map[min_f_cell.first][min_f_cell.second] = gbl::MAP::Cell::Visited;

		i_total_checks++;

		//Here we highlight the cells starting from the finish all the way to the start when we finally find the path.
		if (min_f_cell == i_finish_position)
		{
			gbl::Position<> path_cell = min_f_cell;

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

		for (const gbl::Position<>& adjacent_cell : get_adjacent_cells(min_f_cell, i_map))
		{
			if (gbl::MAP::Cell::Visited != i_map[adjacent_cell.first][adjacent_cell.second])
			{
				float g_score = i_g_scores[min_f_cell.first][min_f_cell.second];

				if (abs(adjacent_cell.first - min_f_cell.first) == abs(adjacent_cell.second - min_f_cell.second))
				{
					//If the adjacent cell is located diagonally, we add the square root of 2.
					g_score += sqrt(2);
				}
				else
				{
					g_score++;
				}

				if (g_score < i_g_scores[adjacent_cell.first][adjacent_cell.second])
				{
					i_previous_cell[adjacent_cell] = min_f_cell;

					i_f_scores[adjacent_cell.first][adjacent_cell.second] = g_score + i_h_scores[adjacent_cell.first][adjacent_cell.second];
					i_g_scores[adjacent_cell.first][adjacent_cell.second] = g_score;

					if (i_path_vector.end() == std::find(i_path_vector.begin(), i_path_vector.end(), adjacent_cell))
					{
						i_path_vector.push_back(adjacent_cell);
					}
				}
			}
		}
	}

	i_duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time);

	return 0;
}

void astar_reset(bool& i_finished, unsigned short& i_path_length, unsigned short& i_total_checks, std::chrono::microseconds& i_duration, std::map<gbl::Position<>, gbl::Position<>>& i_previous_cell, std::vector<gbl::Position<>>& i_path_vector, gbl::Map<float>& i_f_scores, gbl::Map<float>& i_g_scores, gbl::Map<float>& i_h_scores, const gbl::Position<>& i_finish_position, const gbl::Position<>& i_start_position, gbl::Map<>& i_map)
{
	i_finished = 0;

	i_path_length = 0;
	i_total_checks = 0;

	i_duration = std::chrono::microseconds(0);

	i_previous_cell.clear();

	i_path_vector.clear();
	i_path_vector.push_back(i_start_position);

	for (unsigned short a = 0; a < gbl::MAP::COLUMNS; a++)
	{
		for (unsigned short b = 0; b < gbl::MAP::ROWS; b++)
		{
			//Calculating the h score beforehand because we can.
			i_h_scores[a][b] = calculate_h_score(gbl::Position<>(a, b), i_finish_position);

			if (a == i_start_position.first && b == i_start_position.second)
			{
				i_f_scores[a][b] = i_h_scores[a][b];
				i_g_scores[a][b] = 0;
				
			}
			else
			{
				i_f_scores[a][b] = FLT_MAX;
				i_g_scores[a][b] = FLT_MAX;
			}
		}
	}
}