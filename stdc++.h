// C++ includes used for precompiling -*- C++ -*-
 
// Copyright (C) 2003-2015 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
 
// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.
 
// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.
 
/** @file stdc++.h
 *  This is an implementation file for a precompiled header.
 */
 
// 17.4.1.2 Headers
 
// C
#ifndef _GLIBCXX_NO_ASSERT
#include <cassert>
#endif
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <codecvt>
 
#if __cplusplus >= 201103L
#include <ccomplex>
#include <cfenv>
#include <cinttypes>
#include <cstdalign>
#include <cstdbool>
#include <cstdint>
#include <ctgmath>
#include <cwchar>
#include <cwctype>
#endif
 
// C++
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>
#include <codecvt>
 
#if __cplusplus >= 201103L
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#endif

using namespace std;
#ifdef UNICODE
typedef std::wstring tstring;
typedef std::wstringstream tstringstream;
#else
typedef std::string tstring;
typedef std::stringstream tstringstream;
#endif

#ifdef UNICODE
#define tsprintf swprintf
#define tstrcat   wcscat
#define tstrchr   wcschr
#define tstrrchr  wcsrchr
#define tstrncmp  wcsncmp
#define tstricmp  wcsicmp
#define tstrncpy  wcsncpy
#define tstrcpy   wcscpy
#define tstrcmp   wcscmp
#define tstrstr   wcsstr
#define tstrlen   wcslen
#define tstrtol   wcstol
#define tstrnicmp wcsnicmp
#define tstrncpy_s wcsncpy_s
#define tstrnlen_s wcsnlen_s

#define ttoi    _wtoi
#define ttof    _wtof
#define ttol    _wtol

#define tstringstream wstringstream
#else
#define tsprintf sprintf
#define tstrcat   strcat
#define tstrchr   strchr
#define tstrrchr  strrchr
#define tstrncmp  strncmp
#define tstricmp  stricmp
#define tstrncpy  strncpy
#define tstrcmp   strcmp
#define tstrstr   strstr
#define tstrlen   strlen
#define tstrtol   strtol
#define tstrnicmp strnicmp
#define tstrncpy_s strncpy_s
#define tstrnlen_s strnlen_s

#define ttoi     atoi
#define ttof     atof
#define ttol     atol

#define tstringstream stringstream
#endif // UNICODE