#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include "snake.h"
#include "../NEAT/NeuralNetwork.h"

class Renderer {

public:
	Renderer(sf::RenderWindow& window) : m_window(window) {}
	virtual void draw();
	virtual ~Renderer() = default;

protected:
	sf::RenderWindow& m_window;
};

class GameRenderer : public Renderer {

public:
	GameRenderer(sf::RenderWindow& window, SnakeEngine& engine);
	void draw() override;
	void toggleDrawInfo();

private:
	SnakeEngine& m_engine;
	bool m_drawInfo = true;

	const float EYE_RADIUS = 3.5f;
	const float EYE_ANGLE = M_PI / 2.0f;
	const float EYE_DISTANCE = 5.5f;
	const float PUPIL_RADIUS = 3.0f;
	const float PUPIL_ANGLE = -M_PI / 2.0f + 0.25f;
	const float PUPIL_DISTANCE = 1.0f;
	const sf::Color COLOR = sf::Color(242, 149, 69);

	void drawSnake();
};


class NeuralNetworkRenderer : public Renderer {

public:
	NeuralNetworkRenderer(sf::RenderWindow& window, FeedForwardNeuralNetwork engine);
	void draw() override;

private:
	FeedForwardNeuralNetwork m_neuralNetwork;
	std::unordered_map<int, sf::Vector2f> m_neuronPositions;
	sf::Font m_font;

	const float NEURON_RADIUS = 20.0f;
	const float NEURON_GAP = 80.0f;
	const float LAYER_GAP = 100.0f;

	const std::array<std::string, 5> INPUT_NAMES = {
		"Length", "Apple Angle", "Ray 1", "Ray 2", "Ray 3"
	};

	const std::array<std::string, 5> OUTPUT_NAMES = {
		"Turn Left", "Turn Right", "Sprint"
	};

	void drawNeuralNetwork();
};