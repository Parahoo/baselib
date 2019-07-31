//使用c++11标准库转换字符编码
 
#include <stdio.h>
#include <locale>
#include <codecvt>

#include "stdc++.h"
#include "stdstrencodingtrans.h"

const std::string ws2s( const std::wstring& src )
{
	std::locale sys_locale("");

	const wchar_t* data_from = src.c_str();
	const wchar_t* data_from_end = src.c_str() + src.size();
	const wchar_t* data_from_next = 0;

	int wchar_size = 4;
	char* data_to = new char[(src.size() + 1) * wchar_size];
	char* data_to_end = data_to + (src.size() + 1) * wchar_size;
	char* data_to_next = 0;

	memset( data_to, 0, (src.size() + 1) * wchar_size );

	typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
    mbstate_t out_state = { 0 };
	auto result = std::use_facet<convert_facet>(sys_locale).out(
		out_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next );
	if( result == convert_facet::ok )
	{
		std::string dst = data_to;
		delete[] data_to;
		return dst;
	}
	else
	{
		printf( "convert error!\n" );
		delete[] data_to;
		return std::string("");
	}
}

const std::wstring s2ws( const std::string& src )
{
	std::locale sys_locale("");
		
	const char* data_from = src.c_str();
	const char* data_from_end = src.c_str() + src.size();
	const char* data_from_next = 0;

	wchar_t* data_to = new wchar_t[src.size() + 1];
	wchar_t* data_to_end = data_to + src.size() + 1;
	wchar_t* data_to_next = 0;

	wmemset( data_to, 0, src.size() + 1 );

	typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
    mbstate_t in_state = { 0 };
	auto result = std::use_facet<convert_facet>(sys_locale).in(
		in_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next );
	if( result == convert_facet::ok )
	{
		std::wstring dst = data_to;
		delete[] data_to;
		return dst;
	}
	else
	{
		printf( "convert error!\n" );
		delete[] data_to;
		return std::wstring(L"");
	}
}

const std::string ws2utf8( const std::wstring& src )
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes( src );
}

const std::wstring utf8_2_ws( const std::string& src )
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	return conv.from_bytes( src );
}

const std::string ts2s(const tstring & src)
{
#ifdef _UNICODE
    return ws2s(src);
#else
    return src;
#endif
}

const std::wstring ts2ws(const tstring & src)
{
#ifdef _UNICODE
    return src;
#else
    return s2ws(src);
#endif
}

const tstring s2ts(const std::string & src)
{
#ifdef _UNICODE
    return s2ws(src);
#else
    return src;
#endif
}

const tstring ws2ts(const std::wstring & src)
{
#ifdef _UNICODE
    return src;
#else
    return ws2s(src);
#endif
}

const std::string ts2utf8(const tstring & src)
{
#ifdef _UNICODE
    return ws2utf8(src);
#else
    wstring un16 = s2ws(src);
    return ws2utf8(un16);
#endif
}

const tstring utf8_2_ts(const std::string & src)
{
#ifdef _UNICODE
    return utf8_2_ws(src);
#else
    wstring un16 = utf8_2_ws(src);
    return ws2s(un16);
#endif
}



int tstring2int(const tstring v)
{
    int r = 0;
    tstringstream(v) >> r;
    return r;
}

double tstring2db(const tstring v)
{
    double r = 0.0;
    tstringstream(v) >> r;
    return r;
}
