#pragma once

#include "Genome.h"

struct Indexer {
	int index = 0;
	int next();
};

class Population {

private:
	Indexer genome_indexer;
	Indexer neuron_indexer;

public:
	NeuronGene crossover_neuron(const NeuronGene& a, const NeuronGene& b);
	LinkGene crossover_link(const LinkGene& a, const LinkGene& b);
	Genome crossover(const Individual& dominant, const Individual& recessive);
	Genome new_genome(int num_inputs, int num_outputs);

	//template<typename FitnessFn>
	//Individual run(FitnessFn compute_fitness, int num_generations);
	Individual run(int num_generations, int num_inputs, int num_outputs, void (*compute_fitness)(std::vector<Individual>&));

	std::vector<Individual> sort_individuals_by_fitness(std::vector<Individual> individuals);
	std::vector<Individual> reproduce(std::vector<Individual> individuals);
};