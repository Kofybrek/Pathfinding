#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/DrawMap.hpp"

void draw_map(const unsigned short i_x, const unsigned short i_y, const gbl::Position<>& i_finish_position, const gbl::Position<>& i_start_position, sf::RenderWindow& i_window, sf::Sprite& i_map_sprite, const gbl::Map<>& i_map, const bool i_change_colors, const gbl::Map<float>& i_distances)
{
	float max_distance = FLT_MIN;

	if (1 == i_change_colors)
	{
		for (const std::array<float, gbl::MAP::ROWS>& column : i_distances)
		{
			for (const float distance : column)
			{
				if (FLT_MAX != distance)
				{
					max_distance = std::max(distance, max_distance);
				}
			}
		}
	}

	for (unsigned short a = 0; a < gbl::MAP::COLUMNS; a++)
	{
		for (unsigned short b = 0; b < gbl::MAP::ROWS; b++)
		{
			i_map_sprite.setPosition(gbl::MAP::ORIGIN_X + i_x + a * gbl::MAP::CELL_SIZE, gbl::MAP::ORIGIN_Y + i_y + b * gbl::MAP::CELL_SIZE);

			if (a == i_finish_position.first && b == i_finish_position.second)
			{
				i_map_sprite.setColor(sf::Color(219, 0, 0));
			}
			else if (a == i_start_position.first && b == i_start_position.second)
			{
				i_map_sprite.setColor(sf::Color(0, 219, 0));
			}
			else
			{
				gbl::MAP::Cell cell_type = i_map[a][b];

				switch (cell_type)
				{
					case gbl::MAP::Cell::Empty:
					{
						i_map_sprite.setColor(sf::Color(36, 36, 85));

						break;
					}
					case gbl::MAP::Cell::Path:
					{
						i_map_sprite.setColor(sf::Color(255, 219, 85));

						break;
					}
					case gbl::MAP::Cell::Visited:
					{
						if (0 != i_change_colors)
						{
							//The gradient will go from (0, 146, 255) to (0, 0, 255) then (0, 0, 85).
							unsigned short color_value = 316 - round(316 * i_distances[a][b] / max_distance);

							i_map_sprite.setColor(sf::Color(0, std::max(0, color_value - 170), std::min(255, 85 + color_value)));
						}
						else
						{
							i_map_sprite.setColor(sf::Color(0, 36, 255));
						}

						break;
					}
					case gbl::MAP::Cell::Wall:
					{
						i_map_sprite.setColor(sf::Color(255, 255, 255));
					}
				}
			}

			i_window.draw(i_map_sprite);
		}
	}
}