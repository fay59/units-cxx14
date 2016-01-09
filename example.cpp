#include "units.hpp"

enum class Units
{
	Meter,
	Second,
	Kilogram,
};

template<Units U, typename NumericType = double>
using BaseUnit = UnitSkeleton<Units, U, NumericType>;

BaseUnit<Units::Meter> _m(1);
BaseUnit<Units::Meter> _ft(0.3048);
BaseUnit<Units::Second> _s(1);
BaseUnit<Units::Kilogram> _Kg(1);
auto _N = (_Kg * _m) / (_s * _s);

int main()
{
	decltype(_m) distance = 12.5 * _m;
	decltype(_m * _m) area = 2 * _ft * distance;
	decltype(_Kg) mass = 18 * _Kg;
	decltype(_N) force = mass * distance / (1.2 * _s * 1.8 * _s);
	
	// uncomment for an error
	// decltype(_N) notForce = mass / distance;
}
