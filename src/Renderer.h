#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include "snake.h"

class GameRenderer {

public:
	GameRenderer(sf::RenderWindow& window, SnakeEngine& engine);
	void draw();
	void drawSnake();

private:
	sf::RenderWindow& m_window;
	SnakeEngine& m_engine;

	const float EYE_RADIUS = 3.5f;
	const float EYE_ANGLE = M_PI / 2.0f;
	const float EYE_DISTANCE = 5.5f;
	const float PUPIL_RADIUS = 3.0f;
	const float PUPIL_ANGLE = -M_PI / 2.0f + 0.25f;
	const float PUPIL_DISTANCE = 1.0f;
	const sf::Color COLOR = sf::Color(242, 149, 69);
};