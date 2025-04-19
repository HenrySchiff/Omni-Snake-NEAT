#pragma once

namespace configuration
{
	struct Config {
		float add_rate;
		float remove_rate;
		float mutation_rate;
		float replace_rate;
		float mutation_power;
		double min;
		double max;
	};

	inline Config neuron_config = {
		0.4f,
		0.001f,
		0.8f,
		0.05f,
		0.2f,
		-30.0,
		30.0,
	};

	inline Config link_config = {
		0.6f,
		0.01f,
		0.5f,
		0.1f,
		0.5f,
		-20.0,
		20.0
	};

	inline float survival_threshold = 0.15f;
	inline int population_size = 20;
}