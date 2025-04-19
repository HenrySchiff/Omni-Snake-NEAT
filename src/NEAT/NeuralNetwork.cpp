#include <unordered_map>
#include <vector>
#include <algorithm>
#include "NeuralNetwork.h"
#include "Genome.h"


FeedForwardNeuralNetwork::FeedForwardNeuralNetwork(std::vector<int> inputs, std::vector<int> outputs, std::vector<Neuron> neurons) :
	input_ids(inputs), output_ids(outputs), neurons(neurons) {
}

FeedForwardNeuralNetwork FeedForwardNeuralNetwork::create_from_genome(const Genome& genome) {
	std::vector<int> inputs;
	std::vector<int> outputs;

	for (int input_id = 0; input_id < genome.num_inputs; input_id++) {
		inputs.push_back(input_id);
	}

	for (int output_id = genome.num_inputs; output_id < genome.num_inputs + genome.num_outputs; output_id++) {
		outputs.push_back(output_id);
	}

	std::vector<std::vector<int>> layers = partition_layers(inputs, outputs, genome.links);

	std::vector<Neuron> neurons;
	for (const std::vector<int>& layer : layers) {
		for (int neuron_id : layer) {
			std::vector<NeuronInput> neuron_inputs;
			for (const LinkGene& link: genome.links) {
				if (!link.is_enabled) {
					continue;
				}
				if (neuron_id == link.link_id.output_id) {
					neuron_inputs.push_back(NeuronInput{ link.link_id.input_id, link.weight });
				}
			}
			auto neuron_it = std::find_if(genome.neurons.begin(), genome.neurons.end(),
				[neuron_id](const NeuronGene& gene) {return gene.neuron_id == neuron_id; });
			neurons.push_back(Neuron{ neuron_id, neuron_it->bias, neuron_inputs });
		}
	}

	return { inputs, outputs, neurons };
}

std::vector<std::vector<int>> FeedForwardNeuralNetwork::partition_layers(std::vector<int>& inputs, std::vector<int>& outputs, std::vector<LinkGene> links) {
	std::unordered_map<int, int> layers_map;
	for (int input_id : inputs) {
		bfs(input_id, layers_map, 0, outputs, links);
	}

	int max_layer = std::max_element(layers_map.begin(), layers_map.end(),
		[](const auto& a, const auto& b) {
			return a.second < b.second;
		})->second;

	std::vector<std::vector<int>> layers(max_layer + 1, {});
	for (const auto& [neuron_id, layer] : layers_map) {
		layers[layer].push_back(neuron_id);
	}
	layers.push_back(outputs);
	return layers;
}

void FeedForwardNeuralNetwork::bfs(int node_id, std::unordered_map<int, int>& layers, int layer, std::vector<int>& outputs, std::vector<LinkGene> links) {
	if (std::find(outputs.begin(), outputs.end(), node_id) != outputs.end()) {
		return;
	}

	if (!layers[node_id] || layers[node_id] < layer) {
		layers[node_id] = layer;
	}

	for (const LinkGene& link : links) {
		if (link.link_id.input_id == node_id) {
			bfs(link.link_id.output_id, layers, layer + 1, outputs, links);
		}
	}
}

std::vector<double> FeedForwardNeuralNetwork::activate(const std::vector<double>& inputs) {
	std::unordered_map<int, double> values;

	for (int i = 0; i < inputs.size(); i++) {
		int input_id = input_ids[i];
		values[input_id] = inputs[i];
	}

	for (int output_id : output_ids) {
		values[output_id] = 0.0;
	}

	for (Neuron neuron : neurons) {
		if (std::find(input_ids.begin(), input_ids.end(), neuron.neuron_id) != input_ids.end()) {
			continue;
		}

		double value = 0.0;
		for (NeuronInput input : neuron.inputs) {
			value += values[input.input_id] * input.weight;
		}
		value += neuron.bias;
		value = sigmoid(value);
		values[neuron.neuron_id] = value;
	}

	std::vector<double> outputs;
	for (int output_id : output_ids) {
		outputs.push_back(values[output_id]);
	}

	return outputs;
}

double FeedForwardNeuralNetwork::sigmoid(double x) {
	return 1.0 / (1.0 + std::exp(-x));
}

void FeedForwardNeuralNetwork::print() {

	std::cout << "\n" << "Neurons:" << "\n\n";
	for (auto& neuron : neurons) {
		std::cout << "Neuron ID: " << neuron.neuron_id;
		std::cout << " Neuron Bias: " << neuron.bias << "\n";

		std::cout << "Inputs:" << "\n";
		for (auto& input : neuron.inputs) {
			std::cout << "Input ID: " << input.input_id;
			std::cout << " Input Weight: " << input.weight << "\n";
		}
		std::cout << "\n";
	}

	std::cout << "\n\n";
}