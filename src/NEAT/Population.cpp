#include <cassert>
#include <optional>
#include "Population.h"
#include "Mutation.h"
#include "neat_config.h"
#include "../OmniSnake/rng.h"

int Indexer::next() {
	return index++;
}

NeuronGene Population::crossover_neuron(const NeuronGene& a, const NeuronGene& b) {
	//assert(a.neuron_id == b.neuron_id);
	LayerType layer = a.layer;
	int neuron_id = a.neuron_id;
	double bias = rng::choose(0.5, a.bias, b.bias);
	return { layer, neuron_id, bias };
	//Activation activation = rng::choose(0.5, a.activation, b.activation);
}

LinkGene Population::crossover_link(const LinkGene& a, const LinkGene& b) {
	//assert(a.link_id == b.link_id);
	LinkId link_id = a.link_id;
	double weight = rng::choose(0.5, a.weight, b.weight);
	bool is_enabled = rng::choose(0.5, a.is_enabled, b.is_enabled);
	return { link_id, weight, is_enabled };
}

Genome Population::crossover(const Individual& a, const Individual& b) {
	Individual dominant = (a.fitness > b.fitness) ? a : b;
	Individual recessive = (a.fitness > b.fitness) ? b : a;

	Genome offspring{ genome_indexer.next(), dominant.genome.num_inputs, dominant.genome.num_outputs};

	for (const auto& dominant_neuron : dominant.genome.neurons) {
		int neuron_id = dominant_neuron.neuron_id;
		std::optional<NeuronGene> recessive_neuron = std::nullopt;

		for (const NeuronGene& neuron : recessive.genome.neurons) {
			if (neuron.neuron_id == neuron_id) {
				recessive_neuron = neuron;
				break;
			}
		}

		if (!recessive_neuron) {
			offspring.neurons.push_back(dominant_neuron);
		}
		else {
			offspring.neurons.push_back(crossover_neuron(dominant_neuron, *recessive_neuron));
		}
	}

	for (const auto& dominant_link : dominant.genome.links) {
		LinkId link_id = dominant_link.link_id;
		std::optional<LinkGene> recessive_link = std::nullopt;

		for (const LinkGene& link : recessive.genome.links) {
			if (link.link_id == link_id) {
				recessive_link = link;
				break;
			}
		}

		if (!recessive_link) {
			offspring.links.push_back(dominant_link);
		}
		else {
			offspring.links.push_back(crossover_link(dominant_link, *recessive_link));
		}
	}

	return offspring;
}



Genome Population::new_genome(int num_inputs, int num_outputs) {
	bool random = true;

	Genome genome = { genome_indexer.next(), num_inputs, num_outputs};

	for (int input_id = 0; input_id < num_inputs; input_id++) {
		genome.neurons.push_back(NeuronGene{ INPUT, input_id, 0.0 });
	}

	for (int output_id = num_inputs; output_id < num_inputs + num_outputs; output_id++) {
		double bias = random ? rng::gaussian(0.0, 1.0) : 0.0;
		genome.neurons.push_back(NeuronGene{ OUTPUT, output_id, bias });
	}

	for (int input_id = 0; input_id < num_inputs; input_id++) {
		for (int output_id = num_inputs; output_id < num_inputs + num_outputs; output_id++) {
			double weight = random ? rng::gaussian(0.0, 1.0) : 1.0;
			genome.links.push_back(LinkGene{ LinkId{ input_id, output_id }, weight, true });
		}
	}

	return genome;
}

//template<typename FitnessFn>
Individual Population::run(int num_generations, int num_inputs, int num_outputs, void (*compute_fitness)(std::vector<Individual>&)) {

	neuron_indexer.index = num_inputs + num_outputs;
	std::vector<Individual> individuals;

	//Individual manual = Individual{ new_genome(num_inputs, num_outputs), static_cast<double>(rng::random()) };
	//Mutation::mutate_add_neuron(manual.genome, neuron_indexer);
	//return manual;

	// populate first generation
	for (int i = 0; i < population_size; i++) {
		individuals.push_back(Individual{ new_genome(num_inputs, num_outputs), 0.0 });
	}

	for (int i = 0; i < num_generations; i++) {
		compute_fitness(individuals);
		individuals = reproduce(individuals);
		std::cout << "Generation: " << i + 1 << ", Best Fitness: " << individuals.at(0).fitness << "\n";
	}

	Individual best = sort_individuals_by_fitness(individuals).at(0);
	return best;
}

std::vector<Individual> Population::sort_individuals_by_fitness(std::vector<Individual> individuals) {
	std::sort(individuals.begin(), individuals.end(),
		[](const auto& a, const auto& b) { return a.fitness > b.fitness; }
	);
	return individuals;
}

std::vector<Individual> Population::reproduce(std::vector<Individual> individuals) {
	auto old_members = sort_individuals_by_fitness(individuals);
	int reproduction_cutoff = std::ceil(configuration::survival_threshold * old_members.size());
	old_members.erase(old_members.begin() + reproduction_cutoff, old_members.end());

	std::vector<Individual> new_generation = old_members;
	int spawn_size = configuration::population_size - new_generation.size();

	while (spawn_size-- >= 0) {
		const Individual& p1 = rng::choice(old_members);
		const Individual& p2 = rng::choice(old_members);
		Genome offspring = crossover(p1, p2);
		Mutation::mutate(offspring, neuron_indexer);
		new_generation.push_back({ offspring, 0.0 });
	}

	return new_generation;
}