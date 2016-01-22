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

constexpr unit_base<units::meter> m_(1);
constexpr unit_base<units::meter> ft_(0.3048);
constexpr unit_base<units::second> s_(1);
constexpr unit_base<units::kilogram> Kg_(1);
constexpr auto N_ = (Kg_ * m_) / (s_ * s_);
constexpr auto Hz_ = 1 / s_;

int main()
{
	decltype(m_) distance = 12.5 * m_;
	decltype(m_ * m_) area = 2 * ft_ * distance;
	decltype(Kg_) mass = 18 * Kg_;
	decltype(N_) force = distance * mass / (1.2 * s_ * 1.8 * s_);
	
	// uncomment for an error
	// decltype(N_) notForce = mass / distance;
	
	// get the numerical part by dividing the quantity by its unit
	double squareFeet = area / (ft_ * ft_);
	
	// mutable unit types
	decltype(m_)::var mutableDistance = 3.2 * ft_;
	mutableDistance += distance;
}
