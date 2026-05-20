// Aircrew.cpp
//

#include "Aircrew.h"
#include <iostream>
using namespace std;

Aircrew* Aircrew::makeAircrew(char ch, std::string name_str)
{
	Aircrew* pa = nullptr;
	if (ch == 'P')
		pa = new Pilot(name_str);
	else if (ch == 'A')
		pa = new Attendant(name_str);
	else if (ch == 'T')
		pa = new TaggedAttendant(name_str);

	return pa;
}

Aircrew::Aircrew(string name_str) : nm(name_str) 
{
	flightsTaken = 0;
	hoursWorked = 0;
}

const double Aircrew::maxHours(void) const
{
	return 60;
}

const string Aircrew::name(void) const
{
	return nm;
}

void Aircrew::setFlights(int i)
{
	flightsTaken = i;
}

void Aircrew::setHours(double h)
{
	hoursWorked = h;
}

void Aircrew::print(void) const
{
	cout << type() << ": " << nm <<
		" has operated " << flightsTaken <<
		" flights for a total of " << hoursWorked << " hours this week" << endl;
	cout << "Available flights: " << maxFlights() - flightsTaken << endl;
	cout << "Available hours: " << maxHours() - hoursWorked << endl;
}

void Aircrew::scheduleFlight(int f, double h) const
{
	cout << "Attempting to schedule for " << f << " stop(s) "
		<< h << " hours flight..." << endl;
	if (f + 1 + flightsTaken <= maxFlights() && h + hoursWorked <= maxHours())
		cout << "This crew member can be scheduled" << endl;
	else
		cout << "This crew member should be replaced" << endl;
	cout << "Done scheduling " << nm << endl;
}

Aircrew::~Aircrew(void) 
{

}


Pilot::Pilot(string name_str): Aircrew(name_str)
{

}

const string Pilot::type(void) const
{
	return "Pilot";
}

const int Pilot::maxFlights(void) const
{
	return 5;
}


Attendant::Attendant(string name_str): Aircrew(name_str)
{

}

const string Attendant::type(void) const
{
	return "Attendant";
}

const int Attendant::maxFlights(void) const
{
	return 8;
}

TaggedAttendant::TaggedAttendant(string name_str): Aircrew(name_str)
{

}

const string TaggedAttendant::type(void) const
{
	return "TaggedAttendant";
}

const int TaggedAttendant::maxFlights(void) const
{
	return 8;
}

void TaggedAttendant::scheduleFlight(int f, double h) const
{
	cout << "Attempting to schedule for " << f << " stop(s) "
		<< h << " hours flight..." << endl;
	if (f + 1 + flightsTaken <= maxFlights() && h/2 + hoursWorked <= maxHours())
		cout << "This crew member can be scheduled" << endl;
	else
		cout << "This crew member should be replaced" << endl;
	cout << "Done scheduling " << nm << endl;
}











