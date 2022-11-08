// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "TFE.hpp"

#include "TextTools.hpp"

#include <iostream>
#include <exception>

TFE::TFE()
: m_window({600, 800}, "Twenty Forty-Eight", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings{0, 0, 8})
, m_cursor_hand(false) {
	m_window.setVerticalSyncEnabled(true);

	m_cursor.loadFromSystem(sf::Cursor::Arrow);
	m_window.setMouseCursor(m_cursor);

	m_fonts.reserve(5);
	if (!m_fonts["bold"].loadFromFile("resources/ClearSans-Bold.ttf") ||
		!m_fonts["regular"].loadFromFile("resources/ClearSans-Regular.ttf")) {
		throw std::runtime_error("Unable to open fonts");
	}
	m_grid.emplace(m_fonts.at("bold"));

	m_title.setFont(m_fonts.at("bold"));
	m_title.setString("2048");
	m_title.setFillColor(sf::Color(119, 110, 101));
	m_title.setCharacterSize(110);
	m_title.setPosition({8, 0});

	m_prompt.setFont(m_fonts.at("regular"));
	m_prompt.setString("Join the tiles, get to");
	m_prompt.setFillColor(sf::Color(119, 110, 101));
	m_prompt.setCharacterSize(20);
	m_prompt.setPosition({13, 128});

	m_prompt_bold.setFont(m_fonts.at("bold"));
	m_prompt_bold.setString("2048!");
	m_prompt_bold.setFillColor(sf::Color(119, 110, 101));
	m_prompt_bold.setCharacterSize(19);
	m_prompt_bold.setPosition({195, 129});

	m_tutorial_button_text.setFont(m_fonts.at("bold"));
	m_tutorial_button_text.setString("How to play " + sf::String(8594u));
	m_tutorial_button_text.setStyle(sf::Text::Style::Underlined);
	m_tutorial_button_text.setFillColor(sf::Color(119, 110, 101));
	m_tutorial_button_text.setCharacterSize(20);
	m_tutorial_button_text.setPosition({13, 155});

	m_current_score_box.create({150, 80}, 6, sf::Color(187, 173, 160));
	m_current_score_box.setPosition(286, 16);

	m_current_score_number.setFont(m_fonts.at("bold"));
	m_current_score_number.setString("0");
	m_current_score_number.setFillColor(sf::Color::White);
	m_current_score_number.setCharacterSize(32);
	m_current_score_number.setPosition({361, 55});

	m_current_score_tag.setFont(m_fonts.at("bold"));
	m_current_score_tag.setString("SCORE");
	m_current_score_tag.setFillColor(sf::Color(238, 228, 218));
	m_current_score_tag.setCharacterSize(16);
	m_current_score_tag.setPosition({361, 32});
	center_text(m_current_score_tag);

	m_best_score_box.create({150, 80}, 6, sf::Color(187, 173, 160));
	m_best_score_box.setPosition(443, 16);

	m_best_score_number.setFont(m_fonts.at("bold"));
	m_best_score_number.setString("0");
	m_best_score_number.setFillColor(sf::Color::White);
	m_best_score_number.setCharacterSize(32);
	m_best_score_number.setPosition({518, 55});

	m_best_score_tag.setFont(m_fonts.at("bold"));
	m_best_score_tag.setString("BEST");
	m_best_score_tag.setFillColor(sf::Color(238, 228, 218));
	m_best_score_tag.setCharacterSize(16);
	m_best_score_tag.setPosition({518, 32});
	center_text(m_best_score_tag);

	m_new_game_button.create({170, 50}, 6, sf::Color(143, 122, 102));
	m_new_game_button.setPosition({423, 129});

	m_new_game_button_text.setFont(m_fonts.at("bold"));
	m_new_game_button_text.setString("New Game");
	m_new_game_button_text.setFillColor(sf::Color(249, 246, 242));
	m_new_game_button_text.setCharacterSize(26);
	m_new_game_button_text.setPosition({507, 146});
	center_text(m_new_game_button_text);

	m_grid->clear();
}

bool TFE::run() {
	events();
	update();
	draw();
	return m_window.isOpen();
}

void TFE::events() {
	sf::Event event;
	while (m_window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed: {
				m_window.close();
			} break;
			case sf::Event::MouseMoved: {
				sf::Vector2f position{sf::Vector2i{event.mouseMove.x, event.mouseMove.y}};

				if (m_tutorial_button_text.getGlobalBounds().contains(position) ||
					m_new_game_button.getGlobalBounds().contains(position)) {
					if (!m_cursor_hand) {
						m_cursor.loadFromSystem(sf::Cursor::Hand);
						m_window.setMouseCursor(m_cursor);
						m_cursor_hand = true;
					}
				} else if (m_cursor_hand) {
					m_cursor.loadFromSystem(sf::Cursor::Arrow);
					m_window.setMouseCursor(m_cursor);
					m_cursor_hand = false;
				}
			} break;
			case sf::Event::MouseButtonReleased: {
				sf::Vector2f position{sf::Vector2i{event.mouseButton.x, event.mouseButton.y}};

				if (m_tutorial_button_text.getGlobalBounds().contains(position)) {
					std::cout << "action: tutorial" << std::endl;
				} else if (m_new_game_button.getGlobalBounds().contains(position)) {
					m_grid->clear();
				}
			} break;
			case sf::Event::KeyPressed: {
				switch (event.key.code) {
					case sf::Keyboard::W: [[fallthrough]];
					case sf::Keyboard::Up: {
						m_grid->queue_input(Move::Up);
					} break;
					case sf::Keyboard::A: [[fallthrough]];
					case sf::Keyboard::Left: {
						m_grid->queue_input(Move::Left);
					} break;
					case sf::Keyboard::S: [[fallthrough]];
					case sf::Keyboard::Down: {
						m_grid->queue_input(Move::Down);
					} break;
					case sf::Keyboard::D: [[fallthrough]];
					case sf::Keyboard::Right: {
						m_grid->queue_input(Move::Right);
					} break;
					default: break;
				}
			} break;
			default: break;
		}
	}
}

void TFE::update() {
	float dt = m_clock.restart().asSeconds();
	m_grid->update(dt);

	m_current_score_number.setString(std::to_string(m_grid->get_score()));
	m_best_score_number.setString(std::to_string(std::max(
		m_grid->get_score(),
		static_cast<unsigned>(std::stol(std::string(m_best_score_number.getString())))
	)));
	center_text(m_current_score_number);
	center_text(m_best_score_number);
}

void TFE::draw() {
	m_window.clear(sf::Color(250, 248, 239));

	m_window.draw(m_title);

	m_window.draw(m_prompt);
	m_window.draw(m_prompt_bold);

	m_window.draw(m_tutorial_button_text);

	m_window.draw(m_new_game_button);
	m_window.draw(m_new_game_button_text);

	m_window.draw(m_current_score_box);
	m_window.draw(m_current_score_tag);
	m_window.draw(m_current_score_number);

	m_window.draw(m_best_score_box);
	m_window.draw(m_best_score_tag);
	m_window.draw(m_best_score_number);

	m_window.draw(m_grid.value());

	m_window.display();
}
