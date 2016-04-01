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
#include <ratio>
#include <type_traits>
#include <utility>

namespace detail
{
#pragma mark - Sequence type
	// (integer_sequence only works with integers, not enums)

	template<typename T, T... S>
	struct sequence
	{
		using value_type = T;
		static constexpr size_t size = sizeof...(S);
	};

#pragma mark - Test if a sequence contains a value
	template<typename IntegerType, IntegerType I, IntegerType... S>
	struct sequence_contains_type;

	template<typename IntegerType, IntegerType I, IntegerType S1,
		IntegerType... Ss>
	struct sequence_contains_type<IntegerType, I, S1, Ss...>
	: std::conditional_t<I == S1,
		std::true_type,
		typename sequence_contains_type<IntegerType, I, Ss...>::type>
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
	struct combine_;

	template<typename IntegerType, IntegerType... S1, IntegerType... S2>
	struct combine_<sequence<IntegerType, S1...>, sequence<IntegerType, S2...>>
	{
		using type = sequence<IntegerType, S1..., S2...>;
	};

	template<typename Seq1, typename Seq2>
	using combine = typename combine_<Seq1, Seq2>::type;

	template<typename Seq1, typename Seq2>
	constexpr auto combine_sequences(Seq1, Seq2)
	{
		return combine<Seq1, Seq2>{};
	}

#pragma mark - Remove an item
	template<typename Seq, typename IntegerType, IntegerType I>
	struct left_of_;

	template<typename IntegerType, IntegerType I>
	struct left_of_<sequence<IntegerType>, IntegerType, I>
	{
		using type = sequence<IntegerType>;
	};

	template<typename IntegerType, IntegerType I, IntegerType S1,
		IntegerType... Ss>
	struct left_of_<sequence<IntegerType, S1, Ss...>, IntegerType, I>
	{
		using type = std::conditional_t<I == S1,
			sequence<IntegerType>,
			combine<
				sequence<IntegerType, S1>,
				typename left_of_<
					sequence<IntegerType, Ss...>,
					IntegerType, I>::type>>;
	};

	template<typename Seq, typename IntegerType, IntegerType I>
	using left_of = typename left_of_<Seq, IntegerType, I>::type;

	template<typename Seq, typename IntegerType, IntegerType I>
	struct right_of_;

	template<typename IntegerType, IntegerType I>
	struct right_of_<sequence<IntegerType>, IntegerType, I>
	{
		using type = sequence<IntegerType>;
	};

	template<typename IntegerType, IntegerType I, IntegerType S1,
		IntegerType... Ss>
	struct right_of_<sequence<IntegerType, S1, Ss...>, IntegerType, I>
	{
		using type = std::conditional_t<I == S1,
			sequence<IntegerType, Ss...>,
			typename right_of_<sequence<IntegerType, Ss...>, IntegerType, I>::type
		>;
	};

	template<typename Seq, typename IntegerType, IntegerType I>
	using right_of = typename right_of_<Seq, IntegerType, I>::type;

	template<typename Seq, typename IntegerType, IntegerType I>
	struct removed_from_
	{
		using type = combine<left_of<Seq, IntegerType, I>,
		                    right_of<Seq, IntegerType, I>>;
	};
	template<typename Seq, typename IntegerType, IntegerType I>
	using removed_from = typename removed_from_<Seq, IntegerType, I>::type;

#pragma mark - Remove identical terms
	template<typename Seq1, typename Seq2>
	struct remove_intersection_;

	template<typename IntegerType1, IntegerType1... Ones, typename IntegerType2>
	struct remove_intersection_<
		sequence<IntegerType1, Ones...>,
		sequence<IntegerType2>>
	{
		using type = sequence<IntegerType1, Ones...>;
	};

	template<typename IntegerType1, IntegerType1... Ones, typename IntegerType2,
		IntegerType2 Two, IntegerType2... Twos>
	struct remove_intersection_<
		sequence<IntegerType1, Ones...>,
		sequence<IntegerType2, Two, Twos...>>
	{
		using type = removed_from<
			typename remove_intersection_<
				sequence<IntegerType1, Ones...>,
				sequence<IntegerType2, Twos...>
			>::type,
			IntegerType2, Two
		>;
	};

	template<typename Seq1, typename Seq2>
	using remove_intersection = typename remove_intersection_<Seq1, Seq2>::type;

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
	struct sorted_;

	template<typename IntegerType>
	struct sorted_<sequence<IntegerType>>
	{
		using type = sequence<IntegerType>;
	};

	template<typename IntegerType, IntegerType S1>
	struct sorted_<sequence<IntegerType, S1>>
	{
		using type = sequence<IntegerType, S1>;
	};

	template<typename IntegerType, IntegerType S1, IntegerType... Ss>
	struct sorted_<sequence<IntegerType, S1, Ss...>>
	{
	private:
		using Seq = sequence<IntegerType, S1, Ss...>;
		static constexpr IntegerType Max = sequence_max<Seq>::value;

	public:
		using type = combine<
			typename sorted_<removed_from<Seq, IntegerType, Max>>::type,
			sequence<IntegerType, Max>>;
	};

	template<typename Sequence>
	using sorted = typename sorted_<Sequence>::type;
}

namespace unitscxx
{
	template<typename NumericType, typename Numerator, typename Denominator>
	class quantity
	{
		NumericType rawValue;

	public:
		using var = quantity;
		using numerator = Numerator;
		using denominator = Denominator;
		using unit_system = typename numerator::value_type;

		static_assert(std::is_same<unit_system,
			typename denominator::value_type>::value,
			"quantity with incompatible unit systems");

		template<typename NT, typename N, typename D>
		friend class quantity;

		constexpr quantity() : rawValue{} {};
		constexpr quantity(const quantity&) = default;
		constexpr quantity(quantity&&) = default;
		quantity& operator=(const quantity&) = default;
		quantity& operator=(quantity&&) = default;

		explicit constexpr quantity(NumericType val)
			: rawValue(val)
		{
		}

		quantity& operator+=(quantity that)
		{
			return *this = *this + that;
		}

		quantity& operator-=(quantity that)
		{
			return *this = *this - that;
		}

		constexpr quantity operator+(quantity that) const
		{
			return quantity(rawValue + that.rawValue);
		}

		constexpr quantity operator-(quantity that) const
		{
			return quantity(rawValue - that.rawValue);
		}

		constexpr quantity operator+() const
		{
			return quantity(+rawValue);
		}

		constexpr quantity operator-() const
		{
			return quantity(-rawValue);
		}

		template<typename N = Numerator, typename D = Denominator, typename
			= typename std::enable_if<N::size == 0 && D::size == 0>::type>
		constexpr auto operator+(NumericType that) const
		{
			return quantity<NumericType, N, D>(rawValue + that);
		}

		template<typename N = Numerator, typename D = Denominator, typename
			= typename std::enable_if<N::size == 0 && D::size == 0>::type>
		constexpr auto operator-(NumericType that) const
		{
			return quantity<NumericType, N, D>(rawValue - that);
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
			return quantity(rawValue * that);
		}

		template<typename NT, typename N, typename D>
		constexpr auto operator*(quantity<NT, N, D> that) const
		{
			using combined_numerator   = detail::combine<numerator, N>;
			using combined_denominator = detail::combine<denominator, D>;

			using unsorted_numerator   = detail::remove_intersection<
				combined_numerator,
				combined_denominator>;

			using unsorted_denominator = detail::remove_intersection<
				combined_denominator,
				combined_numerator>;

			using result_numerator   = detail::sorted<unsorted_numerator>;
			using result_denominator = detail::sorted<unsorted_denominator>;

			using result_quantity = quantity<
				decltype(rawValue * that.rawValue),
				result_numerator,
				result_denominator>;

			return result_quantity(rawValue * that.rawValue);
		}

		constexpr quantity operator/(NumericType that) const
		{
			return quantity(rawValue / that);
		}

		template<typename NT, typename N, typename D>
		constexpr auto operator/(quantity<NT, N, D> that) const
		{
			// flip fraction rows around for division
			using combined_numerator   = detail::combine<numerator, D>;
			using combined_denominator = detail::combine<denominator, N>;

			using unsorted_numerator   = detail::remove_intersection<
				combined_numerator,
				combined_denominator>;

			using unsorted_denominator = detail::remove_intersection<
				combined_denominator,
				combined_numerator>;

			using result_numerator   = detail::sorted<unsorted_numerator>;
			using result_denominator = detail::sorted<unsorted_denominator>;

			using result_quantity = quantity<
				decltype(rawValue / that.rawValue),
				result_numerator,
				result_denominator>;

			return result_quantity(rawValue / that.rawValue);
		}

		template<intmax_t N, intmax_t D>
		constexpr quantity operator*(std::ratio<N, D>) const
		{
			return (*this) * N / D;
		}

		template<intmax_t N, intmax_t D>
		constexpr quantity operator/(std::ratio<N, D>) const
		{
			return (*this) / N * D;
		}

		constexpr bool operator==(quantity that) const
		{
			return rawValue == that.rawValue;
		}

		constexpr bool operator!=(quantity that) const
		{
			return rawValue != that.rawValue;
		}

		constexpr bool operator<(quantity that) const
		{
			return rawValue < that.rawValue;
		}

		constexpr bool operator>(quantity that) const
		{
			return rawValue > that.rawValue;
		}

		constexpr bool operator<=(quantity that) const
		{
			return rawValue <= that.rawValue;
		}

		constexpr bool operator>=(quantity that) const
		{
			return rawValue >= that.rawValue;
		}

		template<typename N = Numerator, typename D = Denominator, typename
			= std::enable_if_t<N::size == 0 && D::size == 0>>
		constexpr operator NumericType()
		{
			return rawValue;
		}
	};

	template<typename MulType, typename NT, typename N, typename D, typename =
		typename std::enable_if<std::is_arithmetic<MulType>::value>::type>
	constexpr quantity<NT, N, D> operator*(MulType left, quantity<NT, N, D> right)
	{
		using unit_system = typename N::value_type;
		using unitless_quantity = quantity<NT,
			detail::sequence<unit_system>,
			detail::sequence<unit_system>>;
		return unitless_quantity(left) * right;
	}

	template<typename MulType, typename NT, typename N, typename D, typename =
		typename std::enable_if<std::is_arithmetic<MulType>::value>::type>
	constexpr quantity<NT, D, N> operator/(MulType left, quantity<NT, N, D> right)
	{
		using unit_system = typename N::value_type;
		using unitless_quantity = quantity<NT,
			detail::sequence<unit_system>,
			detail::sequence<unit_system>>;
		return unitless_quantity(left) / right;
	}

	template<intmax_t RN, intmax_t RD, typename QNT, typename QN, typename QD>
	constexpr quantity<QNT, QN, QD> operator*(
		std::ratio<RN, RD> left, quantity<QNT, QN, QD> right)
	{
		return right * left;
	}

	template<intmax_t RN, intmax_t RD, typename QNT, typename QN, typename QD>
	constexpr quantity<QNT, QD, QN> operator/(
		std::ratio<RN, RD> left, quantity<QNT, QN, QD> right)
	{
		return (1 / right) * left;
	}

	template<typename NumericType, typename UnitType, UnitType... U>
	using unit_base = quantity<NumericType,
		detail::sequence<UnitType, U...>,
		detail::sequence<UnitType>>;
}

#endif
