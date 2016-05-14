//
// tests.cpp
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

using namespace std;
using namespace detail;

template<typename Seq, size_t N>
struct get_at;

template<size_t N, typename IntegerType, IntegerType I, IntegerType... S>
struct get_at<sequence<IntegerType, I, S...>, N>
	: integral_constant<IntegerType, conditional<
		N == 0,
		integral_constant<IntegerType, I>,
		get_at<sequence<IntegerType, S...>, N-1>>::type::value>
{
};

template<size_t N, typename Seq>
constexpr typename Seq::value_type seq_get(Seq)
{
	return get_at<Seq, N>::value;
}

void static_sequence_tests()
{
	sequence<size_t, 1, 2, 3> seq1;
	sequence<size_t, 4, 5, 6> seq2;
	auto seq3 = combine_sequences(seq1, seq2);
	
	auto leftOf5 = left_of<decltype(seq3), size_t, 5>{};
	static_assert(sequence_contains<size_t, 4>(leftOf5),
		"left_of/sequence_contains");
	static_assert(!sequence_contains<size_t, 5>(leftOf5),
		"left_of/sequence_contains");
	static_assert(!sequence_contains<size_t, 6>(leftOf5),
		"left_of/sequence_contains");
	
	auto rightOf2 = right_of<decltype(seq3), size_t, 2>{};
	static_assert(!sequence_contains<size_t, 1>(rightOf2),
		"right_of/sequence_contains");
	static_assert(!sequence_contains<size_t, 2>(rightOf2),
		"right_of/sequence_contains");
	static_assert(sequence_contains<size_t, 3>(rightOf2),
		"right_of/sequence_contains");
	
	auto no3 = removed_from<decltype(seq3), size_t, 3>{};
	static_assert(sequence_contains<size_t, 2>(no3),
		"removed_from/sequence_contains");
	static_assert(!sequence_contains<size_t, 3>(no3),
		"removed_from/sequence_contains");
	static_assert(sequence_contains<size_t, 4>(no3),
		"removed_from/sequence_contains");
	
	static_assert(sequence_max<decltype(seq3)>::value == 6, "sequence_max");
	
	auto seq4 = combine_sequences(seq2, seq1);
	auto seq4Sorted = sorted<decltype(seq4)>{};
	static_assert(seq_get<0>(seq4Sorted) == 1, "sorted/seq_get");
	static_assert(seq_get<1>(seq4Sorted) == 2, "sorted/seq_get");
	static_assert(seq_get<2>(seq4Sorted) == 3, "sorted/seq_get");
	static_assert(seq_get<3>(seq4Sorted) == 4, "sorted/seq_get");
	static_assert(seq_get<4>(seq4Sorted) == 5, "sorted/seq_get");
	static_assert(seq_get<5>(seq4Sorted) == 6, "sorted/seq_get");
	
	auto seq5 = remove_intersection<decltype(seq4Sorted),
		sequence<size_t, 2, 5>>{};
	static_assert(seq_get<0>(seq5) == 1, "sorted/seq_get");
	static_assert(seq_get<1>(seq5) == 3, "sorted/seq_get");
	static_assert(seq_get<2>(seq5) == 4, "sorted/seq_get");
	static_assert(seq_get<3>(seq5) == 6, "sorted/seq_get");
}
