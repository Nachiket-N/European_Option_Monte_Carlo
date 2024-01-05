#include <iostream>
#include <cmath>
#include <boost/tuple/tuple.hpp>	// Tuple class
#include <boost/tuple/tuple_io.hpp> // I/O for tuples

#include "Utils/OptionData.hpp"
#include "Utils/NormalGenerator.cpp"
#include "Utils/Range.cpp"
#include "Utils/get_SD_and_SE.hpp"

using boost::tuple;

template <class T>
void print(const std::vector<T> &myList)
{ // A generic print function for vectors

	std::cout << std::endl
			  << "Size of vector is " << myList.size() << "\n[";

	// We must use a const iterator here, otherwise we get a compiler error.
	const typename std::vector<T>::const_iterator i;
	for (i = myList.begin(); i != myList.end(); ++i)
	{
		std::cout << *i << ",";
	}

	std::cout << "]\n";
}

namespace SDEDefinition
{ // Defines drift + diffusion + data

	OptionData *data; // The data for the option MC

	double drift(double t, double X)
	{ // Drift term

		return (data->r) * X; // r - D
	}

	double diffusion(double t, double X)
	{ // Diffusion term

		double betaCEV = 1.0;
		return data->sig * pow(X, betaCEV);
	}

	double diffusionDerivative(double t, double X)
	{ // Diffusion term, needed for the Milstein method

		double betaCEV = 1.0;
		return 0.5 * (data->sig) * (betaCEV)*pow(X, 2.0 * betaCEV - 1.0);
	}
} // End of namespace

int main()
{
	std::cout << "1 factor MC with explicit Euler\n";
	OptionData myOption;
	myOption.K = 150;
	myOption.T = 1.28;
	myOption.r = 0.04;
	myOption.sig = 0.27;
	myOption.type = -1; // Put -1, Call +1
	double S_0 = 155;

	long N = 900;
	std::cout << "Number of subintervals in time: ";
	std::cin >> N;

	// Create the basic SDE (Context class)
	Range<double> range(0.0, myOption.T);
	double VOld = S_0;
	double VNew;

	std::vector<double> x = range.mesh(N);

	// V2 mediator stuff
	long NSim = 50000;
	std::cout << "Number of simulations: ";
	std::cin >> NSim;

	double k = myOption.T / double(N);
	double sqrk = sqrt(k);

	// Normal random number
	double dW;
	double price = 0.0; // Option price

	// NormalGenerator is a base class
	NormalGenerator *myNormal = new BoostNormal();

	using namespace SDEDefinition;
	SDEDefinition::data = &myOption;

	int coun = 0; // Number of times S hits origin

	/****************************************************************************************/
	// Vector to store calculated prices using Monte carlo. This vector will be passed to find SD and SE values.
	vector<double> storeprice;
	/****************************************************************************************/

	// A.
	for (long i = 1; i <= NSim; ++i)
	{ // Calculate a path at each iteration

		if ((i / 10000) * 10000 == i)
		{ // Give status after each 1000th iteration

			std::cout << i << std::endl;
		}

		VOld = S_0;
		for (unsigned long index = 1; index < x.size(); ++index)
		{

			// Create a random number
			dW = myNormal->getNormal();

			// The FDM (in this case explicit Euler)
			VNew = VOld + (k * drift(x[index - 1], VOld)) + (sqrk * diffusion(x[index - 1], VOld) * dW);

			VOld = VNew;

			// Spurious values
			if (VNew <= 0.0)
				coun++;
		}

		double tmp = myOption.myPayOffFunction(VNew);

		/////////////////////////////////////////////////////////////////////////////////////
		// Storing price values for each simulation to pass to the get_SD_and_SE.h fucntion.
		storeprice.push_back(tmp);
		////////////////////////////////////////////////////////////////////////////////////

		price += (tmp) / double(NSim);
	}

	// D. Finally, discounting the average price
	price *= exp(-myOption.r * myOption.T);

	/****************************************************************************************/
	boost::tuple<double, double> results = get_SD_and_SE(storeprice, myOption.r, myOption.T);
	/****************************************************************************************/

	// Cleanup; V2 use scoped pointer
	delete myNormal;

	std::cout << "Price, after discounting: " << price << ", " << std::endl;
	std::cout << "Number of times origin is hit: " << coun << endl;
	std::cout << "Standard Deviation: " << results.get<0>() << endl;
	std::cout << "Standard Error: " << results.get<1>() << endl;

	return 0;
}

//  Results:

//  Number of sub intervals: 900
/*
Number of simulations: 100 000

For Batch 1:
	Call Price: 2.11662
		SD: 0.0142078
		SE: 4.4929e-005
	Put Price: 5.87702
		SD: 0.019153
		SE: 6.05672e-005

For Batch 2:
Call Price: 7.91053
		SD: 0.0414185
		SE: 0.000130977
	Put Price: 8.01616
		SD: 0.0414185
		SE: 0.000130977
Number of times origin is hit: 0
Standard Deviation: 0.00322095
Standard Error: 1.01855e-005

For Batch 3:
Call Price: 0.200903
		SD: 0.00322095
		SE: 1.01855e-005
	Put Price: 4.08337
		SD: 0.00662296
		SE: 2.09436e-005

For Batch 4:
Call Price: 91.3185
		SD: 1.14818
		SE: 0.00363085
	Put Price: 1.26111
		SD: 0.00779842
		SE: 2.46608e-005

*/
