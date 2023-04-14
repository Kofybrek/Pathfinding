#include <array>
#include <chrono>

#include "Headers/Global.hpp"
#include "Headers/GetCell.hpp"

gbl::MAP::Cell get_cell(const gbl::Position<short>& i_cell, const gbl::Map<>& i_map)
{
	if (0 <= i_cell.first && 0 <= i_cell.second && i_cell.first < gbl::MAP::COLUMNS && i_cell.second < gbl::MAP::ROWS)
	{
		return i_map[i_cell.first][i_cell.second];
	}

	return gbl::MAP::Cell::Invalid;
}