#include <iostream>

using namespace std;

#include "str.h"

template<int N, int L>
struct Dimension
{
	enum { n=N, l=L };
};

template<int Idx, typename D>
struct DimensionsIndex
{
	enum { val = DimensionsIndex<Idx-1, typename D::next>::val };
};

template<typename D>
struct DimensionsIndex<0, D>
{
	enum { val=D::val };
};

template<int...>
struct DimensionsImpl;

template<int Head, int... Tail>
struct DimensionsImpl<Head, Tail...>
{
	enum {val=Head};
	typedef DimensionsImpl<Tail...> next;
};

template<int Head>
struct DimensionsImpl<Head>
{
	enum {val=Head};
	typedef int next;
};

template<>
struct DimensionsImpl<>
{
};

template<int... Dims>
struct Dimensions
{
	template<int D>
	struct add_dim
	{
		typedef Dimensions<Dims..., D> type;
	};
	enum {size=sizeof...(Dims)};
	typedef DimensionsImpl<Dims...> dims;
};

template<int S, typename D, typename D1, typename D2>
struct DimensionsSumImpl
{
	typedef DimensionsIndex<D::size, typename D1::dims> val1;
	typedef DimensionsIndex<D::size, typename D2::dims> val2;
	typedef typename D::template add_dim<(val1::val + val2::val)> sum;
	typedef typename DimensionsSumImpl<S-1, typename sum::type, D1, D2>::dims dims;
};

template<typename D, typename D1, typename D2>
struct DimensionsSumImpl<0, D, D1, D2>
{
	typedef D dims;
};

template<typename D1, typename D2>
struct DimensionsSum
{
	typedef typename DimensionsSumImpl<D1::size, Dimensions<>, D1, D2>::dims dims;
};

template<int S, typename D, typename D1, typename D2>
struct DimensionsSubImpl
{
	typedef DimensionsIndex<D::size, typename D1::dims> val1;
	typedef DimensionsIndex<D::size, typename D2::dims> val2;
	typedef typename D::template add_dim<(val1::val - val2::val)> sum;
	typedef typename DimensionsSubImpl<S-1, typename sum::type, D1, D2>::dims dims;
};

template<typename D, typename D1, typename D2>
struct DimensionsSubImpl<0, D, D1, D2>
{
	typedef D dims;
};

template<typename D1, typename D2>
struct DimensionsSub
{
	typedef typename DimensionsSubImpl<D1::size, Dimensions<>, D1, D2>::dims dims;
};
template<const char* Texts, const int* Lens, typename Dims>
struct Unit
{
	typedef Dims dims;

	typedef typename UnitText<Texts, Lens, dims>::str str;
	constexpr static const char * text = str::value;
};

template<typename Dims>
struct UnitSI 
{
//	enum { m=M, kg=KG, s=S };
	
	// dimension texts constrains
	typedef Dims dims;
	constexpr static char SITexts[] = {	'm',
										'k', 'g',
										's'};

	constexpr static int SITextLengths[] = {	1,
												2,
												1};

	typedef Unit<SITexts, SITextLengths, Dims> unit;
	constexpr static const char * text = unit::text;

};

template<typename U1, typename U2>
using UnitMul = UnitSI<typename DimensionsSum<typename U1::dims, typename U2::dims>::dims>;
template<typename U1, typename U2>
using UnitDiv = UnitSI<typename DimensionsSub<typename U1::dims, typename U2::dims>::dims>;
//template<typename U1, typename U2>
//using UnitMul = Unit<Texts, Lens, DimensionsSum<dims::size, Dimensions<>, dims, typename U::dims>>;
//using UnitMul = UnitSI<U1::m + U2::m, U1::kg + U2::kg, U1::s + U2::s>;

//template<typename U1, typename U2>
//using UnitDiv = UnitSI<U1::m - U2::m, U1::kg + U2::kg, U1::s - U2::s>;

/*template<typename U1, typename U2>
constexpr UnitDiv<U1, U2> operator/ (const U1 & u1, const U2 & u2)
{
	return UnitDiv<U1, U2>();
}*/

//using SpeedUnit2 = Unit<1,0,0>() / Unit<0,0,1>();

template<typename U> // magnitude with unit
struct Value {
	typedef U unit;
    double _val;    // the magnitude
    constexpr explicit Value(double d)
         : _val(d) {} // construct a Value from a double
	constexpr Value<U> operator+ (const Value<U> & v)
	{
		return Value<U> (_val + v._val);
	}
	
	template <typename ValueIn>
	constexpr Value<UnitDiv<U, typename ValueIn::unit>> operator/ (const ValueIn & v)
	{
		return Value<UnitDiv<U, typename ValueIn::unit>> (_val / v._val);
	}
	
	template <typename ValueIn>
	constexpr Value<UnitMul<U, typename ValueIn::unit>> operator* (const ValueIn & v)
	{
		return Value<UnitMul<U, typename ValueIn::unit>> (_val * v._val);
	}
	
	template<typename U2>
	friend ostream & operator << (ostream & str, const Value<U2> & p);
};

template<typename U2>
ostream &  operator << (ostream & str, const Value<U2> & p)
{
    str << p._val << " " << U2::text << endl;
	str << "\t" << DimensionsIndex<3, typename Dimensions<3, 5, 7, 6, 4>::dims>::val;
	return str;
}

using Second = UnitSI<Dimensions<0,0,1>>;  // s
using Second2 = UnitSI<Dimensions<0,0,2>>; // s*s
using Meter = UnitSI<Dimensions<1,0,0>>;
using Distance = Value<Meter>;
using Time = Value<Second>;
using SpeedUnit = UnitDiv<Meter, Second>;
using Speed = Value<SpeedUnit>;        // m /s
using Acceleration = Value<UnitSI<Dimensions<1,0,-2>>>; // m /s/s

//using SpeedUnit2 = Meter / Second;

constexpr
Value<Second> operator "" s(long double d)
   // a f-p literal suffixed by 's'
{
   return Value<Second> (d); 
}
constexpr
Value<Second2> operator "" s2(long double d)
      // a f-p literal  suffixed by 's2'
{
  return Value<Second2> (d);
}

constexpr
Value<Meter> operator "" m(long double d)
      // a f-p literal  suffixed by 's2'
{
  return Value<Meter> (d);
}
