#ifndef __UNIT_H
#define __UNIT_H

#include "str.h"
#include "dim.h"

template<const char* Texts, const int* Lens, typename Dims>
struct Unit
{
	using dims = Dims;

	using str = typename UnitText<Texts, Lens, dims>::str;
	constexpr static const char * text = str::value;
};

template<typename Dims>
struct UnitSI
{
	// dimension texts constrains
	using dims = Dims;
	constexpr static char SITexts[] = {	'm',
										'k', 'g',
										's',
										'A',
										'K',
										'c', 'd',
										'm', 'o', 'l',
										'r', 'a', 'd',
										's', 'r'};

	constexpr static int SITextLengths[] = {	1,
												2,
												1,
												1,
												1,
												2,
												3,
												3,
												2};

	using unit = Unit<SITexts, SITextLengths, Dims>;
	constexpr static const char * text = unit::text;
	
	template <typename U2>
	using mul = UnitSI<typename dims::template map<typename U2::dims, AddOperator>>;
	
	template <typename U2>
	using div = UnitSI<typename dims::template map<typename U2::dims, SubOperator>>;
};

#endif // UNIT_H