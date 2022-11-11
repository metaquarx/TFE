// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "Grid.hpp"
#include "Sqroundre.hpp"
#include "UI.hpp"

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <string>

class TFE {
public:
	TFE();
	bool run();

private:
	sf::RenderWindow m_window;
	sf::Clock m_clock;

	std::unordered_map<std::string, sf::Font> m_fonts;

	std::optional<Grid> m_grid;
	UI m_ui;

	void events();
	void update();
	void draw();

	void show_cursor_hand(bool on);
	sf::Cursor m_cursor;
	bool m_cursor_hand;
};
