// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <SFML/Graphics.hpp>

#include <array>

class Sqroundre : public sf::Transformable, public sf::Drawable {
public:
	void create(sf::Vector2f size, float radius, sf::Color color, bool centered = false);
	sf::FloatRect getGlobalBounds() const;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
	sf::RectangleShape m_vertical;
	sf::RectangleShape m_horizontal;
	std::array<sf::CircleShape, 4> m_corners;
};
