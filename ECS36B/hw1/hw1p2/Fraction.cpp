// Fraction.cpp
//
//

#include "Fraction.h"
#include <iostream>
#include <cmath>
#include <stdexcept>
using namespace std;

int gcd(int a, int b)
{
	if (a % b == 0)
		return b;
	return gcd(b, a % b);
}


Fraction::Fraction(int n, int d)
{
	if (d == 0)
		throw invalid_argument("zero denominator");

	if (n < 0 && d < 0)
	{
		n = -n;
		d = -d;
	}

	int g = gcd(n, d);
	num = n / g;
	den = d / g;
}

Fraction Fraction::operator + (const Fraction& f) const
{
	int new_num = num * f.den + den * f.num;
	int new_den = den * f.den;
	return Fraction(new_num, new_den);
}

Fraction Fraction::operator - (const Fraction& f) const
{
	int new_num = num * f.den - den * f.num;
	int new_den = den * f.den;
	return Fraction(new_num, new_den);
}


Fraction Fraction::operator * (const Fraction& f) const
{
	return Fraction(num * f.num, den * f.den);
}

Fraction Fraction::operator / (const Fraction& f) const
{
	return Fraction(num * f.den, den * f.num);
}

Fraction Fraction::operator + (int n) const
{
	int new_num = num + den * n;
	
	return Fraction(new_num, den);
}

Fraction Fraction::operator - (int n) const
{
	int new_num = num - den * n;

	return Fraction(new_num, den);
}

Fraction Fraction::operator * (int n) const
{
	return Fraction(num * n, den);
}

Fraction Fraction::operator / (int n) const
{
	return Fraction(num, den * n);
}

Fraction Fraction::operator = (int n)
{
	num = n;
	den = 1;
	return n;
}

Fraction Fraction::operator + () const
{
	return Fraction(num, den);
}

Fraction Fraction::operator - () const
{
	return Fraction(-num, den);
}


Fraction operator + (int n, const Fraction& f)
{
	return f + n;
}

Fraction operator - (int n, const Fraction& f)
{
	return -f + n;
}

Fraction operator * (int n, const Fraction& f)
{
	return f * n;
}

Fraction operator / (int n, const Fraction& f)
{
	return Fraction(n * f.den, f.num);
}

ostream& operator << (ostream& os, const Fraction& f)
{
	if (f.num * f.den < 0)
		os << '-';
	os << abs(f.num);

	if (abs(f.den) != 1)
		os << '/' << abs(f.den);
	
	return os;
}

int Fraction::getNum() const
{
	return abs(num);
}

int Fraction::getDen() const
{
	return abs(den);
}
