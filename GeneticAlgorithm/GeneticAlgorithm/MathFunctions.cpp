#include "GeneticFunctions.h"

double sqr(double x) {
	return x*x;
}

// Functions to calculate
double DeJong(Chromosome & x) {
	return 3905.93 - 100 * sqr(x[0] * x[0] - x[1]) - sqr(1 - x[0]);
}

double GoldSteinPrice(Chromosome & x)
{
	return (1 + sqr(x[0] + x[1] + 1)*(19 - 14 * x[0] + 3 * x[0] * x[0] - 14 * x[1] + 6 * x[0] * x[1] + 3 * x[1] * x[1])) *
		(30 + sqr(2 * x[0] - 3 * x[1]) * (18 - 32 * x[0] + 12 * x[0] * x[0] + 48 * x[1] - 36 * x[0] * x[1] + 27 * x[1] * x[1]));
}

double Branin(Chromosome & x)
{
	const double b = 5.1 / 4 * 7. * 7. / 22 / 22;
	const double c = 5. / 22 * 7;
	const double d = 6;
	const double e = 10;
	const double f = 1. / 8 * 7. / 22;

	return sqr(x[1] - b*x[0] * x[0] + c*x[0] - d) + e*(1 - f)*cos(x[0]) + e;
}

double MartinGaddy(Chromosome & x)
{
	return (x[0] - x[1]) * (x[0] - x[1]) + ((x[0] + x[1] - 10) / 3.) * ((x[0] + x[1] - 10) / 3.);
}

double RosenBrock(Chromosome & x)
{
	return 100 * (x[0] * x[0] - x[1]) * (x[0] * x[0] - x[1]) + (1 - x[0]) * (1 - x[0]);
}

double RosenBrock2(Chromosome & x)
{
	double result = 0.;

	for (int i = 0; i < 3; i++) 
		result += 100 * (x[i] * x[i] - x[i + 1]) * (x[i] * x[i] - x[i + 1]) + (1 - x[i]) * (1 - x[i]);

	return result;
}

double HyperSphere(Chromosome & x)
{
	double result = 0.;

	for (int i = 0; i < 6; i++)
		result += (x[i] * x[i]);

	return result;
}

double Griewangk(Chromosome & x)
{
	double sum = 0.;
	double mult = 1.;

	for (int i = 0; i < 10; i++) {
		sum += (x[i] * x[i] / 4000.);
		mult *= cos(x[i] / sqrt(i + 1.));
	}

	return 1. / (0.1 + (sum - mult + 1.));
}
