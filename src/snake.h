#pragma once

#define _USE_MATH_DEFINES

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <unordered_set>


enum class Action {
	TurnLeft = 0,
	TurnRight = 1,
	Sprint = 2
};



class Snake {

public:
	const sf::Vector2f STARTING_POSITION = { 100.0f, 100.0f };
	const int STARTING_SEGMENT_COUNT = 15;

	const float NORMAL_SPEED = 2.5f;
	const float SPRINT_SPEED = 7.5f;
	const float TURN_SPEED = 0.09f;

	const float SEGMENT_LENGTH = 10.0f;
	const float SEGMENT_RADIUS = 7.0f;
	const float INCREMENT_AMOUNT = 5.0f;
	const int IGNORED_SEGMENTS = 3;

	Snake();
	void process(std::unordered_set<Action> actions);
	void incrementLength();
	bool checkCollision();
	void reset(bool randomized);

	sf::Vector2f getHead();
	float getSpeed();
	float getDirection();
	int getSegmentCount();
	std::vector<sf::Vector2f> getPoints();
	bool isAlive();

private:
	sf::Vector2f m_position = STARTING_POSITION;
	float m_speed = NORMAL_SPEED;
	float m_direction = 0.0f;
	std::vector<sf::Vector2f> m_points = { m_position, m_position };
	int m_segmentCount = STARTING_SEGMENT_COUNT;
	bool m_alive = true;
};



class SnakeEngine {

public:
	SnakeEngine();
	void process(std::unordered_set<Action> actions);
	void reset(bool randomized);
	sf::Vector2f createApple();
	Snake& getSnake();
	std::vector<sf::Vector2f> getApples();
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> getWalls();

	std::vector<double> get_nn_inputs();
	double getAngleToApple();
	std::pair<double, sf::Vector2f> getRayIntersect(float angle);

private:
	Snake m_snake;
	std::vector<sf::Vector2f> m_apples;
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> m_walls;
};