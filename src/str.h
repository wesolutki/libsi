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

// dimension text helper
// recursion through text lengths
template <const char* Texts, const int* Lens, int N, int Start, int Len, typename B>
struct DimensionTextHelper
{
	typedef typename DimensionTextHelper<Texts, Lens, N-1, Start+Lens[N-1], Len,B>::str str;
};

// and StringBuilder add_char concatenation
template <const char* Texts, const int* Lens, int Start, int Len, typename B>
struct DimensionTextHelper<Texts, Lens, 0, Start, Len, B>
{
	typedef typename DimensionTextHelper<Texts, Lens, 0, Start+1, Len-1, typename B::template add_char<Texts[Start]>::str>::str str;
};

template <const char* Texts, const int* Lens, int Start, typename B>
struct DimensionTextHelper<Texts, Lens, 0, Start, 0, B>
{
	typedef B str;
};

// unit text helper
// conditions for first and (-)1 dimension
template<const char* Texts, const int* Lens, int N, typename B, int D, bool is_first>
struct UnitTextHelper
{
	typedef typename B::template cond_add_char<(is_first==true & D<0), '/'>::str begin;
	typedef typename begin::template cond_add_char<(is_first==false & D<0), '/'>::str div;
	typedef typename div::template cond_add_char<(is_first==false & D>0), '*'>::str mul;
	typedef typename DimensionTextHelper<Texts, Lens, N, 0, Lens[N], mul>::str dimension;
	typedef typename dimension::template cond_add_char<(D != 1 & D != -1 & D<0), (char)-D+48>::str min_val;
	typedef typename min_val::template cond_add_char<(D != 1 & D != -1 & D>0), (char)D+48>::str str;
};

template<const char* Texts, const int* Lens, int N, typename B, bool is_first>
struct UnitTextHelper<Texts, Lens, N, B, 0, is_first>
{
	typedef B str;
};

// unit text implementation
// created using recursive dimension execution
template<const char* Texts, const int* Lens, int N, bool is_first, typename B, int...>
struct UnitTextImpl;

template<const char* Texts, const int* Lens, int N, bool is_first, typename B, int Head, int... Tail>
struct UnitTextImpl<Texts, Lens, N, is_first, B, Head, Tail...>
{
	typedef typename UnitTextImpl<	Texts, 
									Lens, 
									N+1, 
									(is_first==true & Head==0), 
									typename UnitTextHelper<Texts, Lens, N, B, Head, is_first>::str, Tail...>::str str;
};

template<const char* Texts, const int* Lens, int N, bool is_first, typename B>
struct UnitTextImpl<Texts, Lens, N, is_first, B>
{
	typedef B str;
};