//
// Created by root on 9/14/17.
// 网关服务上的用户管理
//

#ifndef PROJECT_GATEUSER_H
#define PROJECT_GATEUSER_H

#include <muduo/net/TcpConnection.h>
#include <muduo/base/Singleton.h>
#include <map>

using namespace muduo;
using namespace muduo::net;


struct GateUser
{
    std::string			m_strUUID;		    //玩家的UUID
    std::string			m_strKey;		    //玩家分配的key
    unsigned long       m_uMsgOrder = 0;	//玩家序列
    Timestamp			m_timeActive;	    //玩家活动时间,毫秒
    int			        m_login = 0;        //网关上接收过玩家消息
    int			        m_userState = 0;	//玩家当前状态
    int			        m_logicID = 0;		//玩家在哪个logic上面
    TcpConnectionPtr    connPtr_ = NULL;    //网关与玩家的连接对象

    int  GetUserState(){return m_userState;}
    void SetUserState(int nValue){m_userState = nValue;}
    int  GetUserLogicID(){return m_logicID;}
    void SetUserLogicID(int nValue){m_logicID = nValue;}
};

typedef std::shared_ptr<GateUser> GateUserPtr;


class GateUserManager
{
public:
    GateUserPtr CreateUser(const std::string& strUUID, const TcpConnectionPtr& conn);

    GateUserPtr FindUser(const std::string& strUUID);
    GateUserPtr FindUser(const TcpConnectionPtr& conn);

    void AddUser(const GateUserPtr& user);
    void DelUser(const std::string& strUUID);

    const std::map<std::string, GateUserPtr>& GetAllUsers();

private:
    std::string generateKey();

private:
    std::map<std::string, GateUserPtr> uuidUserMap_;
    std::map<TcpConnectionPtr, GateUserPtr> connUserMap_;
};

#define gGateUserManager muduo::Singleton<GateUserManager>::instance()


#endif //PROJECT_GATEUSER_H
