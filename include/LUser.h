//
// Created by root on 11/7/17.
//

#ifndef PROJECT_LUSER_H
#define PROJECT_LUSER_H


#include "LBase.h"
#include <muduo/net/Buffer.h>


///比较活跃的老玩家条件：如游戏注册时间超过7天且游戏局数超过50局
#define		OLD_ACTIVE_USER_TIME		7*24*3600	//7天时间(单位秒)
#define		OLD_ACTIVE_USER_PLAYNUM		50			//50大局

class LUser
{
public:
    LUser();
    ~LUser();

    void	ReadData(muduo::net::Buffer& buf);
    void	WriteData(muduo::net::Buffer& buf);

public:
    Lint		m_id;
    Lstring		m_openid;
    Lstring		m_nike;
    Lint		m_sex;
    Lstring		m_provice;//省
    Lstring		m_city;//城市
    Lstring		m_country;//地区
    Lstring		m_headImageUrl;//头像URL
    Lstring		m_unioid;
    Lint		m_numOfCard1s;//卡片数量
    Lint		m_numOfCard2s;
    Lint		m_numOfCard3s;
    Lint		m_regTime;//注册时间
    Lint		m_lastLoginTime;//最后一次登录时间
    Lint		m_new;
    Lint		m_gm;
    Lint		m_totalbuynum;//总共购买数量
    Lint		m_totalplaynum;//总共玩的大局次数
    Lint        m_creditValue;//信用值
    Lint        m_count;
    Lint        m_coins;  //金币
    Llong		m_agentId;//推广员id
    Lint		m_type;//登陆平台标识 1:Android, 2:ios
    Lint		m_bindTime;//绑定时间
    Lint		m_status;//玩家账号状态 0删除，1正常，2封号
    Llong		m_phone;//玩家绑定的电话号码
    Lint		m_oldActive; // 标识是比较活跃的老玩家

    enum USER_STATUS
    {
        USER_STATUS_DELETE  = 0, //删除
        USER_STATUS_NORMAL  = 1, //正常
        USER_STATUS_FENGHAO = 2  //封号
    };
};


#endif //PROJECT_LUSER_H
