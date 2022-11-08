// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "TextTools.hpp"

#include <cmath>

void center_text(sf::Text & text) {
	auto bounds = text.getLocalBounds();
	text.setOrigin(std::round(bounds.width / 2), std::round(bounds.height / 2));
}
