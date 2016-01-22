//
// usunits.hpp
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

#ifndef USUNITS_HPP
#define USUNITS_HPP

#include <ratio>
#include "units.hpp"
#include "siunits.hpp"

// UNITSCXX_SI_ARITHMETIC_TYPE defined in siunits.hpp (if not before)

#ifndef UNITSCXX_US_FOOT_LENGTH
// Use the US survey foot by default
#define UNITSCXX_US_FOOT_LENGTH UNITSCXX_SI_ARITHMETIC_TYPE(0.30480061)
#endif

namespace unitscxx
{
namespace us
{
#pragma mark - Units of length
	constexpr auto ft = UNITSCXX_US_FOOT_LENGTH * si::m;
	constexpr auto in = ft / 12;
	constexpr auto pica = in / 6;
	constexpr auto p = pica / 12;
	constexpr auto yd = 3 * ft;

	constexpr auto li =
		UNITSCXX_SI_ARITHMETIC_TYPE(33) / UNITSCXX_SI_ARITHMETIC_TYPE(50) * ft;
	constexpr auto rd = 25 * li;
	constexpr auto ch = 4 * rd;
	constexpr auto fur = 10 * ch;
	constexpr auto mi = 8 * fur;
	constexpr auto lea = 3 * mi;
	
	constexpr auto ftm = 2 * yd;
	constexpr auto cb = 120 * ftm;
	constexpr auto nmi = 1.151 * mi;
	
#pragma mark - Units of area
	constexpr auto acre = ch * fur;
	constexpr auto section = mi * mi;
	constexpr auto twp = 36 * section;
	
#pragma mark - Units of fluid volume
	constexpr auto min = 61.611519921875 * (std::micro() * si::L);
	constexpr auto tsp = 80 * min;
	constexpr auto Tbsp = 3 * tsp;
	constexpr auto jig = 3 * Tbsp;
	namespace fl
	{
		constexpr auto oz = 2 * Tbsp;
	}
	constexpr auto gi = 4 * fl::oz;
	constexpr auto cp = 2 * gi;
	namespace fl
	{
		constexpr auto pt = 2 * cp;
		constexpr auto qt = 2 * pt;
		constexpr auto gal = 4 * qt;
	}	
	constexpr auto hogshead = 63 * fl::gal;
	namespace fl
	{
		constexpr auto bbl = hogshead / 2;
	}
	constexpr auto oilbbl = hogshead * 2 / 3;
	
#pragma mark - Units of dry volume
	namespace dry
	{
		constexpr auto pt = 33.6 * in * in * in;
		constexpr auto qt = 2 * pt;
		constexpr auto gal = 4 * qt;
	}
	constexpr auto pk = 2 * dry::gal;
	constexpr auto bu = 4 * pk;
	namespace dry
	{
		constexpr auto bbl = 7056 * in * in * in;
	}

#pragma mark - Units of mass
	constexpr auto lb = 453.59237 * si::g;
	constexpr auto oz = lb / 16;
	constexpr auto dr = oz / 16;
	constexpr auto gr = lb / 7000;
	constexpr auto cwt = 100 * lb;
	constexpr auto ton = 20 * cwt;
	constexpr auto dwt = 24 * gr;
	constexpr auto ozt = 20 * dwt;
	constexpr auto lbt = 12 * ozt;

#pragma mark - Temperature
	// Use when you want to convert a Farenheit temperature difference to
	// a Kelvin temperature difference
	constexpr decltype(si::K) Fdelta(UNITSCXX_SI_ARITHMETIC_TYPE value)
	{
		return value / 1.8 * si::K;
	}
	
	constexpr decltype(si::K) F2K(UNITSCXX_SI_ARITHMETIC_TYPE value)
	{
		return Fdelta(value - 32) + si::Czero;
	}
	
	constexpr UNITSCXX_SI_ARITHMETIC_TYPE K2F(decltype(si::K) value)
	{
		return ((value - si::Czero) / si::K) * 1.8 + 32;
	}
}
}

#endif