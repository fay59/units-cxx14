//
// example.cpp
// units-cxx14
// 
// Copyright (c) 2016 Félix Cloutier
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include <iomanip>
#include <iostream>
#include <type_traits>

#include "units.hpp"
#include "siunits.hpp"
#include "usunits.hpp"

using namespace std;

// These import *lots* of very short names. Be careful what you name your
// variables!
using namespace unitscxx::si;
using namespace unitscxx::us;

void newtons()
{
	// It's easy to refer to unit types using decltype.
	decltype(m) distance = 3*m;
	decltype(kg) mass = 15*kg;
	
	// Because kg is a constexpr, its decltype is const.
	cout << "Mass is constant: " << is_const<decltype(mass)>::value << '\n';
	
	// You can get a mutable type using the quantity's `var` typedef.
	decltype(s*s)::var squareSeconds(3.5);
	
	// Additions and subtractions must use the same unit. Unitless quantities
	// can't be added to or subtracted from quantities with units.
	squareSeconds += 1 * s*s;
	
	// It's easy to refer to unit types with decltype, but it's even simpler to
	// use auto.
	// (Just Newton's second law here.)
	auto force = mass * (distance / squareSeconds);
	
	// You can't assign a quantity with units to another quantity with the
	// wrong units.
	cout << "You can assign a force to a distance: "
		<< is_assignable<decltype(squareSeconds)&, decltype(force)>::value
		<< '\n';
	
	// To get a numerical value back from a quantity with units, you must
	// divide it by its defining unit.
	cout << (distance / m) << "m "
		<< "* " << (mass / kg) << "kg "
		<< "/ " << (squareSeconds / s / s) << "s^2 "
		<< "= " << (force / N) << "N\n";
	
	cout << '\n';
}

void recipe()
{
	// US units are defined in terms of SI units. This means that their types
	// are compatible.
	decltype(L) flour = (1. + 2./3.) * cp;
	
	// Some units for fluid volumes are in the us::fl namespace. This is because
	// these have a different meaning for dry quantities (which are in the
	// us::dry namespace).
	cout << "One fluid pint is " << (fl::pt / (milli()*L)) << "mL; "
		<< "one dry pint is " << (dry::pt / (milli()*L)) << "mL.\n";
	
	auto mL = milli() * L;
	decltype(cp) sugar = 355 * mL; // using "cp" to prove the point
	decltype(m*m*m) cocoa = 2./3. * cp; // using "m*m*m" for the same reason
	decltype(mL) bakingSoda = 1.5 * tsp; // you get the gist
	auto salt = 1 * tsp;
	auto buttermilk = 1.5 * cp;
	auto vegetableOilShortening = 0.5 * cp;
	auto vanilla = 1 * tsp;
	auto kirsch = cp / 2;
	
	auto totalVolume = flour + sugar + cocoa + bakingSoda + salt + buttermilk
		+ vegetableOilShortening + vanilla + kirsch;
	
	cout << "Total volume is "
		<< (totalVolume / L) << "L, "
		<< (totalVolume / cp) << " cps, "
		<< (totalVolume / fl::oz) << " fl oz, or "
		<< (totalVolume / Tbsp) << " tablespoons.\n";
	
	cout << '\n';
}

void temperature()
{
	// Temperature is special because scales don't all start at zero.
	// This means that we must rely on functions to convert from one unit to the
	// other.
	// The base headers only define units for Kelvin; other temperature
	// quantites are unitless and must be converted to Kelvin to be useful.
	// The functions F2K and C2K convert from Farenheit and Celsius to Kelvin,
	// and K2F and K2C do the opposite.
	// si::Czero, though not used here, is the Kelvin temperature for zero
	// Celsius.
	
	double todayInCelsius = 22.5;
	auto todayInKelvin = C2K(todayInCelsius);
	auto todayInFarenheit = K2F(todayInKelvin);
	cout << "Today's temperature is ";
	cout << todayInCelsius << "°C, or ";
	cout << (todayInKelvin / K) << "K, or ";
	cout << todayInFarenheit << "F.\n";
	
	double roomTemperatureInFarenheit = 71;
	auto roomTemperatureInKelvin = F2K(roomTemperatureInFarenheit);
	auto roomTemperatureInCelsius = K2C(roomTemperatureInKelvin);
	cout << "Normal room temperature is ";
	cout << roomTemperatureInCelsius << "°C, or ";
	cout << (roomTemperatureInKelvin / K) << "K, or ";
	cout << roomTemperatureInFarenheit << "F.\n";
}

int main()
{
	cout << boolalpha;
	
	newtons();
	recipe();
	temperature();
}
