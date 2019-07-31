#pragma once

// wstring to ANSI
const std::string ws2s( const std::wstring& src );

// ANSI to wstring
const std::wstring s2ws( const std::string& src );

// wstring to utf-8
const std::string ws2utf8( const std::wstring& src );

// utf-8 to wstring
const std::wstring utf8_2_ws( const std::string& src );



// tstring to ANSI
const std::string ts2s( const tstring& src );

// tstring to wstring
const std::wstring ts2ws( const tstring& src );

// ANSI to tstring
const tstring s2ts( const std::string& src );

// wstring to tstring
const tstring ws2ts( const std::wstring& src );


// tstring to utf8
const std::string ts2utf8( const tstring& src );

// ANSI to tstring
const tstring utf8_2_ts( const std::string& src );



// 去除头空格
template<class T>
T& LTrim(T& t)
{
    t.erase(t.begin(),
        std::find_if(t.begin(), t.end(), [](int i)->bool {return !isspace(i & 0xFF); }));
    return t;
}

//去除尾空格
template<class T>
T& RTrim(T& t)
{
    t.erase(std::find_if(t.rbegin(), t.rend(), [](int i)->bool {return !isspace(i & 0xFF); }).base(), t.end());
    return t;
}

//去除首尾空格
template<class T>
T& Trim(T& t)
{
    return RTrim(LTrim(t));
}


int  tstring2int(const tstring v);

double tstring2db(const tstring v);

tstring GetTargetPath(const tstring v);