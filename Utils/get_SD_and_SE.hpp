#ifndef GET_SD_AND_SE_H
#define GET_SD_AND_SE_H

#include <string>
#include <iostream>
#include <vector>
#include <cmath>

#include <boost/tuple/tuple.hpp>	// Tuple class
#include <boost/tuple/tuple_io.hpp> // I/O for tuples

using namespace std;
using boost::tuple;

// Function to get Standard Deviation and Standard Error
boost::tuple<double, double> get_SD_and_SE(vector<double> input, double r, double T)

{
	double input_size = input.size();
	double sum_prices = 0;
	double sumsquare_prices = 0;

	for (int i = 0; i < input_size; i++)
	{
		sum_prices += input[i];
		sumsquare_prices += pow(input[i], 2);
	}

	double root = sqrt(sumsquare_prices - (pow(sum_prices, 2) / input_size));
	double SD = root * exp(-r * T) / (input_size - 1.0);
	double SE = SD / sqrt(input_size);

	return boost::tuple<double, double>(SD, SE);
}

#endif