//
// Created by root on 11/7/17.
//

#include "LUser.h"
#include "BufReadWrite.h"


LUser::LUser()
{
    m_id = 0;
    m_sex = 1;

    m_numOfCard1s = 0;
    m_numOfCard2s = 0;
    m_numOfCard3s = 0;
    m_regTime = 0;
    m_lastLoginTime = 0;
    m_new = 0;
    m_gm = 0;
    m_totalbuynum = 0;
    m_totalplaynum = 0;
    m_creditValue = 0;
    m_count = 0;
    m_coins = 0;
    m_agentId = 0;
    m_type = 0;
    m_bindTime = 0;
    m_status = 1;
    m_phone = 0;
    m_oldActive = 0;
}

LUser::~LUser()
{

}

void LUser::ReadData(muduo::net::Buffer& buf)
{
    Read(buf, m_id);
    Read(buf, m_openid);
    Read(buf, m_nike);
    Read(buf, m_sex);
    Read(buf, m_provice);
    Read(buf, m_city);
    Read(buf, m_country);
    Read(buf, m_headImageUrl);
    Read(buf, m_unioid);
    Read(buf, m_numOfCard1s);
    Read(buf, m_numOfCard2s);
    Read(buf, m_numOfCard3s);
    Read(buf, m_lastLoginTime);
    Read(buf, m_regTime);
    Read(buf, m_new);
    Read(buf, m_gm);
    Read(buf, m_totalbuynum);
    Read(buf, m_totalplaynum);
    Read(buf, m_creditValue);
    Read(buf, m_count);
    Read(buf, m_coins);
    Read(buf, m_agentId);
    Read(buf, m_type);
    Read(buf, m_bindTime);
    Read(buf, m_status);
    Read(buf, m_phone);
    Read(buf, m_oldActive);
}

void LUser::WriteData(muduo::net::Buffer& buf)
{
    Write(buf, m_id);
    Write(buf, m_openid);
    Write(buf, m_nike);
    Write(buf, m_sex);
    Write(buf, m_provice);
    Write(buf, m_city);
    Write(buf, m_country);
    Write(buf, m_headImageUrl);
    Write(buf, m_unioid);
    Write(buf, m_numOfCard1s);
    Write(buf, m_numOfCard2s);
    Write(buf, m_numOfCard3s);
    Write(buf, m_lastLoginTime);
    Write(buf, m_regTime);
    Write(buf, m_new);
    Write(buf, m_gm);
    Write(buf, m_totalbuynum);
    Write(buf, m_totalplaynum);
    Write(buf, m_creditValue);
    Write(buf, m_count);
    Write(buf, m_coins);
    Write(buf, m_agentId);
    Write(buf, m_type);
    Write(buf, m_bindTime);
    Write(buf, m_status);
    Write(buf, m_phone);
    Write(buf, m_oldActive);
}
