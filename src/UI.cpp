// SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include "UI.hpp"
#include "TextTools.hpp"

UI::UI()
: m_blurred(false)
, m_show_tutorial(false)
, m_blur_progress(0.f) {
	m_title.setString("2048");
	m_title.setFillColor(sf::Color(119, 110, 101));
	m_title.setCharacterSize(110);
	m_title.setPosition({8, 0});

	m_prompt.setString("Join the tiles, get to");
	m_prompt.setFillColor(sf::Color(119, 110, 101));
	m_prompt.setCharacterSize(20);
	m_prompt.setPosition({13, 128});

	m_prompt_bold.setString("2048!");
	m_prompt_bold.setFillColor(sf::Color(119, 110, 101));
	m_prompt_bold.setCharacterSize(19);
	m_prompt_bold.setPosition({195, 129});

	m_tutorial_button_text.setString("How to play " + sf::String(8594u));
	m_tutorial_button_text.setStyle(sf::Text::Style::Underlined);
	m_tutorial_button_text.setFillColor(sf::Color(119, 110, 101));
	m_tutorial_button_text.setCharacterSize(20);
	m_tutorial_button_text.setPosition({13, 155});

	m_current_score_box.create({150, 80}, 6, sf::Color(187, 173, 160));
	m_current_score_box.setPosition(286, 16);

	m_current_score_number.setString("0");
	m_current_score_number.setFillColor(sf::Color::White);
	m_current_score_number.setCharacterSize(32);
	m_current_score_number.setPosition({361, 55});

	m_current_score_tag.setString("SCORE");
	m_current_score_tag.setFillColor(sf::Color(238, 228, 218));
	m_current_score_tag.setCharacterSize(16);
	m_current_score_tag.setPosition({361, 32});

	m_best_score_box.create({150, 80}, 6, sf::Color(187, 173, 160));
	m_best_score_box.setPosition(443, 16);

	m_best_score_number.setString("0");
	m_best_score_number.setFillColor(sf::Color::White);
	m_best_score_number.setCharacterSize(32);
	m_best_score_number.setPosition({518, 55});

	m_best_score_tag.setString("BEST");
	m_best_score_tag.setFillColor(sf::Color(238, 228, 218));
	m_best_score_tag.setCharacterSize(16);
	m_best_score_tag.setPosition({518, 32});

	m_new_game_button.create({170, 50}, 6, sf::Color(143, 122, 102));
	m_new_game_button.setPosition({423, 129});

	m_new_game_button_text.setString("New Game");
	m_new_game_button_text.setFillColor(sf::Color(249, 246, 242));
	m_new_game_button_text.setCharacterSize(26);
	m_new_game_button_text.setPosition({507, 146});

	m_blur.setSize({1000, 1000});
	m_blur.setFillColor(sf::Color::Transparent);
	m_blur.setPosition({0, 205});

	m_tutorial_text.setString(
"                            Use your                                       to move the\n"
"tiles. Tiles with the same number                              when they\n"
"touch. Add them up to reach\n\n"
"Additional keybinds:\n"
"            - New game\n"
"            - Back/Exit"
	);
	m_tutorial_text.setFillColor(sf::Color(119, 110, 101));
	m_tutorial_text.setCharacterSize(20);
	m_tutorial_text.setPosition({16, 220});

	m_tutorial_text_bold.setString(
"HOW TO PLAY:                   arrow keys or WASD\n"
"                                                             merge into one\n"
"                                                    2048!\n\n\n"
"     N\n"
"   Esc\n"
	);
	m_tutorial_text_bold.setFillColor(sf::Color(119, 110, 101));
	m_tutorial_text_bold.setCharacterSize(20);
	m_tutorial_text_bold.setPosition({16, 220});

	m_copyright_text.setString(
"     This program is licensed under the GPLv3 licence. It is heavily inspired by the\n"
"          game of the same name made by Gabrile Cirulli, released under the MIT\n"
"        licence, which you can find here: https://github.com/gabrielecirulli/2048"
	);
	m_copyright_text.setFillColor(sf::Color(119, 110, 101));
	m_copyright_text.setCharacterSize(16);
	m_copyright_text.setPosition({5, 700});
}

void UI::set_font(const sf::Font &regular, const sf::Font &bold) {
	m_title.setFont(bold);
	m_prompt.setFont(regular);
	m_prompt_bold.setFont(bold);
	m_tutorial_button_text.setFont(bold);
	m_current_score_number.setFont(bold);
	m_current_score_tag.setFont(bold);
	m_best_score_number.setFont(bold);
	m_best_score_tag.setFont(bold);
	m_new_game_button_text.setFont(bold);
	m_tutorial_text.setFont(regular);
	m_tutorial_text_bold.setFont(bold);
	m_copyright_text.setFont(regular);
	center_text(m_current_score_tag);
	center_text(m_best_score_tag);
	center_text(m_new_game_button_text);
}

void UI::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(m_title, states);
	target.draw(m_prompt, states);
	target.draw(m_prompt_bold, states);
	target.draw(m_tutorial_button_text, states);
	target.draw(m_current_score_box, states);
	target.draw(m_current_score_tag, states);
	target.draw(m_current_score_number, states);
	target.draw(m_best_score_box, states);
	target.draw(m_best_score_tag, states);
	target.draw(m_best_score_number, states);
	target.draw(m_new_game_button, states);
	target.draw(m_new_game_button_text, states);

	target.draw(m_blur);

	if (m_show_tutorial) {
		target.draw(m_tutorial_text, states);
		target.draw(m_tutorial_text_bold, states);
		target.draw(m_copyright_text, states);
	}
}

void UI::update_score(unsigned int new_value) {
	m_current_score_number.setString(std::to_string(new_value));
	m_best_score_number.setString(std::to_string(std::max(
		new_value,
		static_cast<unsigned>(std::stol(std::string(m_best_score_number.getString())))
	)));
	center_text(m_current_score_number);
	center_text(m_best_score_number);
}

void UI::blur(bool on) {
	m_blurred = on;
}

void UI::update(float dt) {
	constexpr float bg_max_opacity = 0.8f;
	constexpr float blur_speed = 8.f;

	m_blur_progress += (m_blurred ? 1.f : -1.f) * dt * blur_speed;
	m_blur_progress = std::clamp(m_blur_progress, 0.f, 1.f);

	float opacity = std::clamp(m_blur_progress, 0.f, bg_max_opacity);
	m_blur.setFillColor(sf::Color(250, 248, 239, static_cast<sf::Uint8>(255.f * opacity)));

	m_tutorial_text.setFillColor(sf::Color(119, 110, 101, static_cast<sf::Uint8>(255.f * m_blur_progress)));
	m_tutorial_text_bold.setFillColor(sf::Color(119, 110, 101, static_cast<sf::Uint8>(255.f * m_blur_progress)));
}

void UI::show_tutorial(bool on) {
	m_show_tutorial = on;
	blur(on);
}
