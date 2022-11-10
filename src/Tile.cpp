// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Tile.hpp"
#include <cmath>
#include <limits>
#include <variant>

SlideAnim::SlideAnim(sf::Vector2f target, float time)
: m_target(target)
, m_end(std::max(time, std::numeric_limits<float>::min())) {
}

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
: m_value(0)
, m_progress(1.f) {
	m_text.setFont(font);

	m_graphic.setPosition({999, 999});
	m_text.setPosition({999, 999});
}

bool Tile::operator==(const Tile & other) const {
	return m_value == other.m_value;
}

void Tile::slide(sf::Vector2f new_location, float time) {
	m_anim.push(SlideAnim{new_location, time});
}

void Tile::pop() {
	m_anim.push(PopAnim());
}

static sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float t) {
	return {
		a.x + t * (b.x - a.x),
		a.y + t * (b.y - a.y)
	};
}

void Tile::update(float dt) {
	m_clock += dt;

	if (m_anim.size()) {
		auto & current = m_anim.front();

		if (std::holds_alternative<SlideAnim>(current)) {
			auto & curr = std::get<SlideAnim>(current);

			if (m_progress == 1.f) {
				m_clock = 0.f;
				curr.m_begin = m_graphic.getPosition();
			}

			m_progress = m_clock / curr.m_end;
			m_progress = std::min(m_progress, 1.f);

			auto position = lerp(curr.m_begin, curr.m_target, m_progress);

			m_graphic.setPosition(position);
			m_text.setPosition(position);

			if (m_progress == 1.f) {
				m_anim.pop();
				if (curr.m_end == std::numeric_limits<float>::min()) {
					update(dt);
				}
			}

		} else if (std::holds_alternative<PopAnim>(current)) {
			constexpr float pop_duration = .2f;

			auto i = [](float t) {
				return std::pow(t, 2.5f);
			};
			auto pop = [&](float t) {
				return t < .9f ? i(t / .9f) : i((1 - t) / .82f) + 1.f;
			};

			if (m_progress == 1.f) {
				m_clock = -move_speed;
			}

			m_progress = m_clock / pop_duration;
			m_progress = std::clamp(m_progress, 0.f, 1.f);

			float scale = pop(m_progress);
			m_graphic.setScale({scale, scale});
			m_text.setScale({scale, scale});

			if (m_progress == 1.f) {
				m_anim.pop();
			}
		}
	}
}

void Tile::set_value(unsigned new_value) {
	m_value = new_value;
	auto new_colours = colour_of(new_value);

	m_graphic.create({129, 129}, 6, new_colours.first, true);
	m_text.setFillColor(new_colours.second);
	m_text.setString(std::to_string(static_cast<unsigned>(std::pow(2, new_value))));

	unsigned text_size = 64;
	do {
		m_text.setCharacterSize(text_size);
		auto lb = m_text.getLocalBounds();
		m_text.setOrigin(sf::Vector2f{lb.left + lb.width/2, lb.top + lb.height/2});

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
