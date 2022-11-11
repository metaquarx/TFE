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
		if (event.type == sf::Event::Closed) {
			m_window.close();
		} else if (event.type == sf::Event::MouseMoved) {
			auto position = m_window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});

			if (m_ui.m_tutorial_button_text.getGlobalBounds().contains(position) ||
				m_ui.m_new_game_button.getGlobalBounds().contains(position)) {
				show_cursor_hand(true);
			} else {
				show_cursor_hand(false);
			}
		} else if (event.type == sf::Event::MouseButtonReleased) {
			auto position = m_window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

			if (m_ui.m_tutorial_button_text.getGlobalBounds().contains(position)) {
				m_ui.m_busy ? m_ui.clear() : m_ui.show_tutorial();
			} else if (m_ui.m_new_game_button.getGlobalBounds().contains(position)) {
				m_grid->clear();
				m_ui.clear();			}
		} else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Escape && m_ui.m_busy) {
				m_ui.clear();
			} else if (event.key.code == sf::Keyboard::Escape) {
				m_window.close();
			} else if (event.key.code == sf::Keyboard::N) {
				m_ui.clear();
				m_grid->clear();
			} else if (m_ui.m_busy) {
				// skip next checks
			} else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
				m_grid->queue_input(Move::Up);
			} else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
				m_grid->queue_input(Move::Left);
			} else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
				m_grid->queue_input(Move::Down);
			} else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
				m_grid->queue_input(Move::Right);
			}
		}
	}
}

void TFE::update() {
	float dt = m_clock.restart().asSeconds();

	m_ui.update(dt);
	m_grid->update(dt);
	m_ui.update_score(m_grid->get_score());

	auto state = m_grid->get_state();
	if (state == Grid::GameState::Lose) {
		m_ui.show_lose_screen();
	} else if (state == Grid::GameState::Win) {
		m_ui.show_win_screen();
		m_grid->pass();
	}
}

void TFE::draw() {
	m_window.clear(sf::Color(250, 248, 239));

	m_window.draw(m_grid.value());
	m_window.draw(m_ui);

	m_window.display();
}

void TFE::show_cursor_hand(bool on) {
	if (on && !m_cursor_hand) {
		m_cursor.loadFromSystem(sf::Cursor::Hand);
		m_window.setMouseCursor(m_cursor);
		m_cursor_hand = true;
	} else if (!on && m_cursor_hand) {
		m_cursor_hand = false;
		m_cursor.loadFromSystem(sf::Cursor::Arrow);
		m_window.setMouseCursor(m_cursor);
	}
}
