#pragma once

void draw_map(const unsigned short i_x, const unsigned short i_y, const gbl::Position<>& i_finish_position, const gbl::Position<>& i_start_position, sf::RenderWindow& i_window, sf::Sprite& i_map_sprite, const gbl::Map<>& i_map, const bool i_change_colors = 0, const gbl::Map<float>& i_distances = {});