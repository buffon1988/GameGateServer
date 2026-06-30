//
// Created by root on 9/6/17.
//

#include "BaseFunc.h"
#include <muduo/base/Timestamp.h>
#include <openssl/md5.h>


std::string calcMd5ToHex(const void* pData, std::size_t uDataLen)
{
    if (pData == NULL || uDataLen == 0)
    {
        return "";
    }

    unsigned char sign[128] = {0};
    MD5(static_cast<const unsigned char*>(pData), uDataLen, sign);

    //转换成16进制
    char signHex[128] = {0};
    for(int i = 0 ; i < 16; ++i)
    {
        sprintf(signHex + i*2, "%02x", sign[i]);
    }

    return std::string(signHex);
}


int L_Rand(int begin, int end)
{
    static bool init = false;
    if(!init)
    {
        //随机种子
        srand(static_cast<unsigned int>(muduo::Timestamp::now().secondsSinceEpoch()));
        init = true;
    }

    if(begin > end)
    {
        int tmp = begin;
        begin = end;
        end = tmp;
    }

    int seed = static_cast<int>((static_cast<double>(rand()) / RAND_MAX * 1000000000));

    return seed%(end-begin+1) + begin;
}

void L_ParseString(Lstring src, std::vector<Lstring>& des,Lstring par)
{
    des.clear();
    Lstring::size_type pos = src.find(par);
    while(pos != Lstring::npos)
    {
        Lstring sub = src.substr(0,pos);
        src.erase(0,pos+1);
        des.push_back(sub);
        pos = src.find(par);
    }
    if(src.size() != 0)
    {
        des.push_back(src);
    }
}

/*
*	解析1,2;3,4;这种类型字符串
*/
void L_ParseString(Lstring src, std::map<Lstring,Lstring>& des,Lstring pa1,Lstring pa2)
{
    des.clear();
    Lstring::size_type pos = src.find(pa1);
    while(pos != Lstring::npos)
    {
        Lstring sub = src.substr(0,pos);
        src.erase(0,pos+1);
        pos = src.find(pa1);

        Lstring::size_type pos1 = sub.find(pa2);
        if(pos1 != Lstring::npos)
        {
            Lstring suba = sub.substr(0,pos1);
            Lstring& subb = sub.erase(0,pos1+1);
            des[suba] = subb;
        }

    }
    if(src.size() != 0)
    {
        Lstring::size_type pos1 = src.find(pa2);
        if(pos1 != Lstring::npos)
        {
            Lstring suba = src.substr(0,pos1);
            Lstring& subb = src.erase(0,pos1+1);
            des[suba] = subb;
        };
    }
}

//BKDR Hash
unsigned int caculateHashKey(const char* pchValue, int iDataLen)
{
    if(pchValue == NULL)
    {
        return 0;
    }

    unsigned int uSeed = 131 ;// 31 131 1313 13131 131313 etc..
    unsigned int uHash = 0;

    while(*pchValue && iDataLen > 0)
    {
        uHash = uHash*uSeed + (*pchValue++);
        iDataLen--;
    }

    return uHash;
}

void updateRandomFactor()
{
    static Lstring strSeed = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*(){}[];,.?|";
    Lstring strKey;
    for(int i = 0; i < 128; i++)
    {
        int iRand = L_Rand(0, strSeed.length() - 1);
        strKey += strSeed.at(iRand);
    }

    unsigned int iHash = caculateHashKey(strKey.c_str(), strKey.length());
    srand(iHash);
}

time_t convert_string_to_time_t(const std::string & time_string)
{
    struct tm tm1;
    time_t time1;
    sscanf(time_string.c_str(), "%d/%d/%d %d:%d:%d" ,
               &(tm1.tm_year),
               &(tm1.tm_mon),
               &(tm1.tm_mday),
               &(tm1.tm_hour),
               &(tm1.tm_min),
               &(tm1.tm_sec));

    tm1.tm_year -= 1900;
    tm1.tm_mon--;
    tm1.tm_isdst =-1;
    time1 = mktime(&tm1);

    return time1;
}

// 判断活动时间是否有效
bool checkStrTimeValid(const Lstring& strBeginTime, const Lstring& strEndTime)
{
    time_t BeginTime = convert_string_to_time_t(strBeginTime.c_str());
    time_t EndTime = convert_string_to_time_t(strEndTime.c_str());

    return checkTimeValid(BeginTime, EndTime);
}

bool checkTimeValid(const time_t& BeginTime, const time_t& EndTime)
{
    if(BeginTime == -1 || EndTime == -1 || BeginTime == 0 || EndTime == 0)
    {
        return false;
    }

    if(BeginTime >= EndTime)
    {
        return false;
    }

    muduo::Timestamp curTime(muduo::Timestamp::now());
    time_t curSecond = curTime.secondsSinceEpoch();
    if(curSecond >= BeginTime && curSecond <= EndTime)
    {
        return true;
    }
    return false;
}

// 判断时间配置是否合法
bool checkActiveTimeSetVaild(const Lstring&  strBeginTime, const Lstring&  strEndTime)
{
    time_t BeginTime = convert_string_to_time_t(strBeginTime.c_str());
    time_t EndTime = convert_string_to_time_t(strEndTime.c_str());

    return checkActiveTimeSetVaild(BeginTime, EndTime);
}

bool checkActiveTimeSetVaild(const time_t& BeginTime, const time_t& EndTime)
{
    if(BeginTime == -1 || EndTime == -1)
    {
        return false;
    }

    if(BeginTime >= EndTime)
    {
        return false;
    }

    muduo::Timestamp curTime(muduo::Timestamp::now());
    time_t curSecond = curTime.secondsSinceEpoch();
    if(curSecond <= EndTime)
    {
        return true;
    }

    return false;
}

