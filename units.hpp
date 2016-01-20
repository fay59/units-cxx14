//
// units.hpp
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

#ifndef UNITS_HPP
#define UNITS_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

namespace detail
{
#pragma mark - Sequence type
	// (integer_sequence only works with integers, not enums)

	template<typename T, T... S>
	struct sequence
	{
		typedef T value_type;
		static constexpr size_t size = sizeof...(S);
	};

#pragma mark - Test if a sequence contains a value
	template<typename IntegerType, IntegerType I, IntegerType... S>
	struct sequence_contains_type;

	template<typename IntegerType, IntegerType I, IntegerType S1,
		IntegerType... Ss>
	struct sequence_contains_type<IntegerType, I, S1, Ss...>
		: std::conditional<I == S1,
			std::true_type,
			typename sequence_contains_type<IntegerType, I, Ss...>::type>::type
	{
	};

	template<typename IntegerType, IntegerType I>
	struct sequence_contains_type<IntegerType, I> : std::false_type
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

	template<typename IntegerType, IntegerType I, IntegerType S1,
		IntegerType... Ss>
	struct left_of<sequence<IntegerType, S1, Ss...>, IntegerType, I>
	{
		typedef typename std::conditional<I == S1,
			sequence<IntegerType>,
			typename combine<
				sequence<IntegerType, S1>,
				typename left_of<
					sequence<IntegerType, Ss...>,
					IntegerType, I>::type>::type>::type type;
	};

	template<typename Seq, typename IntegerType, IntegerType I>
	struct right_of;

	template<typename IntegerType, IntegerType I>
	struct right_of<sequence<IntegerType>, IntegerType, I>
	{
		typedef sequence<IntegerType> type;
	};

	template<typename IntegerType, IntegerType I, IntegerType S1,
		IntegerType... Ss>
	struct right_of<sequence<IntegerType, S1, Ss...>, IntegerType, I>
	{
		typedef typename std::conditional<I == S1,
			sequence<IntegerType, Ss...>,
			typename right_of<sequence<IntegerType, Ss...>,
		IntegerType, I>::type>::type type;
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
	struct remove_intersection<
		sequence<IntegerType1, Ones...>,
		sequence<IntegerType2>>
	{
		typedef sequence<IntegerType1, Ones...> type;
	};

	template<typename IntegerType1, IntegerType1... Ones, typename IntegerType2,
		IntegerType2 Two, IntegerType2... Twos>
	struct remove_intersection<
		sequence<IntegerType1, Ones...>,
		sequence<IntegerType2, Two, Twos...>>
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

	template<typename IntegerType, IntegerType S1, IntegerType S2,
		IntegerType... Ss>
	struct sequence_max<sequence<IntegerType, S1, S2, Ss...>>
	{
		static constexpr IntegerType value = sequence_max<
			sequence<IntegerType, (S1 > S2 ? S1 : S2), Ss...>>::value;
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
			typename sorted<
				typename removed_from<Seq, IntegerType, Max>::type>::type,
			sequence<IntegerType, Max>>::type type;
	};
}

namespace unitscxx
{
	template<typename NumericType, typename Numerator, typename Denominator>
	class quantity
	{
		NumericType rawValue;

	public:
		typedef Numerator numerator;
		typedef Denominator denominator;
		typedef typename numerator::value_type unit_system;

		static_assert(std::is_same<
			typename denominator::value_type,
			typename numerator::value_type>::value,
			"quantity with incompatible unit systems");

		constexpr quantity() = default;
		constexpr quantity(const quantity&) = default;
		constexpr quantity(quantity&&) = default;
		quantity& operator=(const quantity&) = default;
		quantity& operator=(quantity&&) = default;

		explicit constexpr quantity(NumericType rawValue)
		: rawValue(rawValue)
		{
		}

		constexpr NumericType raw() const { return rawValue; }

		quantity& operator+=(quantity that)
		{
			return *this = *this + that;
		}

		quantity& operator-=(quantity that)
		{
			return *this = *this - that;
		}

		constexpr quantity operator-(quantity that) const
		{
			return quantity(raw() - that.raw());
		}

		constexpr quantity operator+(quantity that) const
		{
			return quantity(raw() + that.raw());
		}

		quantity& operator*=(NumericType that)
		{
			rawValue *= that;
			return *this;
		}

		quantity& operator/=(NumericType that)
		{
			rawValue /= that;
			return *this;
		}

		constexpr quantity operator*(NumericType that) const
		{
			return quantity(raw() * that);
		}

		template<typename NT, typename N, typename D>
		constexpr auto operator*(quantity<NT, N, D> that) const
		{
			typedef typename detail::combine<numerator, N>::type
				combined_numerator;

			typedef typename detail::combine<denominator, D>::type
				combined_denominator;

			typedef typename detail::remove_intersection<
				combined_numerator,
				combined_denominator>::type unsorted_numerator;

			typedef typename detail::remove_intersection<
				combined_denominator,
				combined_numerator>::type unsorted_denominator;

			typedef typename detail::sorted<unsorted_numerator>::type
				result_numerator;
			typedef typename detail::sorted<unsorted_denominator>::type
				result_denominator;

			auto rawQuantity = raw() * that.raw();

			typedef quantity<
				decltype(rawQuantity),
				result_numerator,
				result_denominator> result_quantity;

			return result_quantity(rawQuantity);
		}

		constexpr quantity operator/(NumericType that) const
		{
			return quantity(raw() / that);
		}

		template<typename NT, typename N, typename D>
		constexpr auto operator/(quantity<NT, N, D> that) const
		{
			// flip fraction rows around for division
			typedef typename detail::combine<numerator, D>::type
				combined_numerator;

			typedef typename detail::combine<denominator, N>::type
				combined_denominator;

			typedef typename detail::remove_intersection<
				combined_numerator,
				combined_denominator>::type unsorted_numerator;

			typedef typename detail::remove_intersection<
				combined_denominator,
				combined_numerator>::type unsorted_denominator;

			typedef typename detail::sorted<unsorted_numerator>::type
				result_numerator;
			typedef typename detail::sorted<unsorted_denominator>::type
				result_denominator;

			auto rawQuantity = raw() / that.raw();

			typedef quantity<
				decltype(rawQuantity),
				result_numerator,
				result_denominator> result_quantity;

			return result_quantity(rawQuantity);
		}
	};

	template<typename MulType, typename NT, typename N, typename D>
	constexpr auto operator*(MulType left, quantity<NT, N, D> right)
		-> typename std::enable_if<
			std::is_arithmetic<MulType>::value,
			quantity<NT, N, D>>::type
	{
		typedef typename N::value_type unit_system;
		typedef quantity<NT,
			detail::sequence<unit_system>,
			detail::sequence<unit_system>> unitless_quantity;
		return unitless_quantity(left) * right;
	}

	template<typename MulType, typename NT, typename N, typename D>
	constexpr auto operator/(MulType left, quantity<NT, N, D> right)
		-> typename std::enable_if<
			std::is_arithmetic<MulType>::value,
			quantity<NT, D, N>>::type
	{
		typedef typename N::value_type unit_system;
		typedef quantity<NT,
			detail::sequence<unit_system>,
			detail::sequence<unit_system>> unitless_quantity;
		return unitless_quantity(left) / right;
	}

	template<typename NumericType, typename UnitType, UnitType... U>
	using unit_base = quantity<NumericType,
		detail::sequence<UnitType, U...>,
		detail::sequence<UnitType>>;
}

#endif
