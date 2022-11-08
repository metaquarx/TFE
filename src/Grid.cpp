// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "Grid.hpp"

#include "Sqroundre.hpp"
#include "TextTools.hpp"

#include <random>
#include <cassert>

static sf::Vector2f calculate_tile_position(Coord coord) {
	return {
		7.f + 14.f + (static_cast<float>(coord.x) * (129.f + 14.f)),
		207.f + 14.f + (static_cast<float>(coord.y) * (129.f + 14.f))
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
			empty.create({129, 129}, 6, sf::Color(205, 193, 180));
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
				tile.value().update(dt);
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
	tile.slide(calculate_tile_position(new_location), sf::seconds(0));
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

	sf::Time move_speed = sf::seconds(0.5);

	switch (move) {
		case Move::Up: {
			decltype(m_tiles) new_tiles{};
			for (std::size_t x = 0; x < 4; x++) {
				std::size_t current_empty = 0;
				for (std::size_t y = 0; y < 4; y++) {
					if (m_tiles[x][y]) {
						new_tiles[x][current_empty] = *m_tiles[x][y];
						new_tiles[x][current_empty]->slide(calculate_tile_position({x, current_empty}), move_speed);
						current_empty++;
					}
				}
			}

			for (std::size_t x = 0; x < 4; x++) {
				for (std::size_t y = 0; y < 3; y++) {
					if (new_tiles[x][y] && new_tiles[x][y + 1] && new_tiles[x][y]->get_value() == new_tiles[x][y + 1]->get_value()) {
						new_tiles[x][y]->increase_value();
						new_tiles[x][y + 1].reset();
						score_bonus += static_cast<unsigned>(std::pow(2, new_tiles[x][y]->get_value()));
					}
				}
			}

			std::swap(m_tiles, new_tiles);
			if (m_tiles != new_tiles) {
				spawn_new();
			}
		} break;
		case Move::Left: {
			decltype(m_tiles) new_tiles{};
			for (std::size_t y = 0; y < 4; y++) {
				std::size_t current_empty = 0;
				for (std::size_t x = 0; x < 4; x++) {
					if (m_tiles[x][y]) {
						new_tiles[current_empty][y] = *m_tiles[x][y];
						new_tiles[current_empty][y]->slide(calculate_tile_position({current_empty, y}), move_speed);
						current_empty++;
					}
				}
			}

			for (std::size_t y = 0; y < 4; y++) {
				for (std::size_t x = 0; x < 3; x++) {
					if (new_tiles[x][y] && new_tiles[x + 1][y] && new_tiles[x][y]->get_value() == new_tiles[x + 1][y]->get_value()) {
						new_tiles[x][y]->increase_value();
						new_tiles[x + 1][y].reset();
						score_bonus += static_cast<unsigned>(std::pow(2, new_tiles[x][y]->get_value()));
					}
				}
			}

			std::swap(m_tiles, new_tiles);
			if (m_tiles != new_tiles) {
				spawn_new();
			}
		} break;
		case Move::Down: {
			decltype(m_tiles) new_tiles{};
			for (std::size_t x = 0; x < 4; x++) {
				std::size_t current_empty = 3;
				for (std::size_t ym = 0; ym < 4; ym++) {
					auto y = 3 - ym;
					if (m_tiles[x][y]) {
						new_tiles[x][current_empty] = *m_tiles[x][y];
						new_tiles[x][current_empty]->slide(calculate_tile_position({x, current_empty}), move_speed);
						current_empty--;
					}
				}
			}

			for (std::size_t x = 0; x < 4; x++) {
				for (std::size_t ym = 0; ym < 3; ym++) {
					auto y = 3 - ym;
					if (new_tiles[x][y] && new_tiles[x][y - 1] && new_tiles[x][y]->get_value() == new_tiles[x][y - 1]->get_value()) {
						new_tiles[x][y]->increase_value();
						new_tiles[x][y - 1].reset();
						score_bonus += static_cast<unsigned>(std::pow(2, new_tiles[x][y]->get_value()));
					}
				}
			}

			std::swap(m_tiles, new_tiles);
			if (m_tiles != new_tiles) {
				spawn_new();
			}
		} break;
		case Move::Right: {
			decltype(m_tiles) new_tiles{};
			for (std::size_t y = 0; y < 4; y++) {
				std::size_t current_empty = 3;
				for (std::size_t xm = 0; xm < 4; xm++) {
					auto x = 3 - xm;
					if (m_tiles[x][y]) {
						new_tiles[current_empty][y] = *m_tiles[x][y];
						new_tiles[current_empty][y]->slide(calculate_tile_position({current_empty, y}), move_speed);
						current_empty--;
					}
				}
			}

			for (std::size_t y = 0; y < 4; y++) {
				for (std::size_t xm = 0; xm < 3; xm++) {
					auto x = 3 - xm;
					if (new_tiles[x][y] && new_tiles[x - 1][y] && new_tiles[x][y] == new_tiles[x - 1][y]) {
						new_tiles[x][y]->increase_value();
						new_tiles[x - 1][y].reset();
						score_bonus += static_cast<unsigned>(std::pow(2, new_tiles[x][y]->get_value()));
					}
				}
			}

			std::swap(m_tiles, new_tiles);
			if (m_tiles != new_tiles) {
				spawn_new();
			}
		} break;
	}
	m_score += score_bonus;
}
