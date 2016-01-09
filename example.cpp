//
// units.cpp
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

#include "units.hpp"

enum class units
{
	meter,
	second,
	kilogram,
};

template<units U>
using unit_base = unitscxx::unit_base<double, units, U>;

unit_base<units::meter> _m(1);
unit_base<units::meter> _ft(0.3048);
unit_base<units::second> _s(1);
unit_base<units::kilogram> _Kg(1);
auto _N = (_Kg * _m) / (_s * _s);

int main()
{
	decltype(_m) distance = 12.5 * _m;
	decltype(_m * _m) area = 2 * _ft * distance;
	decltype(_Kg) mass = 18 * _Kg;
	decltype(_N) force = distance * mass / (1.2 * _s * 1.8 * _s);
	
	// uncomment for an error
	// decltype(_N) notForce = mass / distance;
}
