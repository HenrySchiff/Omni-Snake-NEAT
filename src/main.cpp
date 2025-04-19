#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "events.h"
#include "configuration.h"
#include "utility.h"
#include "rng.h"
#include "snake.h"
#include "Controller.h"
#include "Renderer.h"
#include "NEAT/Population.h"
#include "NEAT/NeuralNetwork.h"
#include "NEAT/Genome.h"

void compute_fitness(std::vector<Individual>& individuals) {
    for (auto& individual : individuals) {
        auto nn = FeedForwardNeuralNetwork::create_from_genome(individual.genome);
        SnakeEngine engine;
        AIController controller(nn, engine);

        int frames = 0;
        const int max_frames = 10000;
        const int max_rounds = 1;

        individual.fitness = 0;

        for (int i = 0; i < max_rounds; i++) {
            while (engine.getSnake().isAlive() && frames < max_frames) {
                controller.check_inputs();
                engine.process(controller.get_actions());
                frames++;
            }

            individual.fitness += engine.getSnake().getSegmentCount();
            engine.reset(false);
        }
    }
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 3;

    auto window = sf::RenderWindow({ conf::window_size.x, conf::window_size.y }, "Omni Snake", sf::Style::Default, settings);
    window.setFramerateLimit(conf::max_framerate);

    bool training = false;
    const int GENERATIONS = 200;
    const int INPUTS = 5;
    const int OUTPUTS = 3;

    SnakeEngine engine;
    GameRenderer renderer(window, engine);
    std::unique_ptr<Controller> controller;

    if (training) {
        Population pop;
        Individual best = pop.run(GENERATIONS, INPUTS, OUTPUTS, compute_fitness);
        print_individual(best);
        FeedForwardNeuralNetwork nn = FeedForwardNeuralNetwork::create_from_genome(best.genome);
        controller = std::make_unique<AIController>(nn, engine);
    } else {
        controller = std::make_unique<KeyboardController>();
    }

    while (window.isOpen()) {
        processEvents(window, engine, controller.get());
        engine.process(controller->get_actions());
        renderer.draw();
    }
}