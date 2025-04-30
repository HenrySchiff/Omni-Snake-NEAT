#include "Renderer.h"
#include "configuration.h"


void Renderer::draw() {}


GameRenderer::GameRenderer(sf::RenderWindow& window, SnakeEngine& engine) :
    Renderer(window), m_engine(engine) {}

void GameRenderer::draw() {
    m_window.clear();

    std::vector<sf::Vector2f> apples = m_engine.getApples();

    for (auto it = begin(apples); it != end(apples); ++it) {
        sf::Vector2f apple = *it;

        sf::CircleShape shape{ conf::radius };
        shape.setOrigin(conf::radius, conf::radius);
        shape.setPosition(apple);
        shape.setFillColor(sf::Color::Red);
        m_window.draw(shape);
    }

    sf::RectangleShape widthBorder{ { conf::window_size_f.x, conf::border } };
    sf::RectangleShape heightBorder{ { conf::border, conf::window_size_f.y } };
    widthBorder.setFillColor(conf::border_color);
    heightBorder.setFillColor(conf::border_color);

    widthBorder.setPosition({ 0, 0 });
    m_window.draw(widthBorder);
    widthBorder.setPosition({ 0, conf::window_size_f.x - conf::border });
    m_window.draw(widthBorder);

    heightBorder.setPosition({ 0, 0 });
    m_window.draw(heightBorder);
    heightBorder.setPosition({ conf::window_size_f.y - conf::border, 0 });
    m_window.draw(heightBorder);

    drawSnake();

    if (m_drawInfo) {
        for (auto& wall : m_engine.getWalls()) {
            util::drawLine(m_window, wall.first, wall.second, 3.0f, sf::Color::Magenta);
        }

        for (float angle : {-M_PI / 4.0, 0.0, M_PI / 4.0}) {
            sf::Vector2f intersection = m_engine.getRayIntersect(angle).second;
            util::drawLine(m_window, m_engine.getSnake().getHead(), intersection, 3.0f, sf::Color::Magenta);
        }
    }

    m_window.display();
}

void GameRenderer::drawSnake() {
    Snake snake = m_engine.getSnake();

    sf::Vector2f head = snake.getHead();
    float speed = snake.getSpeed();
    float direction = snake.getDirection();
    std::vector<sf::Vector2f> points = snake.getPoints();

    float radius = snake.SEGMENT_RADIUS;
    sf::CircleShape shape{ radius };
    shape.setOrigin(radius, radius);
    shape.setFillColor(COLOR);

    for (int i = 0; i < points.size() - 1; ++i) {
        sf::Vector2f point = points[i];
        sf::Vector2f nextPoint = points[i + 1];

        shape.setPosition(point);
        m_window.draw(shape);
        util::drawLine(m_window, point, nextPoint, snake.SEGMENT_RADIUS * 2, COLOR);
    }

    util::drawLine(m_window, points.back(), head, snake.SEGMENT_RADIUS * 2, COLOR);
    shape.setPosition(head);
    m_window.draw(shape);

    sf::CircleShape eye{ EYE_RADIUS };
    eye.setOrigin(EYE_RADIUS, EYE_RADIUS);
    eye.setFillColor(sf::Color::White);

    float leftEyeAngle = direction - EYE_ANGLE;
    sf::Vector2f leftEye = head + sf::Vector2f(std::cos(leftEyeAngle) * EYE_DISTANCE, std::sin(leftEyeAngle) * EYE_DISTANCE);

    float rightEyeAngle = direction + EYE_ANGLE;
    sf::Vector2f rightEye = head + sf::Vector2f(std::cos(rightEyeAngle) * EYE_DISTANCE, std::sin(rightEyeAngle) * EYE_DISTANCE);

    eye.setPosition(leftEye);
    m_window.draw(eye);
    eye.setPosition(rightEye);
    m_window.draw(eye);

    sf::CircleShape pupil{ PUPIL_RADIUS };
    pupil.setOrigin(PUPIL_RADIUS, PUPIL_RADIUS);
    pupil.setFillColor(sf::Color::Black);

    float leftPupilAngle = direction - PUPIL_ANGLE;
    sf::Vector2f leftPupil = leftEye + sf::Vector2f(std::cos(leftPupilAngle) * PUPIL_DISTANCE, std::sin(leftPupilAngle) * PUPIL_DISTANCE);

    float rightPupilAngle = direction + PUPIL_ANGLE;
    sf::Vector2f rightPupil = rightEye + sf::Vector2f(std::cos(rightPupilAngle) * PUPIL_DISTANCE, std::sin(rightPupilAngle) * PUPIL_DISTANCE);

    pupil.setPosition(leftPupil);
    m_window.draw(pupil);
    pupil.setPosition(rightPupil);
    m_window.draw(pupil);
}

void GameRenderer::toggleDrawInfo() {
    m_drawInfo = !m_drawInfo;
}


NeuralNetworkRenderer::NeuralNetworkRenderer(sf::RenderWindow& window, FeedForwardNeuralNetwork nn) :
    Renderer(window), m_neuralNetwork(nn) 
{
    m_font.loadFromFile("C:/Users/henry/OneDrive/Desktop/Projects/C++ Projects/cmake-sfml-project/build/bin/Debug/consolas.ttf");

    int numLayers = m_neuralNetwork.num_layers;
    std::vector<int> layerCounts(numLayers, 0);
    std::vector<int> layerHeights(numLayers, 0);

    for (const Neuron& neuron : m_neuralNetwork.get_neurons()) {
        layerCounts[neuron.layer]++;
    }

    int highestLayer = std::distance(layerCounts.begin(), std::max_element(layerCounts.begin(), layerCounts.end()));
    float maxLayerHeight = layerCounts[highestLayer] * (NEURON_RADIUS * 2.0f + NEURON_GAP) - NEURON_GAP;

    float layoutWidth = (numLayers - 1) * LAYER_GAP;
    float layoutHeight = maxLayerHeight;

    sf::Vector2u windowSize = m_window.getSize();
    sf::Vector2f offset = {
        (windowSize.x - layoutWidth) / 2.0f,
        (windowSize.y - layoutHeight) / 2.0f
    };

    for (const Neuron& neuron : m_neuralNetwork.get_neurons()) {
        int layer = neuron.layer;
        int count = layerCounts[layer];
        int index = layerHeights[layer]++;

        float layerHeight = count * (NEURON_RADIUS * 2.0f + NEURON_GAP) - NEURON_GAP;
        float layerOffset = (maxLayerHeight - layerHeight) / 2.0f;

        sf::Vector2f position = {
            layer * LAYER_GAP + offset.x,
            layerOffset + index * (NEURON_RADIUS * 2.0f + NEURON_GAP) + offset.y
        };
        m_neuronPositions[neuron.neuron_id] = position;
    }
}

void NeuralNetworkRenderer::draw() {
    m_window.clear();

    drawNeuralNetwork();

    m_window.display();
}

void NeuralNetworkRenderer::drawNeuralNetwork() {
    std::vector<Neuron> neurons = m_neuralNetwork.get_neurons();
    int numLayers = m_neuralNetwork.num_layers;

    for (Neuron neuron : neurons) {
        for (NeuronInput input : neuron.inputs) {
            sf::Vector2f start = m_neuronPositions[input.input_id];
            sf::Vector2f end = m_neuronPositions[neuron.neuron_id];
            util::drawLine(m_window, start, end, 1.0f, sf::Color::Cyan);
        }
    }

    sf::CircleShape shape{ NEURON_RADIUS };
    shape.setOrigin(NEURON_RADIUS, NEURON_RADIUS);
    shape.setFillColor(sf::Color::White);
    sf::Text text;
    text.setFont(m_font);
    text.setCharacterSize(25);
    text.setFillColor(sf::Color::White);

    for (int i = 0; i < neurons.size(); i++) {
        Neuron neuron = neurons.at(i);

        if (neuron.layer == 0) {
            text.setString(INPUT_NAMES[i]);
            text.setPosition(m_neuronPositions[neuron.neuron_id] + sf::Vector2f{-200, -15});
            m_window.draw(text);
        }

        if (neuron.layer == numLayers - 1) {
            text.setString(OUTPUT_NAMES[i - neurons.size() + 3]);
            text.setPosition(m_neuronPositions[neuron.neuron_id] + sf::Vector2f{40, -15});
            m_window.draw(text);
        }

        shape.setPosition(m_neuronPositions[neuron.neuron_id]);
        m_window.draw(shape);
    }
}