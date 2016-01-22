//
// siunits.hpp
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

#ifndef SIUNITS_HPP
#define SIUNITS_HPP

#include <cmath>
#include <ratio>
#include "units.hpp"

#ifndef UNITSCXX_SI_ARITHMETIC_TYPE
#define UNITSCXX_SI_ARITHMETIC_TYPE double
#endif

namespace unitscxx
{
namespace si
{
#pragma mark - Base units
	enum units
	{
		meter,
		gram, // the actual "base" unit is the kilogram
		second,
		ampere,
		kelvin,
		mole,
		candela,
	};
	
	template<units U>
	using si_base = unit_base<UNITSCXX_SI_ARITHMETIC_TYPE, units, U>;
	
	constexpr si_base<units::meter> m(1);
	constexpr si_base<units::gram> g(1);
	constexpr si_base<units::second> s(1);
	constexpr si_base<units::ampere> A(1);
	constexpr si_base<units::kelvin> K(1);
	constexpr si_base<units::mole> mol(1);
	constexpr si_base<units::candela> cd(1);
	// (little bit of cheating here)
	constexpr auto kg = std::kilo() * g;
	
#pragma mark - Named derived units
	// skipping radian: rad = m / m
	// skipping steradian: sr = (m*m) / (m*m)
	constexpr auto Hz = 1 / s;
	constexpr auto N = kg * m / (s * s);
	constexpr auto Pa = N / (m * m);
	constexpr auto J = N * m;
	constexpr auto W = J / s;
	constexpr auto C = s * A;
	constexpr auto V = W / A;
	constexpr auto F = C / V;
	constexpr auto Ohm = V / A; // for lack of a better Ω replacement
	constexpr auto S = A / V;
	constexpr auto Wb = V * s;
	constexpr auto T = Wb / (m * m);
	constexpr auto H = Wb / A;
	// skipping lumen: lm = (cd * sr)
	constexpr auto lx = cd / (m * m);
	// skipping becquerel: Bq = 1 / s
	constexpr auto Gy = J / kg;
	// skipping sievert: Sv = J / kg
	constexpr auto kat = mol / s;
	
	// special treatment for Celsius
	constexpr auto Czero = 273.15 * K;
	constexpr decltype(K) C2K(UNITSCXX_SI_ARITHMETIC_TYPE celsiusTemperature)
	{
		return celsiusTemperature * K + Czero;
	}
	
	constexpr UNITSCXX_SI_ARITHMETIC_TYPE K2C(decltype(K) kelvinTemperature)
	{
		return (kelvinTemperature - Czero) / K;
	}
	
#pragma mark - Commonly-accepted non-standard units
	namespace detail
	{
		constexpr auto dm = std::deci() * m;
		constexpr auto hm = std::hecto() * m;
	}
	
	constexpr auto deg = UNITSCXX_SI_ARITHMETIC_TYPE(180) / M_PI;
	constexpr auto ha = detail::hm * detail::hm;
	constexpr auto L = detail::dm * detail::dm * detail::dm;
	constexpr auto t = std::mega() * kg;
	constexpr auto au = 1.496e11 * m;
}
}

#endif