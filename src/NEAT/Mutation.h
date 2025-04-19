#pragma once
#include <unordered_map>
#include <unordered_set>
#include <set>
#include "neat_config.h"
#include "Genome.h"
#include "Population.h"

using namespace configuration;

namespace Mutation {
	bool dfs(int node, std::unordered_map<int, std::vector<int>>& adjList, std::unordered_set<int>& visited, std::unordered_set<int>& recStack);
	bool would_be_cyclic(const std::vector<LinkGene>& links, int input_id, int output_id);
	std::vector<NeuronGene>::iterator choose_random_by_layer(std::vector<NeuronGene>& neurons, std::set<LayerType> layers);

	void mutate_add_link(Genome& genome);
	void mutate_remove_link(Genome& genome);
	void mutate_add_neuron(Genome& genome, Indexer& neuron_indexer);
	void mutate_remove_neuron(Genome& genome);
	
	double mutate_delta(double value, Config config);
	double mutate_double(double value, Config config);

	void mutate(Genome& genome, Indexer& neuron_indexer);
}