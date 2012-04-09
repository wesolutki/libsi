#include <iostream>

using namespace std;

#include "str.h"

template<int N, int L>
struct Dimension 
{
	enum { n=N, l=L };
};

template<const char* Texts, const int* Lens, int... Dims>
struct Unit
{
	typedef typename UnitTextImpl<Texts, Lens, 0, true, StringBuilder<>, Dims...>::str str;
	constexpr static const char * text = str::value;
};

template<int M, int KG, int S>
struct UnitSI 
{
	enum { m=M, kg=KG, s=S };
	
	// dimension texts constrains
	constexpr static char SITexts[] = {	'm',
										'k', 'g',
										's'};

	constexpr static int SITextLengths[] = {	1,
												2,
												1};
	
	constexpr static const char * text = Unit<SITexts, SITextLengths, M, KG, S>::text;
};

template<typename U1, typename U2>
using UnitMul = UnitSI<U1::m + U2::m, U1::kg + U2::kg, U1::s + U2::s>;

template<typename U1, typename U2>
using UnitDiv = UnitSI<U1::m - U2::m, U1::kg + U2::kg, U1::s - U2::s>;

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
    str << p._val << " " << U2::text;
	return str;
}

using Second = UnitSI<0,0,1>;  // s
using Second2 = UnitSI<0,0,2>; // s*s
using Meter = UnitSI<1,0,0>;
using Distance = Value<Meter>;
using Time = Value<Second>;
using SpeedUnit = UnitDiv<Meter, Second>;
using Speed = Value<SpeedUnit>;        // m /s
using Acceleration = Value<UnitSI<1,0,-2>>; // m /s/s

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
