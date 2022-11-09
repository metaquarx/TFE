// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Sqroundre.hpp"

void Sqroundre::create(sf::Vector2f size, float radius, sf::Color color, bool centered) {
	m_corners[0].setRadius(radius); // top-left
	m_corners[0].setPosition(0, 0);
	m_corners[0].setFillColor(color);

	m_vertical.setPosition(radius, 0);
	m_vertical.setSize({size.x - radius - radius, size.y});
	m_vertical.setFillColor(color);

	m_corners[1].setRadius(radius); // top-right
	m_corners[1].setPosition(size.x - radius - radius, 0);
	m_corners[1].setFillColor(color);

	m_corners[2].setRadius(radius); // bottom-left
	m_corners[2].setPosition(0, size.y - radius - radius);
	m_corners[2].setFillColor(color);

	m_horizontal.setPosition(0, radius);
	m_horizontal.setSize({size.x, size.y - radius - radius});
	m_horizontal.setFillColor(color);

	m_corners[3].setRadius(radius); // bottom-right
	m_corners[3].setPosition(size.x - radius - radius, size.y - radius - radius);
	m_corners[3].setFillColor(color);

	if (centered) {
		m_vertical.setOrigin(size / 2.f);
		m_horizontal.setOrigin(size / 2.f);
		m_corners[0].setOrigin(size / 2.f);
		m_corners[1].setOrigin(size / 2.f);
		m_corners[2].setOrigin(size / 2.f);
		m_corners[3].setOrigin(size / 2.f);
	}
}

sf::FloatRect Sqroundre::getGlobalBounds() const {
	auto position = getPosition() - getOrigin();
	sf::FloatRect ret;
	ret.left = m_corners[0].getPosition().x + position.x;
	ret.top = m_corners[0].getPosition().y + position.y;

	auto radius = m_corners[3].getRadius();
	ret.width = m_corners[3].getPosition().x + radius + radius;
	ret.height = m_corners[3].getPosition().y + radius + radius;

	return ret;
}

void Sqroundre::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_vertical, states);
	target.draw(m_horizontal, states);

	target.draw(m_corners[0], states);
	target.draw(m_corners[1], states);
	target.draw(m_corners[2], states);
	target.draw(m_corners[3], states);
}
