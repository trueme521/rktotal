//////////////////////////////////////////////////////////////////////
///	 @file:   	ci_char_traits.h
///	 @author:	luopeng
///  @date:   	2004/11/21
///	
///	 @brief: 	不区分大小写的char_traits
//////////////////////////////////////////////////////////////////////
#ifndef CI_CHAR_TRAITS
#define CI_CHAR_TRAITS

#include <iosfwd>
#include <ostream>

struct ci_char_traits : public std::char_traits<TCHAR>							
              // just inherit all the other functions
              //  that we don't need to replace
{	
	// Just for vs2005 to hide C4996 warning
	// Because the class is inherited from std::char_traits, It's safe.
	typedef std::_Secure_char_traits_tag _Secure_char_traits;

	static bool eq(TCHAR c1, TCHAR c2)
    {
		return _totupper(c1) == _totupper(c2);
	}

	static bool lt(TCHAR c1, TCHAR c2)
    { 
		return _totupper(c1) <  _totupper(c2); 
	}

	static int compare(const TCHAR* s1, const TCHAR* s2, size_t n)
    {
		return _memicmp(s1, s2, n * sizeof(TCHAR)); 
	}
	
	static const TCHAR* find(const TCHAR* s, int n, TCHAR a)
	{
		while (n-- > 0 && _totupper(*s) != _totupper(a))
		{
			++s;
		}
		return n >= 0 ? s : 0;
	}
};

template<class _E, class _Tr, class _A> inline
std::basic_ostream<_E, _Tr>& __cdecl operator<<(std::basic_ostream<_E, _Tr>& _O,
												const std::basic_string<_E, ci_char_traits, _A>& _X)
{
	_O << _X.c_str();
	return (_O);
}

#endif //CI_CHAR_TRAITS