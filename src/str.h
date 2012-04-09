// variadic template string builder
template<char... S>
struct StringBuilder
{
	// pushing-back back char(s)
	template<char C>
	struct add_char
	{
		typedef StringBuilder<S..., C> str;
	};
	
	template<char... S2>
	struct add_chars
	{
		typedef StringBuilder<S..., S2...> str;
	};
	
	// pushing-back char(s) conditionally
	template<bool B, char C>
	struct cond_add_char
	{
		typedef typename add_char<C>::str str;
	};
	
	template<char C>
	struct cond_add_char<false, C>
	{
		typedef StringBuilder<S...> str;
	};
	
	template<bool B, char... S2>
	struct cond_add_chars
	{
		typedef typename add_chars<S2...>::str str;
	};
	
	template<char... S2>
	struct cond_add_chars<false, S2...>
	{
		typedef StringBuilder<S...> str;
	};
	
	// final string variable
	static const char value[sizeof...(S)+1];
};

template<char... S>
const char StringBuilder<S...>::value[sizeof...(S)+1] = {
	S...
};

// dimension texts constrains
constexpr char dimensionTexts[] = {	'm',
									'k', 'g',
									's'};

constexpr int dimensionTextLengths[] = {	1,
											2,
											1};

// dimension text helper
// recursion through text lengths
template <int N, int Start, int Len, typename B>
struct DimensionTextHelper
{
	typedef typename DimensionTextHelper<N-1, Start+dimensionTextLengths[N-1], Len,B>::str str;
};

// and StringBuilder add_char concatenation
template <int Start, int Len, typename B>
struct DimensionTextHelper<0, Start, Len, B>
{
	typedef typename DimensionTextHelper<0, Start+1, Len-1, typename B::template add_char<dimensionTexts[Start]>::str>::str str;
};

template <int Start, typename B>
struct DimensionTextHelper<0, Start, 0, B>
{
	typedef B str;
};

// unit text helper
// conditions for first and (-)1 dimension
template<int N, typename B, int D, bool is_first>
struct UnitTextHelper
{
	typedef typename B::template cond_add_char<(is_first==true & D<0), '/'>::str begin;
	typedef typename begin::template cond_add_char<(is_first==false & D<0), '/'>::str div;
	typedef typename div::template cond_add_char<(is_first==false & D>0), '*'>::str mul;
	typedef typename DimensionTextHelper<N, 0, dimensionTextLengths[N], mul>::str dimension;
	typedef typename dimension::template cond_add_char<(D != 1 & D != -1 & D<0), (char)-D+48>::str min_val;
	typedef typename min_val::template cond_add_char<(D != 1 & D != -1 & D>0), (char)D+48>::str str;
};

template<int N, typename B, bool is_first>
struct UnitTextHelper<N, B, 0, is_first>
{
	typedef B str;
};

// unit text implementation
// created using recursive dimension execution
template<int N, bool is_first, typename B, int...>
struct UnitTextImpl;

template<int N, bool is_first, typename B, int Head, int... Tail>
struct UnitTextImpl<N, is_first, B, Head, Tail...>
{
	typedef typename UnitTextImpl<N+1, (is_first==true & Head==0), typename UnitTextHelper<N, B, Head, is_first>::str, Tail...>::str str;
};

template<int N, bool is_first, typename B>
struct UnitTextImpl<N, is_first, B>
{
	typedef B str;
};