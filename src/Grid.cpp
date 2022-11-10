// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Grid.hpp"

#include "Sqroundre.hpp"

#include <random>
#include <cassert>

static sf::Vector2f calculate_tile_position(Coord coord) {
	return {
		7.f + 14.f + (static_cast<float>(coord.x) * (129.f + 14.f)) + 64.f,
		207.f + 14.f + (static_cast<float>(coord.y) * (129.f + 14.f)) + 64.f
	};
}

Grid::Grid(const sf::Font & font)
: m_font{font} {
	m_background.create({586, 586}, 6, sf::Color(187, 173, 160));
	m_background.setPosition({7, 207});
}

void Grid::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(m_background, states);

	for (std::size_t i = 0; i < 4; i++) {
		for (std::size_t j = 0; j < 4; j++) {
			Sqroundre empty;
			empty.create({129, 129}, 6, sf::Color(205, 193, 180), true);
			empty.setPosition(calculate_tile_position({i, j}));
			target.draw(empty);
		}
	}

	for (auto column : m_tiles) {
		for (auto tile : column) {
			if (tile) {
				target.draw(tile.value(), states);
			}
		}
	}
}

void Grid::update(float dt) {
	if (m_move_queue.size()) {
		process_input();
	}

	for (auto & column : m_tiles) {
		for (auto & tile : column) {
			if (tile) {
				tile.value().update(dt * (1.f + static_cast<float>(m_move_queue.size())));
			}
		}
	}
}

void Grid::queue_input(Move move) {
	m_move_queue.push(move);
}

unsigned Grid::get_score() const {
	return m_score;
}

void Grid::clear() {
	m_tiles.fill({std::nullopt});
	m_move_queue = {};
	m_score = 0;

	spawn_new();
	spawn_new();
}

void Grid::spawn_new() {
	auto empty = get_empty();
	assert(empty.size());

	static std::random_device rdev;
	static std::default_random_engine reng(rdev());

	std::uniform_int_distribution<unsigned> value_dist(1u, 2u);
	auto new_value = value_dist(reng);

	std::uniform_int_distribution<unsigned> location_dist(0u, static_cast<unsigned>(empty.size() - 1));
	auto new_location = empty[location_dist(reng)];

	auto & tile = m_tiles[new_location.x][new_location.y].emplace(m_font);
	tile.set_value(new_value);
	tile.slide(calculate_tile_position(new_location), 0);
	tile.pop();
}

std::vector<Coord> Grid::get_empty() const {
	std::vector<Coord> empty;
	empty.reserve(16);
	for (std::size_t x = 0; x < 4; x++) {
		for (std::size_t y = 0; y < 4; y++) {
			if (!m_tiles[x][y]) {
				empty.push_back({x, y});
			}
		}
	}
	return empty;
}

void Grid::process_input() {
	auto move = m_move_queue.front();
	m_move_queue.pop();
	unsigned score_bonus = 0;

	bool positive{move == Move::Up || move == Move::Left};
	bool inverse{move == Move::Left || move == Move::Right};

	auto shift = [&](const TileMap & input) {
		TileMap new_tiles{};
		for (std::size_t i = 0; i < 4; i++) {
			std::size_t current_empty = positive ? 0 : 3;
			for (std::size_t j = 0; j < 4; j++) {
				auto x{inverse ? j : i};
				auto y{inverse ? i : j};
				x = positive ? x : 3 - x;
				y = positive ? y : 3 - y;

				auto xm = inverse ? current_empty : x;
				auto ym = inverse ? y : current_empty;

				if (input[x][y]) {
					new_tiles[xm][ym] = *input[x][y];
					new_tiles[xm][ym]->slide(calculate_tile_position({xm, ym}), move_speed);

					if (positive) {
						current_empty++;
					} else {
						current_empty--;
					}
				}
			}
		}

		return new_tiles;
	};

	auto combine = [&](const TileMap & input) {
		TileMap new_tiles{input};
		for (std::size_t i = 0; i < 4; i++) {
			for (std::size_t j = 0; j < 3; j++) {
				auto x{inverse ? j : i};
				auto y{inverse ? i : j};
				x = move == Move::Right ? 3 - x : x;
				y = move == Move::Down ? 3 - y : y;

				auto xm = x + (move == Move::Left) - (move == Move::Right);
				auto ym = y + (move == Move::Up) - (move == Move::Down);

				if (new_tiles[x][y] && new_tiles[xm][ym] && new_tiles[x][y]->get_value() == new_tiles[xm][ym]->get_value()) {
					new_tiles[x][y]->increase_value();
					new_tiles[xm][ym].reset();
					score_bonus += static_cast<unsigned>(std::pow(2, new_tiles[x][y]->get_value()));
				}
			}
		}

		return new_tiles;
	};

	auto new_tiles{shift(combine(shift(m_tiles)))};

	std::swap(m_tiles, new_tiles);
	if (m_tiles != new_tiles) {
		spawn_new();
	}

	m_score += score_bonus;
}
