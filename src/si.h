#include "unit.h"
#include "value.h"

// SI base units
using DummyUnit =	UnitSI<Dimensions<0,0,0,0,0,0,0,0,0>>;
using Metre = 		UnitSI<Dimensions<1,0,0,0,0,0,0,0,0>>;  	// m
using Kilogram = 	UnitSI<Dimensions<0,1,0,0,0,0,0,0,0>>;  	// kg
using Second = 		UnitSI<Dimensions<0,0,1,0,0,0,0,0,0>>;  	// s
using Ampere = 		UnitSI<Dimensions<0,0,0,1,0,0,0,0,0>>;  	// A
using Kelvin = 		UnitSI<Dimensions<0,0,0,0,1,0,0,0,0>>;  	// K
using Candela = 	UnitSI<Dimensions<0,0,0,0,0,1,0,0,0>>;  	// cd
using Mole = 		UnitSI<Dimensions<0,0,0,0,0,0,1,0,0>>;  	// mol
using Radian = 		UnitSI<Dimensions<0,0,0,0,0,0,0,1,0>>;  	// rad
using Steradian = 	UnitSI<Dimensions<0,0,0,0,0,0,0,0,1>>; 		// sr

// SI base quantities
using Length =		Value<Metre>;
using Mass =		Value<Kilogram>;
using Time =		Value<Second>;
using Current =		Value<Ampere>;
using Temperature =	Value<Kelvin>;
using Intensity =	Value<Candela>;
using Amount =		Value<Mole>;
using Angle =		Value<Radian>;
using SolidAngle =	Value<Steradian>;

// SI derived units
using Herts =		DummyUnit::div<Second>;						// 1/s
using Newton =		Kilogram::mul<Metre>::div<Second>::div<Second>;

using Speed = 		Value<Metre::div<Second>>;
using Acceleration =Value<Speed::unit::div<Second>>;

using AngularSpeed =Value<Radian::div<Second>>;
using AngularAcceleration = Value<AngularSpeed::unit::div<Second>>;


constexpr
Time operator "" s(long double d)
   // a f-p literal suffixed by 's'
{
   return Time (d); 
}

constexpr
Length operator "" m(long double d)
      // a f-p literal  suffixed by 's2'
{
  return Length (d);
}