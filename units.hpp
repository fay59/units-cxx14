//
// units.hpp
// units-cxx11
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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include <type_traits>
#include <utility>

using namespace std;

#pragma mark - Sequence type
// (sequence only works with integers, not enums)

template<typename T, T... S>
struct sequence
{
	typedef T value_type;
	static constexpr size_t size = sizeof...(S);
};

#pragma mark - Test if a sequence contains a value
template<typename IntegerType, IntegerType I, IntegerType... S>
struct sequence_contains_type;

template<typename IntegerType, IntegerType I, IntegerType S1, IntegerType... Ss>
struct sequence_contains_type<IntegerType, I, S1, Ss...>
	: conditional<I == S1,
		true_type,
		typename sequence_contains_type<IntegerType, I, Ss...>::type>::type
{
};

template<typename IntegerType, IntegerType I>
struct sequence_contains_type<IntegerType, I> : false_type
{
};

template<typename IntegerType, IntegerType I, IntegerType... Is>
constexpr bool sequence_contains(sequence<IntegerType, Is...>)
{
	return sequence_contains_type<IntegerType, I, Is...>::value;
}

#pragma mark - Combine sequences
template<typename Seq1, typename Seq2>
struct combine;

template<typename IntegerType, IntegerType... S1, IntegerType... S2>
struct combine<sequence<IntegerType, S1...>, sequence<IntegerType, S2...>>
{
	typedef sequence<IntegerType, S1..., S2...> type;
};

template<typename Seq1, typename Seq2>
constexpr auto combine_sequences(Seq1, Seq2)
{
	return typename combine<Seq1, Seq2>::type{};
}

#pragma mark - Remove an item
template<typename Seq, typename IntegerType, IntegerType I>
struct left_of;

template<typename IntegerType, IntegerType I>
struct left_of<sequence<IntegerType>, IntegerType, I>
{
	typedef sequence<IntegerType> type;
};

template<typename IntegerType, IntegerType I, IntegerType S1, IntegerType... Ss>
struct left_of<sequence<IntegerType, S1, Ss...>, IntegerType, I>
{
	typedef typename conditional<I == S1,
		sequence<IntegerType>,
		typename combine<
			sequence<IntegerType, S1>,
			typename left_of<sequence<IntegerType, Ss...>, IntegerType, I>::type
	>::type>::type type;
};

template<typename Seq, typename IntegerType, IntegerType I>
struct right_of;

template<typename IntegerType, IntegerType I>
struct right_of<sequence<IntegerType>, IntegerType, I>
{
	typedef sequence<IntegerType> type;
};

template<typename IntegerType, IntegerType I, IntegerType S1, IntegerType... Ss>
struct right_of<sequence<IntegerType, S1, Ss...>, IntegerType, I>
{
	typedef typename conditional<I == S1,
		sequence<IntegerType, Ss...>,
		typename right_of<sequence<IntegerType, Ss...>, IntegerType, I>::type
	>::type type;
};

template<typename Seq, typename IntegerType, IntegerType I>
struct removed_from
{
	typedef typename combine<
		typename left_of<Seq, IntegerType, I>::type,
		typename right_of<Seq, IntegerType, I>::type>::type type;
};

#pragma mark - Remove identical terms
template<typename Seq1, typename Seq2>
struct remove_intersection;

template<typename IntegerType1, IntegerType1... Ones, typename IntegerType2>
struct remove_intersection<sequence<IntegerType1, Ones...>, sequence<IntegerType2>>
{
	typedef sequence<IntegerType1, Ones...> type;
};

template<typename IntegerType1, IntegerType1... Ones, typename IntegerType2, IntegerType2 Two, IntegerType2... Twos>
struct remove_intersection<sequence<IntegerType1, Ones...>, sequence<IntegerType2, Two, Twos...>>
{
	typedef typename removed_from<
		typename remove_intersection<
			sequence<IntegerType1, Ones...>,
			sequence<IntegerType2, Twos...>
		>::type,
		IntegerType2, Two
	>::type type;
};

#pragma mark - Sort a sequence
template<typename Sequence>
struct sequence_max;

template<typename IntegerType, IntegerType Max>
struct sequence_max<sequence<IntegerType, Max>>
{
	static constexpr IntegerType value = Max;
};

template<typename IntegerType, IntegerType S1, IntegerType S2, IntegerType... Ss>
struct sequence_max<sequence<IntegerType, S1, S2, Ss...>>
{
	static constexpr IntegerType value = sequence_max<sequence<IntegerType, (S1 > S2 ? S1 : S2), Ss...>>::value;
};

template<typename Sequence>
struct sorted;

template<typename IntegerType>
struct sorted<sequence<IntegerType>>
{
	typedef sequence<IntegerType> type;
};

template<typename IntegerType, IntegerType S1>
struct sorted<sequence<IntegerType, S1>>
{
	typedef sequence<IntegerType, S1> type;
};

template<typename IntegerType, IntegerType S1, IntegerType... Ss>
struct sorted<sequence<IntegerType, S1, Ss...>>
{
private:
	typedef sequence<IntegerType, S1, Ss...> Seq;
	static constexpr IntegerType Max = sequence_max<Seq>::value;

public:
	typedef typename combine<
		typename sorted<typename removed_from<Seq, IntegerType, Max>::type>::type,
		sequence<IntegerType, Max>>::type type;
};

#pragma mark - Units
template<typename UnitEnum, UnitEnum... S>
using UnitProduct = typename sorted<sequence<UnitEnum, S...>>::type;

template<typename Den, typename Num>
struct UnitQuotient
{
	typedef Den denominator;
	typedef Num numerator;
	typedef typename denominator::value_type unit_system;
	static_assert(is_same<typename denominator::value_type, typename numerator::value_type>::value, "UnitQuotient with incompatible unit systems");
};

template<typename NumericType, typename Quotient>
class Quantity
{
	NumericType rawValue;
	
public:
	typedef Quotient Units;
	
	Quantity() = default;
	Quantity(const Quantity&) = default;
	Quantity(Quantity&&) = default;
	Quantity& operator=(const Quantity&) = default;
	Quantity& operator=(Quantity&&) = default;
	
	explicit Quantity(NumericType rawValue)
	: rawValue(rawValue)
	{
	}
	
	NumericType raw() const { return rawValue; }
	
	Quantity& operator+=(Quantity that) const
	{
		return *this = *this + that; 
	}
	
	Quantity& operator-=(Quantity that) const
	{
		return *this = *this - that; 
	}
	
	Quantity operator-(Quantity that) const
	{
		return Quantity(raw() - that.raw());
	}
	
	Quantity operator+(Quantity that) const
	{
		return Quantity(raw() + that.raw());
	}
	
	Quantity& operator*=(NumericType that)
	{
		rawValue *= that;
		return *this;
	}
	
	Quantity& operator/=(NumericType that)
	{
		rawValue /= that;
		return *this;
	}
	
	Quantity operator*(NumericType that) const
	{
		return Quantity(raw() * that);
	}
	
	template<typename NT, typename Q>
	auto operator*(Quantity<NT, Q> that) const
	{
		typedef typename combine<typename Units::denominator, typename Q::denominator>::type CombinedDenominator;
		typedef typename combine<typename Units::numerator, typename Q::numerator>::type CombinedNumerator;
		typedef typename remove_intersection<CombinedNumerator, CombinedDenominator>::type UnsortedNumerator;
		typedef typename remove_intersection<CombinedDenominator, CombinedNumerator>::type UnsortedDenominator;
		typedef typename sorted<UnsortedNumerator>::type Numerator;
		typedef typename sorted<UnsortedDenominator>::type Denominator;
		typedef UnitQuotient<Denominator, Numerator> ResultUnits;
		auto rawQuantity = raw() * that.raw();
		return Quantity<decltype(rawQuantity), ResultUnits>(rawQuantity);
	}
	
	Quantity operator/(NumericType that) const
	{
		return Quantity(raw() / that);
	}
	
	template<typename NT, typename Q>
	auto operator/(Quantity<NT, Q> that) const
	{
		// flip fraction rows around for division
		typedef typename combine<typename Units::denominator, typename Q::numerator>::type CombinedDenominator;
		typedef typename combine<typename Units::numerator, typename Q::denominator>::type CombinedNumerator;
		typedef typename remove_intersection<CombinedNumerator, CombinedDenominator>::type UnsortedNumerator;
		typedef typename remove_intersection<CombinedDenominator, CombinedNumerator>::type UnsortedDenominator;
		typedef typename sorted<UnsortedNumerator>::type Numerator;
		typedef typename sorted<UnsortedDenominator>::type Denominator;
		typedef UnitQuotient<Denominator, Numerator> ResultUnits;
		auto rawQuantity = raw() / that.raw();
		return Quantity<decltype(rawQuantity), ResultUnits>(rawQuantity);
	}
};

template<typename MulType, typename NumericType, typename Quotient>
auto operator*(MulType left, Quantity<NumericType, Quotient> right)
	-> typename enable_if<is_arithmetic<MulType>::value, Quantity<NumericType, Quotient>>::type
{
	typedef typename Quotient::unit_system UnitEnum;
	typedef UnitQuotient<UnitProduct<UnitEnum>, UnitProduct<UnitEnum>> EmptyQuotient;
	typedef Quantity<NumericType, EmptyQuotient> Unitless;
	return Unitless(left) * right;
}

template<typename UnitType, UnitType... U, typename NumericType = double>
using UnitSkeleton = Quantity<NumericType, UnitQuotient<UnitProduct<UnitType, U...>, UnitProduct<UnitType>>>;
