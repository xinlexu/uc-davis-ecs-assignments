// Angle.cpp
//

#include "Angle.h"
#include <iostream>
using namespace std;

Angle::Angle(void) {
	theta = 30;
}

int Angle::get(void) {
	return theta;
}

void Angle::change(int dt) {
	set(theta + dt);
}

void Angle::print(void) {
	cout << "angle: " << theta << endl;
}

void Angle::set(int t) {
	if (t < -120)
		theta = -120;
	else if (t > 60)
		theta = 60;
	else
		theta = t;
}


