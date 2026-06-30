//
// Created by root on 11/2/17.
//

#ifndef PROJECT_LBASE_H
#define PROJECT_LBASE_H

#include <cctype>
#include <cstdio>
#include <ctime>
#include <cstdarg>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <sstream>
#include <algorithm>
//#include <mysql.h>

#define sprintf_s snprintf
#define gmtime_s gmtime_r


//基本的整数类型如下
//char(8) short(16) int(32) float(32) double(64)

typedef char Lchar;
typedef unsigned char Luchar;

typedef short Lshort;
typedef unsigned short Lushort;

typedef int	Lint;
typedef unsigned int Luint;

typedef long long Llong;

typedef float Lfloat;

typedef double Ldouble;

typedef std::size_t Lsize;

typedef std::string Lstring;


#endif //PROJECT_LBASE_H
