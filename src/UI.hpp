// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <SFML/Graphics/Text.hpp>
#include "Sqroundre.hpp"

class UI : public sf::Drawable {
public:
	UI();
	void set_font(const sf::Font & regular, const sf::Font & bold);
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	void update_score(unsigned new_value);

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
};
