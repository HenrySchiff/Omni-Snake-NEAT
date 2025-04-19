#pragma once
#define _USE_MATH_DEFINES

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <math.h>
#include <iostream>
#include <random>
#include <vector>
#include <optional>
#include <unordered_map>
#include <unordered_set>

namespace util
{
    // keeps angle between 0 and 2 * PI
    inline float boundAngle(float angle) {
        if (angle >= M_PI * 2) { return angle - M_PI * 2; }
        if (angle < 0) { return M_PI * 2 + angle; }
        return angle;
    }

    template <typename T>
    float angleBetween(const sf::Vector2<T>& point1, const sf::Vector2<T>& point2) {
        sf::Vector2<T> direction = point2 - point1;
        return boundAngle(std::atan2(direction.y, direction.x));
    }

    template <typename T>
    float distance(const sf::Vector2<T>& point1, const sf::Vector2<T>& point2) {
        float dx = static_cast<float>(point2.x - point1.x);
        float dy = static_cast<float>(point2.y - point1.y);
        return std::sqrt(dx * dx + dy * dy);
    }

    template <typename T>
    float length(const sf::Vector2<T>& vector) {
        return distance({ 0, 0 }, vector);
    }

    template <typename T>
    sf::Vector2<T> normalize(const sf::Vector2<T>& vector) {
        return vector / distance({0, 0}, vector);
    }

    template <typename T>
    sf::Vector2<T> rotate(const sf::Vector2<T>& vector, float angle) {
        return { std::cos(angle) * vector.x - std::sin(angle) * vector.y, std::sin(angle) * vector.x + std::cos(angle) * vector.y };
    }

    /* https://github.com/user-simon/2D-RayCasting/blob/master/2D_RayCasting/ray.cpp */
    template <typename T>
    std::optional<sf::Vector2<T>> segmentIntersection(sf::Vector2<T> p1, sf::Vector2<T> p2, sf::Vector2<T> p3, sf::Vector2<T> p4) {
        const float den = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);

        if (den == 0) {
            return std::nullopt;
        }

        const float t = ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / den;
        const float u = -((p1.x - p2.x) * (p1.y - p3.y) - (p1.y - p2.y) * (p1.x - p3.x)) / den;

        if (t > 0 && t < 1 && u > 0 && u < 1) {
            sf::Vector2<T> intersection;
            intersection.x = p1.x + t * (p2.x - p1.x);
            intersection.y = p1.y + t * (p2.y - p1.y);
            return intersection;
        }

        return std::nullopt;
    }

    template <typename T>
    void printVector(const sf::Vector2<T>& vector) {
        std::cout << "Vector2(" << vector.x << ", " << vector.y << ")" << std::endl;
    }

    inline void drawLine(sf::RenderWindow& window, const sf::Vector2f& start, const sf::Vector2f& end, float thickness, const sf::Color& color) {
        sf::Vector2f direction = start - end;
        float length = distance(start, end);
        float angle = std::atan2(direction.y, direction.x) * 180.0f / M_PI + 180.0f;

        sf::RectangleShape line({ length, thickness });
        line.setFillColor(color);
        line.setOrigin(0, thickness / 2);
        line.setPosition(start);
        line.setRotation(angle);

        window.draw(line);
    }
}