#ifndef __LIST_H
#define __LIST_H

#include "math.h"

// recursion list implementation
// multiple ints required
template<int...>
struct ListElementImpl;

// head, tail list recursion through list
template<int Head, int... Tail>
struct ListElementImpl<Head, Tail...>
{
	enum {val=Head};					// dimension expotential
	typedef ListElementImpl<Tail...> next;	// next list dimension
};

// last list recursion
template<int Head>
struct ListElementImpl<Head>
{
	enum {val=Head};
	typedef int next;					// dummy typedef
};

// empty list
template<>
struct ListElementImpl<>
{
};

// list indexing
template<int Idx, typename El>
struct ListIndex
{
	enum { val = ListIndex<Idx-1, typename El::next>::val };
};

template<typename El>
struct ListIndex<0, El>
{
	enum { val=El::val };
};

// init struct
template<int... Els>
struct List
{
	typedef List<Els...> self;	// self
	
	enum {size=sizeof...(Els)};			// list size
	typedef ListElementImpl<Els...> els;		// list elements impl
	
	// pushing-back element
	template<int El>
	using push_back = List<Els..., El>;
	
	// 0-element indexing
	template<int Idx>
	using get = ListIndex<Idx, els>;
	
	// map impl operations
	template<int N, typename Ret, typename L2, typename Op>
	struct map_impl
	{
		typedef get<Ret::size> el1;								// get value from 1st list
		typedef typename L2::template get<Ret::size> el2;		// get value from 2nd list
		typedef typename Op::template op<el1::val, el2::val> op_ret;	// perform operation
		typedef typename Ret::template push_back<op_ret::val> after;	// push-back retrived value
		typedef typename map_impl<N-1, after, L2, Op>::list list;		// return to recursion
	};
	
	// map impl operations
	template<typename Ret, typename L2, typename Op>
	struct map_impl<0, Ret, L2, Op>
	{
		typedef Ret list;
	};
	
	// map operations
	template<typename L, typename Op>
	using map = typename map_impl<size, List<>, L, Op>::list;
};

#endif // __LIST_H