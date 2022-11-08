// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "Grid.hpp"
#include "Sqroundre.hpp"

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
	sf::Cursor m_cursor;
	bool m_cursor_hand;

	std::unordered_map<std::string, sf::Font> m_fonts;

	std::optional<Grid> m_grid;

	sf::Text m_title;

	sf::Text m_prompt;
	sf::Text m_prompt_bold;

	sf::Text m_tutorial_button_text;


	Sqroundre m_current_score_box;
	sf::Text m_current_score_tag;
	sf::Text m_current_score_number;

	Sqroundre m_best_score_box;
	sf::Text m_best_score_tag;
	sf::Text m_best_score_number;

	Sqroundre m_new_game_button;
	sf::Text m_new_game_button_text;

	void events();
	void update();
	void draw();
};
