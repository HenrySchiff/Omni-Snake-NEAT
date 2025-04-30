#pragma once

#include <SFML/Window.hpp>
#include "snake.h"
#include "../NEAT/NeuralNetwork.h"
#include <unordered_set>

class Controller {

public:
	std::unordered_set<Action> get_actions();
	virtual void check_inputs();
	virtual ~Controller() = default;

protected:
	std::unordered_set<Action> m_next_actions;
};


class KeyboardController : public Controller {

public:
	void check_inputs() override;
};


class AIController : public Controller {

public:
	AIController(FeedForwardNeuralNetwork neural_network, SnakeEngine& engine);
	void check_inputs() override;

private:
	FeedForwardNeuralNetwork m_neural_network;
	SnakeEngine& m_engine;
};