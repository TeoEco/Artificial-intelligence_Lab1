#pragma once
#include <vector>
#include <iostream>

using namespace std;

typedef vector<double> Chromosome;
typedef vector<Chromosome> Population;
typedef double(*func)(Chromosome & c);
typedef enum {MAXIMUM, MINIMUM} Condition;

const int POPULATION_SIZE = 400;
const int ELITISM_SIZE = 8;
const int CHALLENGE_ROUNDS = POPULATION_SIZE - ELITISM_SIZE;

const int GENERATIONS_NUMBER = 100;
const int MUTATION_POSSIBILITY = 5;
const double ACCURACY = 0.001;

Chromosome process_function(Population &start_population, func f, Condition cond, double lbound, double rbound);
Population generate_population(unsigned size, unsigned genes, double lbound, double rbound);

double DeJong(Chromosome & x);
double GoldSteinPrice(Chromosome & x);
double Branin(Chromosome & x);
double MartinGaddy(Chromosome & x);
double RosenBrock(Chromosome & x);
double RosenBrock2(Chromosome & x);
double HyperSphere(Chromosome & x);
double Griewangk(Chromosome & x);