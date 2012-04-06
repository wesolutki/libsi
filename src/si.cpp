#include <iostream>

using namespace std;

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

template<char... S>
struct StringBuilder
{
	template<char C>
	struct add_char
	{
		typedef StringBuilder<S..., C> type;
	};
	template<char... S2>
	struct add_chars
	{
		typedef StringBuilder<S..., S2...> type;
	};
	static const char value[sizeof...(S)+1];
};

template<char... S>
const char StringBuilder<S...>::value[sizeof...(S)+1] = {
	S...
};

template<int N, char... P>
struct UnitText
{
	static const char value[sizeof...(P)+1];
};
template<int N, char... P>
const char UnitText<N, P...>::value[sizeof...(P)+1] = {
	P...
};

template<char... P>
struct UnitText<0, P...>
{
	static const char value[1];
};
template<char... P>
const char UnitText<0, P...>::value[1] = {' '};



template<int M, int K, int S>
struct Unit { // a unit in the MKS system
    enum { m=M, kg=K, s=S };
	typedef Unit<M, K, S> unit;

	template<typename U>
	struct mul
	{
		typedef Unit<m + U::m, kg + U::kg, s + U::s> unit;
	};
	/*char text[10];
	char * toString()
	{
		return text;
	}*/
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
	constexpr Value<typename Unit::mul<typename ValueIn::unit>::unit> operator* (const ValueIn & v)
	{
		return Value<typename Unit::mul<typename ValueIn::unit>::unit> (_val * v._val);
	}
	
	template<typename U>
	friend ostream & operator << (ostream & str, const Value<U> & p);
};

template<typename Unit>
ostream &  operator << (ostream & str, const Value<Unit> & p)
{
    str << p._val << " " << StringBuilder<'S', 'U', 'T'>::add_chars<'E', 'K'>::type::value;
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

int main()
{
	Speed v1 = 10.m/2.5s;
	Speed v2 = 5.m/2.5s;
	Time dist = (v1+v2) * 10.s / v2;
	cout << v1 << endl;
	cout << v2 << endl;
	cout << dist << endl;
	Speed sum_v = v1 + v2;
	cout << (v1 + v2) << endl;
	cout << Value<Unit<5, 2, -7>>(5000) << endl;
	
	Meter meter;
	Second second;
	Speed u = Value<SpeedUnit>(5);
	return 0;
}
