# units-cxx14

Units-cxx14 is a C++14 header library that allows you to put units on your
numbers and enforce that you don't mix apples with oranges at compile-time. It
requires no runtime support: your arithmetic operations are exactly as fast as
they would be without units.

The design was inspired by the TI Voyage 200 calculator's capability to handle
units (which is possibly present in later models as well). For instance, writing
`150_m / 3_s` would give you `50 • _m/_s`. This became invaluable for physics
classes.

Since C++ reserves names starting with an underscore in the global namespace
(and *all* names starting with an underscore followed by an uppercase letter
in any namespace) per §17.6.4.3.2, these examples put the underscore at the end
of the identifier (so `m_` instead of `_m`).

## Example

This is how you would define units from scratch, if you don't want to use
siunits.hpp (or usunits.hpp). The example.cpp file has examples for how to use
US units.

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
constexpr unit_base<units::meter> m_(1);
constexpr unit_base<units::second> s_(1);
constexpr unit_base<units::kilogram> Kg_(1);

// The parameter is the "numeric part" of the variable. For instance, you
// can define a foot as being 0.3048 meters.
constexpr unit_base<units::meter> ft_(0.3048);

// Units can also be derived from other units easily.
constexpr auto N_ = (Kg_ * m_) / (s_ * s_);
constexpr auto Hz_ = 1 / s_;

int main()
{
	// The simplest and cleanest way to reference a unit type is to use
	// decltype...
	
	// all good
	decltype(m_) distance = 12.5 * m_;
	decltype(m_ * m_) area = 2 * ft_ * distance;
	decltype(Kg_) mass = 18 * Kg_;
	decltype(N_) force = distance * mass / (1.2 * s_ * 1.8 * s_);

	// bad
	decltype(N_) notForce = mass / distance;
	
	// ... but decltype on a constexpr is a const type. This may or may not
	// be a problem to you: since multiplications and divisions return
	// values of a different type, chances are that you'll need a new
	// variable anyway.
	// If you need a mutable variable, you can use the ::var type member:
	decltype(m_)::var mutableDistance = 3.2 * ft_;
	mutableDistance += distance;
}
```

## License

MIT