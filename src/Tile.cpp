// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Tile.hpp"
#include <cmath>

static std::pair<sf::Color, sf::Color> colour_of(unsigned value) {
	switch (value) {
		case 1: return {{238, 228, 218}, {119, 110, 101}};
		case 2: return {{238, 225, 201}, {119, 110, 101}};
		case 3: return {{243, 178, 122}, {249, 246, 242}};
		case 4: return {{246, 150, 100}, {249, 246, 242}};
		case 5: return {{247, 124,  95}, {249, 246, 242}};
		case 6: return {{247,  95,  59}, {249, 246, 242}};
		case 7: return {{237, 208, 115}, {249, 246, 242}};
		case 8: return {{237, 204,  98}, {249, 246, 242}};
		case 9: return {{237, 201,  80}, {249, 246, 242}};
		case 10: return {{237, 197,  63}, {249, 246, 242}};
		case 11: return {{237, 194,  46}, {249, 246, 242}};
		default: return {sf::Color::Black, sf::Color::White};
	}
}

Tile::Tile(const sf::Font & font)
: m_value(0) {
	m_text.setFont(font);
}

bool Tile::operator==(const Tile & other) const {
	return m_value == other.m_value;
}

void Tile::slide(sf::Vector2f new_location, float time) {
	auto centered = new_location;
	centered += {65.f, 65.f};

	m_graphic.setPosition(new_location);
	m_text.setPosition(centered);
}

void Tile::update(float dt) {
}

void Tile::set_value(unsigned new_value) {
	m_value = new_value;
	auto new_colours = colour_of(new_value);

	m_graphic.create({129, 129}, 6, new_colours.first);
	m_text.setFillColor(new_colours.second);
	m_text.setString(std::to_string(static_cast<unsigned>(std::pow(2, new_value))));

	unsigned text_size = 64;
	do {
		m_text.setCharacterSize(text_size);
		auto lb = m_text.getLocalBounds();
		m_text.setOrigin(sf::Vector2f{std::trunc(lb.left + lb.width/2), std::trunc(lb.top + lb.height/2)});

		text_size--;
	} while (m_text.getGlobalBounds().width > 129.f);
}

void Tile::increase_value() {
	set_value(get_value() + 1);
}

unsigned Tile::get_value() const {
	return m_value;
}

void Tile::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_graphic, states);
	target.draw(m_text, states);
}
