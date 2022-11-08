// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "SFML/Graphics/Drawable.hpp"
#include "Sqroundre.hpp"

class Tile : public sf::Drawable {
public:
	Tile(const sf::Font & font);

	bool operator==(const Tile & other) const;

	void slide(sf::Vector2f new_location, float time);
	void update(float dt);
	void set_value(unsigned new_value);
	void increase_value();
	unsigned get_value() const;

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
	unsigned m_value;

	Sqroundre m_graphic;
	sf::Text m_text;
};
