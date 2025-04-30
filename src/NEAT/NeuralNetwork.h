#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <vector>
#include <unordered_map>
#include "Genome.h"

struct NeuronInput {
	int input_id;
	double weight;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(CEREAL_NVP(input_id), CEREAL_NVP(weight));
	}
};

struct Neuron {
	int layer;
	// ActivationFn activation
	int neuron_id;
	double bias;
	std::vector<NeuronInput> inputs;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(CEREAL_NVP(layer), CEREAL_NVP(neuron_id), CEREAL_NVP(bias), CEREAL_NVP(inputs));
	}
};

class FeedForwardNeuralNetwork {
public:
	FeedForwardNeuralNetwork() = default;
	FeedForwardNeuralNetwork(std::vector<int> inputs, std::vector<int> outputs, std::vector<Neuron> neurons, int num_layers);
	static FeedForwardNeuralNetwork create_from_genome(const Genome& genome);
	static std::vector<std::vector<int>> partition_layers(std::vector<int>& inputs, std::vector<int>& outputs, std::vector<LinkGene> links);
	static void bfs(int node_id, std::unordered_map<int, int>& layers, int layer, std::vector<int>& outputs, std::vector<LinkGene> links);
	std::vector<double> activate(const std::vector<double>& inputs);
	double sigmoid(double x);
	std::vector<Neuron> get_neurons() const;
	void print();
	
	std::vector<int> input_ids;
	int num_layers;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(CEREAL_NVP(input_ids), CEREAL_NVP(num_layers), CEREAL_NVP(output_ids), CEREAL_NVP(neurons));
	}

	void save_to_json(const std::string& filename);
	static FeedForwardNeuralNetwork load_from_json(const std::string& filename);

private:
	std::vector<int> output_ids;
	std::vector<Neuron> neurons;
};