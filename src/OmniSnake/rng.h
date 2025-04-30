#pragma once
#include <random>

namespace rng
{
    inline std::random_device rd;
    inline std::mt19937 gen(rd());
    inline std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    inline float random() {
        return dis(gen);
    }

    template <typename T>
    T choose(float p, T a, T b) {
        if (dis(gen) <= p) { return a; }
        return b;
    }

    inline int choose_3(float first_p, float second_p) {
        float p = random();
        if (p <= first_p) {
            return 0;
        }
        else if (p <= first_p + second_p) {
            return 1;
        }
        else {
            return 2;
        }
    }

    template <typename T>
    T choice(std::vector<T> vector) {
        return vector.at(random() * vector.size());
    }

    inline bool coin_flip(float p) {
        return random() < p;
    }

    inline double gaussian(double mean, double stddev) {
        std::normal_distribution<double> dis(mean, stddev);
        return dis(gen);
    }
}