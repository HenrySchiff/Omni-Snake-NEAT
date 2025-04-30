#include "Controller.h"
#include "snake.h"
#include "rng.h"

std::unordered_set<Action> Controller::get_actions() {
	std::unordered_set<Action> actions = m_next_actions;
	m_next_actions.clear();
	return actions;
}

void Controller::check_inputs() {}


void KeyboardController::check_inputs() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		m_next_actions.insert(Action::TurnLeft);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		m_next_actions.insert(Action::TurnRight);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		m_next_actions.insert(Action::Sprint);
	}
}


AIController::AIController(FeedForwardNeuralNetwork neural_network, SnakeEngine& engine) : 
	m_neural_network(neural_network), m_engine(engine) {
}

void AIController::check_inputs() {
	std::vector<double> inputs = m_engine.get_nn_inputs();
	std::vector<double> outputs = m_neural_network.activate(inputs);

	if (outputs.at(0) > 0.5) {
		m_next_actions.insert(Action::TurnLeft);
	}
	if (outputs.at(1) > 0.5) {
		m_next_actions.insert(Action::TurnRight);
	}
	if (outputs.at(2) > 0.5) {
		m_next_actions.insert(Action::Sprint);
	}
}