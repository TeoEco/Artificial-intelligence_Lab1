#include "GeneticFunctions.h"

void init();

int main() {
	init();

	system("pause");
	return 0;
}

void output_chromosome(Chromosome & c, func f) {
	unsigned i;

	cout << "[";
	for (i = 0; i < c.size() - 1; i++)
		cout << c[i] << ", ";
	cout << c[i] << ']' << "   val=" << f(c) << endl << endl;
}

void init() {
	Population start_population;
	Chromosome c;

	/* De Jong function */
	start_population = generate_population(POPULATION_SIZE, 2, -2.048, 2.048);
	c = process_function(start_population, DeJong, MAXIMUM, -2.048, 2.048);
	cout << "De Jong function result chromosome (1, 1): " << endl;
	output_chromosome(c, DeJong);

	/* GoldStein & Price function */
	start_population = generate_population(POPULATION_SIZE, 2, -2., 2.);
	c = process_function(start_population, GoldSteinPrice, MINIMUM, -2., 2.);
	cout << "GoldStein & Price function result chromosome (0, -1): " << endl;
	output_chromosome(c, GoldSteinPrice);

	/* Branin */
	start_population = generate_population(POPULATION_SIZE, 2, -5., 10.);
	c = process_function(start_population, Branin, MINIMUM, -5., 10.);
	cout << "Branin function result chromosome (anyof3, anyof3): " << endl;
	output_chromosome(c, Branin);

	/* Martin & Gaddy */
	start_population = generate_population(POPULATION_SIZE, 2, 0, 10.);
	c = process_function(start_population, MartinGaddy, MINIMUM, 0, 10.);
	cout << "Martin & Gaddy function result chromosome (5, 5): " << endl;
	output_chromosome(c, MartinGaddy);

	/* Rosenbrock */
	start_population = generate_population(POPULATION_SIZE, 2, -1.2, 1.2);
	c = process_function(start_population, RosenBrock, MINIMUM, -1.2, 1.2);
	cout << "Rosenbrock 1 function result chromosome (1, 1): " << endl;
	output_chromosome(c, RosenBrock);

	/* Rosenbrock 2 */
	start_population = generate_population(POPULATION_SIZE, 4, -1.2, 1.2);
	c = process_function(start_population, RosenBrock2, MINIMUM, -1.2, 1.2);
	cout << "Rosenbrock 2 function result chromosome (1, 1, 1, 1): " << endl;
	output_chromosome(c, RosenBrock2);

	/* Hyper sphere */
	start_population = generate_population(POPULATION_SIZE, 6, -5.12, 5.12);
	c = process_function(start_population, HyperSphere, MINIMUM, -5.12, 5.12);
	cout << "Hyper sphere function result chromosome (0, 0, 0, 0, 0, 0): " << endl;
	output_chromosome(c, HyperSphere);

	/* Grienwangk */
	start_population = generate_population(POPULATION_SIZE, 10, -512., 512.);
	c = process_function(start_population, Griewangk, MAXIMUM, -512., 512.);
	cout << "Griewangk function result chromosome (0, 0, 0, 0, 0, 0, 0, 0, 0, 0): " << endl;
	output_chromosome(c, Griewangk);
}