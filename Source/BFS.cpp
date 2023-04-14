#include <array>
#include <chrono>
#include <map>
#include <queue>

#include "Headers/Global.hpp"
#include "Headers/BFS.hpp"
#include "Headers/GetAdjacentCells.hpp"

bool bfs_search(unsigned short& i_path_length, unsigned short& i_total_checks, std::chrono::microseconds& i_duration, std::map<gbl::Position<>, gbl::Position<>>& i_previous_cell, std::queue<gbl::Position<>>& i_path_queue, gbl::Map<float>& i_distances, const gbl::Position<>& i_finish_position, const gbl::Position<>& i_start_position, gbl::Map<>& i_map)
{
	//I explained some of the things here in "Astar.hpp" since their code is pretty much similar.
	//So I don't have to write comments here.
	//But I want to!
	//So we're gonna play a game.
	//Scroll down to play it.
	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

	for (unsigned short a = 0; a < gbl::PATHFINDING::CHECKS_PER_FRAME; a++)
	{
		if (1 == i_path_queue.empty())
		{
			i_duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time);

			return 1;
		}

		std::vector<gbl::Position<>> adjacent_cells;

		gbl::Position<> cell = i_path_queue.front();

		adjacent_cells = get_adjacent_cells(cell, i_map);

		i_path_queue.pop();

		if (cell == i_finish_position)
		{
			gbl::Position<> path_cell = cell;

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

		/*
		Okay, find a 'B' here.
		0000000000000000000000000000000000000000000000000000000000000000
		0000000000000000000000000000000000000000000000000000000000000000
		00000000000000000000000000000000000000000000000B0000000000000000
		0000000000000000000000000000000000000000000000000000000000000000
		0000000000000000000000000000000000000000000000000000000000000000
		0000000000000000000000000000000000000000000000000000000000000000
		0000000000000000000000000000000000000000000000000000000000000000
		0000000000000000000000000000000000000000000000000000000000000000



		Okay that was easy. Scroll down to play another game.
		*/

		for (const gbl::Position<>& adjacent_cell : adjacent_cells)
		{
			if (gbl::MAP::Cell::Visited != i_map[adjacent_cell.first][adjacent_cell.second])
			{
				i_map[adjacent_cell.first][adjacent_cell.second] = gbl::MAP::Cell::Visited;

				i_distances[adjacent_cell.first][adjacent_cell.second] = 1 + i_distances[cell.first][cell.second];

				i_total_checks++;

				i_previous_cell[adjacent_cell] = cell;

				i_path_queue.push(adjacent_cell);
			}
		}
	}

	/*
	Find a 'G' here.
	6666666666666666666666666666666666666666666666666666666666666666
	6666666666666666666666666666666666666666666666666666666666666666
	6666666666666666666666666666666666666666666666666666666666666666
	6666666666666666666666666666666666666666666666666666666666666666
	6666666666666666666666666666666666666666666666666666666666666666
	6666666666666666666666666666666666666666666666666666666666666666
	6666666666666666666666666666666666666666666666666666666666666666
	6666666666666666666666666666666666666666666666666666666666666666
	6666666666666666666666666666666666666666666666666666666666666666
	6666666666666666G66666666666666666666666666666666666666666666666
	6666666666666666666666666666666666666666666666666666666666666666
	6666666666666666666666666666666666666666666666666666666666666666
	6666666666666666666666666666666666666666666666666666666666666666
	6666666666666666666666666666666666666666666666666666666666666666
	6666666666666666666666666666666666666666666666666666666666666666
	6666666666666666666666666666666666666666666666666666666666666666



	That was harder. But I'm pretty sure you found it as well (I hope you're not using Search).
	Anyway scroll down to play another game.
	*/

	i_duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time);

	return 0;
}

void bfs_reset(bool& i_finished, unsigned short& i_path_length, unsigned short& i_total_checks, std::chrono::microseconds& i_duration, std::map<gbl::Position<>, gbl::Position<>>& i_previous_cell, std::queue<gbl::Position<>>& i_path_queue, gbl::Map<float>& i_distances, const gbl::Position<>& i_start_position, gbl::Map<>& i_map)
{
	std::chrono::steady_clock::time_point start_time;

	i_finished = 0;

	i_path_length = 0;
	i_total_checks = 1;

	i_previous_cell.clear();

	std::queue<gbl::Position<>>().swap(i_path_queue);

	start_time = std::chrono::steady_clock::now();

	i_path_queue.push(i_start_position);

	i_map[i_start_position.first][i_start_position.second] = gbl::MAP::Cell::Visited;

	i_duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time);
	
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
				i_distances[a][b] = FLT_MAX;
			}
		}
	}

	/*
	Find an 'M' here.
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN








	So how long it took you to realize that there was no 'M' here?
	I'm such a prankstar!
	You should've seen your face! (don't worry, I didn't see it either).
	*/
}