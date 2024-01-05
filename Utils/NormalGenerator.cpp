// NormalGenerator.cpp
// Class for generating vectors & matrices of random numbers
//
// This hierarchy uses the Template Method Pattern and it
// delegates to a Strategy pattern for generating uniform
// random numbers.
//
// The solution is object-oriented and uses run-time polymorphic
// functions.

#include "NormalGenerator.hpp"
#include <cmath>

BoostNormal::BoostNormal() : NormalGenerator()
{
	rng = boost::lagged_fibonacci607();
	nor = boost::normal_distribution<>(0, 1);
	myRandom = new boost::variate_generator<boost::lagged_fibonacci607 &, boost::normal_distribution<>>(rng, nor);
}

// Implement (variant) hook function
double BoostNormal::getNormal() const
{
	return (*myRandom)();
}

BoostNormal::~BoostNormal()
{

	delete myRandom;
}
