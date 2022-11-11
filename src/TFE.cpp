// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "TFE.hpp"

#include "TextTools.hpp"

#include <exception>

TFE::TFE()
: m_window({600, 800}, "Twenty Forty-Eight", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings{0, 0, 8})
, m_cursor_hand(false) {
	m_window.setVerticalSyncEnabled(true);

	m_cursor.loadFromSystem(sf::Cursor::Arrow);
	m_window.setMouseCursor(m_cursor);

	m_fonts.reserve(2);
	if (!m_fonts["bold"].loadFromFile("resources/ClearSans-Bold.ttf") ||
		!m_fonts["regular"].loadFromFile("resources/ClearSans-Regular.ttf")) {
		throw std::runtime_error("Unable to open fonts");
	}
	m_grid.emplace(m_fonts.at("bold"));
	m_ui.set_font(m_fonts.at("regular"), m_fonts.at("bold"));

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

				if (m_ui.m_tutorial_button_text.getGlobalBounds().contains(position) ||
					m_ui.m_new_game_button.getGlobalBounds().contains(position)) {
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

				if (m_ui.m_tutorial_button_text.getGlobalBounds().contains(position)) {
					if (m_ui.m_show_tutorial) {
						m_ui.blur(false);
						m_ui.m_show_tutorial = false;
					} else {
						m_ui.blur(true);
						m_ui.m_show_tutorial = true;
					}
				} else if (m_ui.m_new_game_button.getGlobalBounds().contains(position)) {
					m_grid->clear();
					m_ui.blur(false);
					if (m_ui.m_show_tutorial) {
						m_ui.blur(false);
						m_ui.m_show_tutorial = false;
					}
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

	m_ui.update(dt);
	m_grid->update(dt);
	m_ui.update_score(m_grid->get_score());
}

void TFE::draw() {
	m_window.clear(sf::Color(250, 248, 239));

	m_window.draw(m_grid.value());
	m_window.draw(m_ui);

	m_window.display();
}
