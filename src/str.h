template<char... S>
struct StringBuilder
{
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

	/*template<char C>
	using add_char = StringBuilder<S..., C>;
	
	template<char... S2>
	using add_chars = StringBuilder<S..., S2...>;*/
	
	
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
	static const char value[sizeof...(S)+1];
};

template<char... S>
const char StringBuilder<S...>::value[sizeof...(S)+1] = {
	S...
};



/*
template<bool B, char... S>
struct IsUnit
{
	static const char value[sizeof...(S)+1];
};

template<bool B, char... S>
const char IsUnit<S...>::value[sizeof...(S)+1] = {
	S...
};
*/


template<int N, char... P>
struct UnitText
{
	static const char value[sizeof...(P)+1];
};
template<int N, char... P>
const char UnitText<N, P...>::value[sizeof...(P)+1] = {
	P...
};