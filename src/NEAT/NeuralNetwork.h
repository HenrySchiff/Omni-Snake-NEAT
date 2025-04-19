#pragma once

#include <vector>
#include <unordered_map>
#include "Genome.h"

struct NeuronInput {
	int input_id;
	double weight;
};

struct Neuron {
	// ActivationFn activation
	int neuron_id;
	double bias;
	std::vector<NeuronInput> inputs;
};

class FeedForwardNeuralNetwork {
public:
	FeedForwardNeuralNetwork(std::vector<int> inputs, std::vector<int> outputs, std::vector<Neuron> neurons);
	static FeedForwardNeuralNetwork create_from_genome(const Genome& genome);
	static std::vector<std::vector<int>> partition_layers(std::vector<int>& inputs, std::vector<int>& outputs, std::vector<LinkGene> links);
	static void bfs(int node_id, std::unordered_map<int, int>& layers, int layer, std::vector<int>& outputs, std::vector<LinkGene> links);
	std::vector<double> activate(const std::vector<double>& inputs);
	double sigmoid(double x);
	void print();
	std::vector<int> input_ids;

private:
	std::vector<int> output_ids;
	std::vector<Neuron> neurons;
};