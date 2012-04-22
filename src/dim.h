#ifndef __DIM_H
#define __DIM_H

#include "list.h"

// dimensions recursion list implementation
// multiple dimension ints required
template<int... Dims>
using DimensionsImpl = ListElementImpl<Dims...>;

// dimensions init struct
template<int... Dims>
using Dimensions = List<Dims...>;

#endif // DIM_H