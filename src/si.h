#include <iostream>

using namespace std;

#include "str.h"

template<char V>
struct UnitShow
{
	enum {text='0'+V};
};

template<>
struct UnitShow<0>
{
	enum {text=' '};
};
/*
template<char sign, int V>
struct UnitSign
{
	const string text = string(sign);
	//enum {text=sign};
};

template<char sign>
struct UnitSign<sign, 0>
{
	const string text = string("");
	//enum {text=' '};
};
*/


/*template<int N, char... S>
struct UnitPart
{
	enum { n=N };
	static const char value[sizeof...(S)+1];
};

template<int N, char... S>
const char UnitPart<N, S...>::value[sizeof...(S)+1] = {
	S...
};

template<int N = 0, char... S>
const char UnitPart<0, S...>::value[sizeof...(S)+1] = {
	S...
};*/

template <typename Head, typename... Tail>
struct UnitImpl
{
	typedef typename UnitImpl<
			typename B::template add_char<next>::type,
			next, Tail...
		>::type type;
};

template<int M, int K, int S>
struct Unit { // a unit in the MKS system
    enum { m=M, kg=K, s=S };
	typedef Unit<M, K, S> unit;
	
	typedef StringBuilder<'u', 'n', 'i', 't', ' '> init_text;
	typedef typename init_text::template cond_add_char<m!=0, 'm'>::str text1;
	typedef typename text1::template cond_add_chars<kg!=0, 'k', 'g'>::str text2;
	typedef typename text2::template cond_add_char<s!=0, 's'>::str text3;
	
	constexpr static const char * text = //text3::value;
		typename StringBuilder<'u', 'n', 'i', 't', ' '>::template
		typename cond_add_char<m!=0, 'm'>::template str::
		cond_add_char<kg!=0, 'k'>::str::value;
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