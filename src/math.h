#ifndef __MATH_H
#define __MATH_H

struct AddOperator
{
	template <int V1, int V2>
	struct op
	{
		enum {	val = V1 + V2 };
	};
};

struct SubOperator
{
	template <int V1, int V2>
	struct op
	{
		enum {	val = V1 - V2 };
	};
};

#endif // __MATH_H