#pragma once
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "Mutation.h"
#include "Genome.h"
#include "../rng.h"

using namespace Mutation;

bool Mutation::dfs(int node, std::unordered_map<int, std::vector<int>>& adjList, std::unordered_set<int>& visited, std::unordered_set<int>& recStack) {
	visited.insert(node);
	recStack.insert(node);

	for (int neighbor : adjList[node]) {
		if (recStack.find(neighbor) != recStack.end()) {
			return true;
		}
		if (visited.find(neighbor) == visited.end() && dfs(neighbor, adjList, visited, recStack)) {
			return true;
		}
	}

	recStack.erase(node);
	return false;
}

bool Mutation::would_be_cyclic(const std::vector<LinkGene>& links, int input_id, int output_id) {
	std::unordered_map<int, std::vector<int>> adjList;
	for (const auto& link : links) {
		adjList[link.link_id.input_id].push_back(link.link_id.output_id);
	}
	adjList[input_id].push_back(output_id);

	std::unordered_set<int> visited;
	std::unordered_set<int> recStack;

	for (const auto& node : adjList) {
		if (visited.find(node.first) == visited.end()) {
			if (dfs(node.first, adjList, visited, recStack)) {
				return true;
			}
		}
	}

	return false;
}

std::vector<NeuronGene>::iterator Mutation::choose_random_by_layer(std::vector<NeuronGene>& neurons, std::set<LayerType> layers) {
	std::vector<int> filtered_indices;
	for (int i = 0; i < neurons.size(); i++) {
		if (layers.count(neurons.at(i).layer) > 0) {
			filtered_indices.push_back(i);
		}
	}

	return neurons.begin() + rng::choice(filtered_indices);
}

void Mutation::mutate_add_link(Genome& genome) {
	int input_id = choose_random_by_layer(genome.neurons, { INPUT, HIDDEN })->neuron_id;
	int output_id = choose_random_by_layer(genome.neurons, { OUTPUT, HIDDEN })->neuron_id;
	LinkId link_id = { input_id, output_id };

	for (LinkGene& link : genome.links) {
		if (link.link_id == link_id) {
			link.is_enabled = true;
			return;
		}
	}

	if (would_be_cyclic(genome.links, input_id, output_id)) {
		return;
	}

	LinkGene new_link = { {input_id, output_id}, rng::gaussian(0.0, 1.0), true };
	genome.links.push_back(new_link);
}

void Mutation::mutate_remove_link(Genome& genome) {
	if (genome.links.empty()) {
		return;
	}

	genome.links.erase(genome.links.begin() + (rng::random() * genome.links.size()));
}

void Mutation::mutate_add_neuron(Genome& genome, Indexer& neuron_indexer) {
	if (genome.links.empty()) {
		return;
	}

	LinkGene& link_to_split = rng::choice(genome.links);
	link_to_split.is_enabled = false;

	NeuronGene new_neuron = { HIDDEN, neuron_indexer.next(), rng::gaussian(0.0, 1.0)};
	genome.neurons.push_back(new_neuron);

	LinkId link_id = link_to_split.link_id;
	double weight = link_to_split.weight;

	genome.links.push_back(LinkGene{ { link_id.input_id, new_neuron.neuron_id }, 1.0, true });
	genome.links.push_back(LinkGene{ { new_neuron.neuron_id, link_id.output_id }, weight, true });
}

void Mutation::mutate_remove_neuron(Genome& genome) {
	int num_hidden = std::count_if(genome.neurons.begin(), genome.neurons.end(),
		[](const NeuronGene& neuron) { return neuron.layer == HIDDEN; });

	if (num_hidden == 0) {
		return;
	}

	auto neuron_it = choose_random_by_layer(genome.neurons, { HIDDEN });
	int neuron_id = neuron_it->neuron_id;

	genome.links.erase(
		std::remove_if(genome.links.begin(), genome.links.end(),
			[neuron_id](const LinkGene& link) {
				return link.link_id.input_id == neuron_id || link.link_id.output_id == neuron_id;
			}
		), genome.links.end());

	genome.neurons.erase(neuron_it);
}


double Mutation::mutate_delta(double value, Config config) {
	double delta = std::clamp(rng::gaussian(0.0, config.mutation_power), config.min, config.max);
	return std::clamp(value + delta, config.min, config.max);
	;
}

double Mutation::mutate_double(double value, Config config) {
	switch (rng::choose_3(config.mutation_rate, config.replace_rate)) {
	case 0:
		return mutate_delta(value, config);	// mutate existing value
	case 1:
		return rng::gaussian(0.0, 1.0);		// replace with new value
	case 2:
		return value;						// do nothing with value
	default:
		return value;
	}
}

void Mutation::mutate(Genome& genome, Indexer& neuron_indexer) {

	if (rng::coin_flip(neuron_config.add_rate)) {
		mutate_add_neuron(genome, neuron_indexer);
	}
	if (rng::coin_flip(neuron_config.remove_rate)) {
		mutate_remove_neuron(genome);
	}
	if (rng::coin_flip(link_config.add_rate)) {
		mutate_add_link(genome);
	}
	if (rng::coin_flip(link_config.remove_rate)) {
		mutate_remove_link(genome);
	}

	for (auto& neuron : genome.neurons) {
		// neuron.activation = mutate_activation(neuron.activation, neuron_config);
		neuron.bias = mutate_double(neuron.bias, neuron_config);
	}

	for (auto& link : genome.links) {
		link.weight = mutate_double(link.weight, link_config);
	}
}