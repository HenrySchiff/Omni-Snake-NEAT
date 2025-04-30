#pragma once
#include "utility.h"

namespace conf
{
	// Window Configuration
	sf::Vector2u const window_size = { 800, 800 };
	sf::Vector2f const window_size_f = static_cast<sf::Vector2f>(window_size);
	float const window_hypot = util::length(window_size_f);
	uint32_t const max_framerate = 60;
	float const dt = 1.0f / static_cast<float>(max_framerate);

	// Apple Configuration
	float const radius = 10.0f;
	int const max_apples = 1;
	int const tolerance = 5;
	int const dead_zone = 40;

	int const border = 15;
	sf::Color const border_color = { 150, 150, 150 };
}