// OptionData.hpp
//
// Encapsulate option data.

#ifndef OptionData_HPP
#define OptionData_HPP

#include <algorithm> // for max()
using namespace std;

// Encapsulate all data in one place
struct OptionData
{ // Option data + behaviour

	double K;	// Strike price
	double T;	// Time to maturity
	double r;	// Drift or risk free interest rate (assumed equal in BS Market)
	double sig; // Volatility in SDE

	// Extra data
	double H;		// down and out barrier
	double D;		// dividend
	double betaCEV; // elasticity factor (CEV model)
	double scale;	// scale factor in CEV model

	int type; // 1 == call, -1 == put

	// Payoff function
	double myPayOffFunction(double S)
	{
		if (type == 1)
		{ // Call
			return max(S - K, 0.0);
		}
		else
		{ // Put
			return max(K - S, 0.0);
		}
	}
};

#endif