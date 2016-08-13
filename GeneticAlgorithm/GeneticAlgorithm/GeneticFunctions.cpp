#include "GeneticFunctions.h"
#include <ctime>
#include <algorithm>
#include <bitset>

#define DOUBLE_IN_BITS 64 // sizeof(double) * sizeof(double)
#define GEN_SHIFT pow(2, 64)

func F;  // Current counting function

// Sorting functors
bool is_greater(Chromosome & c1, Chromosome & c2) {
	return F(c1) > F(c2);
}
bool is_less(Chromosome & c1, Chromosome & c2) {
	return F(c1) < F(c2);
}

// Converts double to binary string 
bitset<DOUBLE_IN_BITS> get_binary_string(double x, double x_min, double x_max) {
	unsigned long long int s_int = (unsigned __int64)((x - x_min) / (x_max - x_min) * GEN_SHIFT);

	return bitset<DOUBLE_IN_BITS>(s_int);
}
// Converts binary string to double
double get_double_value(bitset<DOUBLE_IN_BITS> & bs, double x_min, double x_max) {
	return x_min + bs.to_ullong() * (x_max - x_min) / GEN_SHIFT;
}

// Sorts population by fitness
void pop_sort(Population & pop, Condition cond) {
	if (cond == MAXIMUM)
		sort(pop.begin(), pop.end(), is_greater);
	else sort(pop.begin(), pop.end(), is_less);
}

// Randomly generate start population
Population generate_population(unsigned size, unsigned gen_count, double lbound, double rbound) {
	Population p;
	const double DELTA = rbound - lbound;

	// For each individual
	for (unsigned i = 0; i < size; i++) {
		// For each gen
		Chromosome c;
		for (unsigned j = 0; j < gen_count; j++) {
			// Generate value from [lbound..rbound] with defined accuracy
			double gen = lbound + DELTA * ACCURACY * (rand() % (int)(1 / ACCURACY + 1));
			c.push_back(gen);
		}
		p.push_back(c);
	}

	return p;
}

// From population select CHALLENGE_ROUNDS species by challenging
void selection(Population pop, Population & next_pop, Condition cond) {
	// Forming offsprings 
	for (int i = 0; i < CHALLENGE_ROUNDS; i++) {
		// Select two indexes of chromosomes for challenging
		int i1 = rand() % pop.size(), i2;
		do {
			i2 = rand() % pop.size();
		} while (i1 == i2); 

		// Take chromosome with best fitness from two 
		if (cond == MAXIMUM && is_greater(pop[i1], pop[i2]) || cond == MINIMUM && !is_greater(pop[i1], pop[i2])) {
			next_pop.push_back(pop[i1]);
			pop.erase(pop.begin() + i1);
		}
		else {
			next_pop.push_back(pop[i2]);
			pop.erase(pop.begin() + i2);
		}
	}
}

void crossover(Population & pop, const double X_MIN, const double X_MAX) {
	const unsigned int POP_SIZE = pop.size();
	const unsigned int GEN_COUNT = pop[0].size();
	const unsigned int BIN_STR_LEN = GEN_COUNT * DOUBLE_IN_BITS;

	for (unsigned i = 0; i < POP_SIZE / 2; i++) {
		// Select pair to crossover
		/*unsigned i1 = rand() % POP_SIZE, i2;
		do {
			i2 = rand() % POP_SIZE;
		} while (i1 == i2);*/
		unsigned i1 = i, i2 = i + 1;

		// Calculate bits to crossover
		unsigned lbound = rand() % BIN_STR_LEN;
		unsigned rbound = rand() % BIN_STR_LEN;

		if (lbound > rbound) swap(lbound, rbound);

		bitset<DOUBLE_IN_BITS> * bin_str1 = new bitset<DOUBLE_IN_BITS>[GEN_COUNT];
		bitset<DOUBLE_IN_BITS> * bin_str2 = new bitset<DOUBLE_IN_BITS>[GEN_COUNT];

		for (unsigned k = 0; k < GEN_COUNT; k++) {
			bin_str1[k] = get_binary_string(pop[i1][k], X_MIN, X_MAX);
			bin_str2[k] = get_binary_string(pop[i2][k], X_MIN, X_MAX);
		}

		// Exchange bits
		for (unsigned k = lbound; k <= rbound; k++) {
			unsigned int gen_index = k / DOUBLE_IN_BITS;
			unsigned int bit_index = k % DOUBLE_IN_BITS;

			bool b = bin_str1[gen_index][bit_index];
			bin_str1[gen_index][bit_index] = bin_str2[gen_index][bit_index];
			bin_str2[gen_index][bit_index] = b;
		}

		// Pull back changed binary string to double values
		for (unsigned k = 0; k < GEN_COUNT; k++) {
			pop[i1][k] = get_double_value(bin_str1[k], X_MIN, X_MAX);
			pop[i2][k] = get_double_value(bin_str2[k], X_MIN, X_MAX);
		}

		// Free memory
		delete[] bin_str1;
		delete[] bin_str2;
	}
}

void crossoverToBuffer(vector<Chromosome> & pop, vector<Chromosome>& buffer, Condition cond) {
	unsigned j;
	unsigned index1, index2;

	buffer.clear();

	index1 = rand() % pop.size();
	do {
		index2 = rand() % pop.size();
	} while (index1 == index2);

	vector<double> h1, h2, h3;
	for (j = 0; j < pop[0].size(); j++) {
		h1.push_back((pop[index1][j] + pop[index2][j]) / 2.);
		h2.push_back((3 * pop[index1][j] - pop[index2][j]) / 2.);
		h3.push_back((3 * pop[index2][j] - pop[index1][j]) / 2.);
	}

	buffer.push_back(Chromosome(h1));
	buffer.push_back(Chromosome(h2));
	buffer.push_back(Chromosome(h3));
	pop_sort(buffer, cond);
}

void linearCrossover(vector<Chromosome> & pop, vector<Chromosome>& tmp_population, Condition cond) {
	vector<Chromosome> buffer;
	vector<Chromosome> generated_population;

	int population_size = pop.size();
	for (size_t i = 0; i < pop.size() / 2; i++) {
		crossoverToBuffer(pop, buffer, cond);
		generated_population.push_back(buffer[0]);
		generated_population.push_back(buffer[1]);
	}

	tmp_population.swap(generated_population);
}

// Accidental mutation in random individual random gen
void mutation(Population & new_population, double lbound, double rbound) {
	// Mutation happens with some possibility
	for (unsigned i = 0; i < new_population.size(); i++)
		if (rand() % 100 < MUTATION_POSSIBILITY) {
			// Mutated chromosome has new value
			Chromosome new_c;
			const double DELTA = rbound - lbound;
			for (unsigned i = 0; i < new_population[i].size(); i++)
				new_c.push_back(lbound + DELTA * ACCURACY * (rand() % (int)(1 / ACCURACY + 1)));
			new_population[i] = new_c;
		}
}

void elitism(Population & pop, Population & next_pop) {
	for (int i = 0; i < ELITISM_SIZE; i++) 
		next_pop.push_back(pop[i]);
}

void show(Population & pop) {
	for (unsigned k = 0; k < pop[0].size(); k++)
		cout << pop[0][k] << "  ";
	cout << "= " << F(pop[0]) << endl;
}

Chromosome process_function(Population  & start_pop, func f, Condition cond, double lbound, double rbound)
{
	Population pop = start_pop, next_pop;
	F = f;

	srand((unsigned)time(0));
	for (unsigned i = 0; i < GENERATIONS_NUMBER; i++) {
		pop_sort(pop, cond);

		next_pop = Population();
		elitism(pop, next_pop);
		selection(pop, next_pop, cond);
		//linearCrossover(pop, next_pop, cond);
		crossover(next_pop, lbound, rbound);
		mutation(next_pop, lbound, rbound);

		pop_sort(next_pop, cond);
		copy(next_pop.begin(), next_pop.end() - ELITISM_SIZE, pop.begin() + ELITISM_SIZE);
		
		if (i % 10 == 0) show(pop); 
	};

	return pop[0];
}