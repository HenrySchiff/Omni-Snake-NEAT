#include "snake.h"
#include "Controller.h"
#include <iostream>

inline void processEvents(sf::Window& window, SnakeEngine& engine, Controller* controller) {
    controller->check_inputs();

    for (auto event = sf::Event(); window.pollEvent(event);) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed) {

            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            if (event.key.code == sf::Keyboard::Space) {
                engine.reset(false);
            }
        }
    }
}