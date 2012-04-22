#ifndef __VALUE_H
#define __VALUE_H

#include <iostream>
using namespace std;

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
	
	constexpr Value<U> operator- (const Value<U> & v)
	{
		return Value<U> (_val - v._val);
	}
	
	template <typename ValueIn>
	constexpr Value<typename U::template div<typename ValueIn::unit>> operator/ (const ValueIn & v)
	{
		return Value<typename U::template div<typename ValueIn::unit>> (_val / v._val);
	}
	
	template <typename ValueIn>
	constexpr Value<typename U::template mul<typename ValueIn::unit>> operator* (const ValueIn & v)
	{
		return Value<typename U::template mul<typename ValueIn::unit>> (_val * v._val);
	}
	
	template<typename U2>
	friend ostream & operator << (ostream & str, const Value<U2> & p);
};

template<typename U2>
ostream &  operator << (ostream & str, const Value<U2> & p)
{
    str << p._val << " " << U2::text << endl;
	return str;
}

#endif // VALUE_H