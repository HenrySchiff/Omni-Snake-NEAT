#define _USE_MATH_DEFINES

#include "snake.h"
#include "utility.h"
#include "configuration.h"
#include "rng.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cmath>
#include <math.h>
#include <unordered_set>
#include <algorithm>
#include <climits>


SnakeEngine::SnakeEngine() {
    m_apples.push_back(createApple());
    m_walls = {
        { { 0, conf::border }, { conf::window_size_f.x, conf::border } },
        { { conf::border, 0 }, { conf::border, conf::window_size_f.y } },
        { { 0, conf::window_size_f.y - conf::border }, { conf::window_size_f.x, conf::window_size_f.y - conf::border } },
        { { conf::window_size_f.x - conf::border, 0 }, { conf::window_size_f.x - conf::border, conf::window_size_f.y } }
    };
}

void SnakeEngine::process(std::unordered_set<Action> actions) {

    for (auto it = begin(m_apples); it != end(m_apples); ++it) {
        sf::Vector2f apple = *it;

        if (util::distance(m_snake.getHead(), apple) <= conf::radius + conf::tolerance + m_snake.SEGMENT_RADIUS) {
            *it = createApple();
            m_snake.incrementLength();
        }
    }

    m_snake.process(actions);
}

void SnakeEngine::reset(bool randomized) {
    m_apples = { createApple() };
    m_snake.reset(randomized);
}

sf::Vector2f SnakeEngine::createApple() {
    float const x = rng::random() * (conf::window_size_f.x - conf::dead_zone * 2) + conf::dead_zone;
    float const y = rng::random() * (conf::window_size_f.y - conf::dead_zone * 2) + conf::dead_zone;

    return { x, y };
}

Snake& SnakeEngine::getSnake() {
    return m_snake;
}

std::vector<sf::Vector2f> SnakeEngine::getApples() {
    return m_apples;
}

std::vector<std::pair<sf::Vector2f, sf::Vector2f>> SnakeEngine::getWalls() {
    return m_walls;
}

std::vector<double> SnakeEngine::get_nn_inputs() {
    std::vector<double> inputs;

    inputs.push_back(static_cast<double>(m_snake.getSegmentCount()));
    inputs.push_back(getAngleToApple());

    for (float angle : {-M_PI / 4.0, 0.0, M_PI / 4.0}) {
        inputs.push_back(getRayIntersect(angle).first);
    }

    return inputs;
}

double SnakeEngine::getAngleToApple() {
    float angle = util::angleBetween(getSnake().getHead(), getApples().at(0));
    float difference = angle - getSnake().getDirection();
    if (difference > M_PI)  { difference -= M_PI * 2; }
    if (difference < -M_PI) { difference += M_PI * 2; }

    // angle is normalized within values of -1 and 1
    return static_cast<double>(difference) / M_PI;
}

std::pair<double, sf::Vector2f> SnakeEngine::getRayIntersect(float angle) {
    sf::Vector2f head = m_snake.getHead();
    sf::Vector2f rayEnd = head + util::rotate(sf::Vector2f(1200, 0), m_snake.getDirection() + angle);
    std::vector<sf::Vector2f> points = m_snake.getPoints();

    float min_distance = std::numeric_limits<float>::max();
    sf::Vector2f closest;

    // check for intersection with each wall
    for (auto& wall : m_walls) {
        auto intersection = util::segmentIntersection(head, rayEnd, wall.first, wall.second);
        if (intersection.has_value()) {
            float distance = util::distance(head, intersection.value());
            if (distance < min_distance) {
                min_distance = distance;
                closest = intersection.value();
            }
        }
    }

    // check for intersection with each body segment
    for (int i = 0; i < points.size() - 1; i++) {
        sf::Vector2f point = points.at(i);
        sf::Vector2f next = points.at(i + 1);

        auto intersection = util::segmentIntersection(head, rayEnd, point, next);
        if (intersection.has_value()) {
            float distance = util::distance(head, intersection.value());
            if (distance < min_distance) {
                min_distance = distance;
                closest = intersection.value();
            }
        }
    }

    // distance is normalized wrt the window's hypot, i.e. the max ray length
    return { static_cast<double>(min_distance) / conf::window_hypot, closest};
}


Snake::Snake() {}

void Snake::process(std::unordered_set<Action> actions) {
    if (!m_alive) {
        return;
    }

    float turn = 0;

    //for (Action action : actions) {
    //    std::cout << static_cast<typename std::underlying_type<Action>::type>(action) << "\n";
    //}

    if (actions.find(Action::TurnRight) != actions.end()) {
        turn += TURN_SPEED;
    }
    if (actions.find(Action::TurnLeft) != actions.end()) {
        turn -= TURN_SPEED;
    }
    if (actions.find(Action::Sprint) != actions.end()) {
        m_speed = SPRINT_SPEED;
    }
    else {
        m_speed = NORMAL_SPEED;
    }

    m_direction = util::boundAngle(m_direction + turn);

    sf::Vector2f head_velocity = { std::cos(m_direction) * m_speed, std::sin(m_direction) * m_speed };
    m_position += head_velocity;

    if (util::distance(m_position, m_points.back()) >= SEGMENT_LENGTH) {
        sf::Vector2f newPoint = util::normalize(m_position - m_points.back()) * SEGMENT_LENGTH + m_points.back();
        m_points.push_back(newPoint);
    }

    if (m_points.size() > m_segmentCount) {
        m_points.erase(m_points.begin());
    }
    if (m_points.size() == m_segmentCount) {
        sf::Vector2f tail_velocity = util::normalize(m_points.at(1) - m_points.at(0)) * m_speed;
        m_points.at(0) += tail_velocity;
    }

    m_alive = !checkCollision();
}

void Snake::incrementLength() {
    m_segmentCount += INCREMENT_AMOUNT;
}

bool Snake::checkCollision() {
    for (signed int i = 0; i < static_cast<int>(m_points.size()) - IGNORED_SEGMENTS; i++) {
        sf::Vector2f point = m_points.at(i);

        if (util::distance(point, m_position) <= SEGMENT_RADIUS * 2) {
            return true;
        }
    }

    if (m_position.x - SEGMENT_RADIUS < conf::border || m_position.x + SEGMENT_RADIUS > conf::window_size.x - conf::border) {
        return true;
    }

    if (m_position.y - SEGMENT_LENGTH < conf::border || m_position.y + SEGMENT_RADIUS > conf::window_size.y - conf::border) {
        return true;
    }

    return false;
}

void Snake::reset(bool randomized) {
    m_position = STARTING_POSITION;
    m_segmentCount = STARTING_SEGMENT_COUNT;
    m_direction = 0.0f;
    m_points = { m_position, m_position };
    m_alive = true;

    if (randomized) {
        m_direction = rng::random() * M_PI * 2;
    }
}

sf::Vector2f Snake::getHead() {
    return m_position;
}

float Snake::getSpeed() {
    return m_speed;
}

float Snake::getDirection() {
    return m_direction;
}

int Snake::getSegmentCount() {
    return m_segmentCount;
}

std::vector<sf::Vector2f> Snake::getPoints() {
    return m_points;
}

bool Snake::isAlive() {
    return m_alive;
}