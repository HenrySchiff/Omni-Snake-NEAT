#pragma once
#include <vector>
#include <iostream>

enum LayerType {
	INPUT,
	OUTPUT,
	HIDDEN
};

struct NeuronGene {
	LayerType layer;
	int neuron_id;
	double bias;
	//Activation activation;
};

struct LinkId {
	int input_id;
	int output_id;

	bool operator == (const LinkId& other) const {
		return input_id == other.input_id && output_id == other.output_id;
	}
};

struct LinkGene {
	LinkId link_id;
	double weight;
	bool is_enabled;
};

struct Genome {
	int genome_id;
	int num_inputs;
	int num_outputs;
	std::vector<NeuronGene> neurons;
	std::vector<LinkGene> links;
};

struct Individual {
	Genome genome;
	double fitness;
};

inline void print_individual(Individual individual) {
	std::cout << "Fitness: " << individual.fitness << "\n\n";

	Genome genome = individual.genome;
	std::cout << "Genome ID: " << genome.genome_id << "\n";
	std::cout << "Genome Inputs: " << genome.num_inputs << "\n";
	std::cout << "Genome Outputs: " << genome.num_outputs << "\n";

	std::cout << "\n" << "Neurons:" << "\n";
	for (auto& neuron : genome.neurons) {
		std::cout << "Neuron ID: " << neuron.neuron_id;
		std::cout << " Neuron Layer: " << neuron.layer;
		std::cout << " Neuron Bias: " << neuron.bias << "\n";
	}

	std::cout << "\n" << "Links:" << "\n";
	for (auto& link : genome.links) {
		std::cout << "Link ID: " << link.link_id.input_id << "->" << link.link_id.output_id;
		std::cout << " Link Weight: " << link.weight;
		std::cout << " Link Enabled: " << link.is_enabled << "\n";
	}

	std::cout << "\n\n";
}