#include <iostream>

using namespace std;

#include "str.h"

template<int N, int L>
struct Dimension 
{
	enum { n=N, l=L };
};

template<int M, int KG, int S>
struct Unit { // a unit in the MKS system
    //enum { m=M, kg=KG, s=S };
	/*constexpr static string text()
	{
		string str("purek   ");
		str.push_back(' ');
		str.push_back(M+48);
		str.push_back(KG+48);
		str.push_back(S+48);
		return str;
	}*/
	typedef Dimension<M, 1> dim_m;
	typedef Dimension<KG, 2> dim_kg;
	typedef Dimension<S, 1> dim_s;
	constexpr static int dims[3] = {M, KG, S};
	enum { m = dim_m::n };
	enum { kg = dim_kg::n };
	enum { s = dim_s::n };
	typedef typename UnitTextImpl<0, true, StringBuilder<>, M, KG, S>::str str;

	/*typedef StringBuilder<'u', 'n', 'i', 't', ' '> init_text;
	typedef typename init_text::template cond_add_char<(m<0), '/'>::str text_m1;
	typedef typename text_m1::template cond_add_char<(m>0), '*'>::str text_m2;
	typedef typename text_m2::template cond_add_char<m!=0, 'm'>::str text_m3;
	typedef typename text_m3::template cond_add_char<(kg<0), '/'>::str text_k1;
	typedef typename text_k1::template cond_add_char<(kg>0), '*'>::str text_k2;
	typedef typename text_k2::template cond_add_chars<kg!=0, 'k', 'g'>::str text_k3;
	typedef typename text_k3::template cond_add_char<(s<0), '/'>::str text_s1;
	typedef typename text_s1::template cond_add_char<(s>0), '*'>::str text_s2;
	typedef typename text_s2::template cond_add_char<s!=0, 's'>::str unit;
*/
	constexpr static const char * text = str::value;
	/*	typename StringBuilder<'u', 'n', 'i', 't', ' '>::template
		typename cond_add_char<m!=0, 'm'>::template str::
		cond_add_char<kg!=0, 'k'>::str::value;*/
		//cond_add_char<s!=0, 's'>::str::value;
};

template<typename U1, typename U2>
using UnitMul = Unit<U1::m + U2::m, U1::kg + U2::kg, U1::s + U2::s>;

template<typename U1, typename U2>
using UnitDiv = Unit<U1::m - U2::m, U1::kg + U2::kg, U1::s - U2::s>;

/*template<typename U1, typename U2>
constexpr UnitDiv<U1, U2> operator/ (const U1 & u1, const U2 & u2)
{
	return UnitDiv<U1, U2>();
}*/

//using SpeedUnit2 = Unit<1,0,0>() / Unit<0,0,1>();

template<typename Unit> // magnitude with unit
struct Value {
	typedef Unit unit;
    double _val;    // the magnitude
    constexpr explicit Value(double d)
         : _val(d) {} // construct a Value from a double
	constexpr Value<Unit> operator+ (const Value<Unit> & v)
	{
		return Value<Unit> (_val + v._val);
	}
	
	template <typename ValueIn>
	constexpr Value<UnitDiv<Unit, typename ValueIn::unit>> operator/ (const ValueIn & v)
	{
		return Value<UnitDiv<unit, typename ValueIn::unit>> (_val / v._val);
	}
	
	template <typename ValueIn>
	constexpr Value<UnitMul<Unit, typename ValueIn::unit>> operator* (const ValueIn & v)
	{
		return Value<UnitMul<Unit, typename ValueIn::unit>> (_val * v._val);
	}
	
	template<typename U>
	friend ostream & operator << (ostream & str, const Value<U> & p);
};

template<typename U>
ostream &  operator << (ostream & str, const Value<U> & p)
{
    str << p._val << " " << U::text;
	return str;
}

using Second = Unit<0,0,1>;  // s
using Second2 = Unit<0,0,2>; // s*s
using Meter = Unit<1,0,0>;
using Distance = Value<Meter>;
using Time = Value<Second>;
using SpeedUnit = UnitDiv<Meter, Second>;
using Speed = Value<SpeedUnit>;        // m /s
using Acceleration = Value<Unit<1,0,-2>>; // m /s/s

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
