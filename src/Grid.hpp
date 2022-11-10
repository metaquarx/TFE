// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <SFML/Graphics.hpp>

#include "Sqroundre.hpp"
#include "Tile.hpp"

#include <array>
#include <optional>
#include <queue>

enum class Move {
	Up, Left, Down, Right
};

using Coord = sf::Vector2<std::size_t>;
class Grid : public sf::Drawable {
public:
	Grid(const sf::Font & font);

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	void update(float dt);
	void queue_input(Move move);
	unsigned get_score() const;

	void clear();

private:
	Sqroundre m_background;
	const sf::Font & m_font;

	using TileMap = std::array<std::array<std::optional<Tile>, 4>, 4>;
	TileMap m_tiles;
	std::queue<Move> m_move_queue;
	unsigned m_score;

	std::vector<Coord> get_empty() const;
	void spawn_new();
	void process_input();
};
