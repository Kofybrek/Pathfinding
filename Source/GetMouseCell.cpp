#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/GetMouseCell.hpp"

gbl::Position<short> get_mouse_cell(const sf::RenderWindow& i_window)
{
	float mouse_x = sf::Mouse::getPosition(i_window).x - gbl::MAP::ORIGIN_X * gbl::SCREEN::RESIZE;
	float mouse_y = sf::Mouse::getPosition(i_window).y - gbl::MAP::ORIGIN_Y * gbl::SCREEN::RESIZE;

	return gbl::Position<short>(floor(mouse_x / gbl::MAP::CELL_SIZE / gbl::SCREEN::RESIZE), floor(mouse_y / gbl::MAP::CELL_SIZE / gbl::SCREEN::RESIZE));
}