//
// Created by root on 9/6/17.
//

#ifndef PROJECT_BASEFUNC_H
#define PROJECT_BASEFUNC_H

#include "LBase.h"


std::string calcMd5ToHex(const void* pData, std::size_t uDataLen);


int L_Rand(int begin, int end);

/*
*	解析1,2,3这种类型字符串
*/
void L_ParseString(Lstring src, std::vector<Lstring>& des,Lstring par=",");

/*
*	解析1,2;3,4;这种类型字符串
*/
void L_ParseString(Lstring src, std::map<Lstring,Lstring>& des,Lstring pa1=";",Lstring pa2=",");


//BKDR Hash
unsigned int caculateHashKey(const char* pchValue, int iDataLen);

//更新随机因子
void updateRandomFactor();

// 字符串时间转成time_t
time_t convert_string_to_time_t(const std::string & time_string);

// 判断活动时间是否有效
bool checkStrTimeValid(const Lstring& strBeginTime, const Lstring& strEndTime);
bool checkTimeValid(const time_t& BeginTime, const time_t& EndTime);

// 判断时间配置是否合法
bool checkActiveTimeSetVaild(const Lstring&  strBeginTime, const Lstring&  strEndTime);
bool checkActiveTimeSetVaild(const time_t& BeginTime, const time_t& EndTime);

#endif //PROJECT_BASEFUNC_H
