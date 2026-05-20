// Airline.cpp
//

#include <iostream>
#include <exception>
#include "Airline.h"
using namespace std;

Airline::Airline(int nA321, int nB777, int nB787): nAirplanes(nA321 + nB777 + nB787)
{
	airplaneList = new Airplane * [nAirplanes + 10];

	int i;
	for (i = 1; i <= nA321; i++)
		airplaneList[i] = new Airplane(10);
	for (; i <= nA321 + nB777; i++)
		airplaneList[i] = new Airplane(32);
	for (; i <= nAirplanes; i++)
		airplaneList[i] = new Airplane(40);

	for (i = 1; i <= nAirplanes; i++)
	{
		cout << "Airplane " << i << " maximum load " << 
			airplaneList[i]->maxLoad() << endl;
	}


}


void Airline::addShipment(int size)
{
	if (size < 0)
		throw invalid_argument("must be positive");

	int i;
	for (i = 1; i <= nAirplanes; i++)
	{
		if (airplaneList[i]->addContainers(size))
		{
			cout << size << " containers added to airplane "
				<< i << endl;
			return ;
		}
	}
	
	cout << " could not fit " << size << " containers" << endl; 
}

void Airline::printSummary(void) const
{
	cout << "Summary:\n";
	int i;
	for (i = 1; i <= nAirplanes; i++)
	{
		if (airplaneList[i]->currentLoad() != 0)
			cout << "airplane " << i << ' ' << 
			airplaneList[i]->currentLoad() << '/' << 
			airplaneList[i]->maxLoad() << endl;
	}
}

Airline::~Airline(void)
{
	int i;
	for (i = 1; i <= nAirplanes; i++)
	{
		cout << "deallocating airplane " << i << endl;
		delete airplaneList[i];
		airplaneList[i] = nullptr;
	}
		
	delete[] airplaneList;
	airplaneList = nullptr;
}














