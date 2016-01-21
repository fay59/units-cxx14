# units-cxx14

Units-cxx14 is a C++14 header library that allows you to put units on your
numbers and enforce that you don't mix apples with oranges at compile-time. It
requires no runtime support: your arithmetic operations are exactly as fast as
they would be without units.

The design was inspired by the TI Voyage 200 calculator's capability to handle
units (which is possibly present in later models as well). For instance, writing
`150_m / 3_s` would give you `50 • _m/_s`. This became invaluable for physics
classes.

## Example

This example is the same as the one that lives in the example.cpp file.

```C++
#include "units.hpp"

// Define some base units that can't be conveniently derived from a
// combination of units.
enum class units
{
	meter,
	second,
	kilogram,
};

// unit_base is (almost) the only type from unitscxx you need to care about.
// You use it to create base units.
template<units U>
using unit_base = unitscxx::unit_base<double, units, U>;

// Create variables that represent each unit that you want to use in
// calculations. Constexpr means that you can easily have them in a header.
constexpr unit_base<units::meter> _m(1);
constexpr unit_base<units::second> _s(1);
constexpr unit_base<units::kilogram> _Kg(1);

// The parameter is the "numeric part" of the variable. For instance, you
// can define a foot as being 0.3048 meters.
constexpr unit_base<units::meter> _ft(0.3048);

// Units can also be derived from other units easily.
constexpr auto _N = (_Kg * _m) / (_s * _s);
constexpr auto _Hz = 1 / _s;

int main()
{
	// The simplest and cleanest way to reference a unit type is to use
	// decltype...
	
	// all good
	decltype(_m) distance = 12.5 * _m;
	decltype(_m * _m) area = 2 * _ft * distance;
	decltype(_Kg) mass = 18 * _Kg;
	decltype(_N) force = distance * mass / (1.2 * _s * 1.8 * _s);

	// bad
	decltype(_N) notForce = mass / distance;
	
	// ... but decltype on a constexpr is a const type. This may or may not
	// be a problem to you: since multiplications and divisions return
	// values of a different type, chances are that you'll need a new
	// variable anyway.
	// If you need a mutable variable, you can use the ::var type member:
	decltype(_m)::var mutableDistance = 3.2 * _ft;
	mutableDistance += distance;
}
```

## License

MIT