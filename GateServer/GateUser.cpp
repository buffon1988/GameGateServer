//
// Created by root on 9/14/17.
//

#include "GateUser.h"
#include "BaseFunc.h"
#include <muduo/base/Logging.h>


GateUserPtr GateUserManager::CreateUser(const std::string& strUUID, const TcpConnectionPtr& conn)
{
    if (FindUser(strUUID))
    {
        LOG_INFO << "CreateUser The user exist. UUID=" << strUUID;
        return NULL;
    }

    GateUserPtr pUser(new GateUser());
    pUser->m_strUUID	= strUUID;
    pUser->m_strKey		= generateKey();
    pUser->m_uMsgOrder	= static_cast<unsigned long>(L_Rand(0, 9999));
    pUser->m_timeActive = Timestamp::now();
    pUser->connPtr_		= conn;
    pUser->m_login		= 0;
    AddUser(pUser);
    return pUser;
}

void GateUserManager::AddUser(const GateUserPtr& user)
{
    if (user.get() == NULL)
    {
        return;
    }

    uuidUserMap_.insert(std::make_pair(user->m_strUUID, user));
    connUserMap_.insert(std::make_pair(user->connPtr_, user));
}

GateUserPtr GateUserManager::FindUser(const std::string& strUUID)
{
    auto it = uuidUserMap_.find(strUUID);
    if(it == uuidUserMap_.end())
    {
        return NULL;
    }
    return it->second;
}

GateUserPtr GateUserManager::FindUser(const TcpConnectionPtr& conn)
{
    auto it = connUserMap_.find(conn);
    if(it == connUserMap_.end())
    {
        return NULL;
    }
    return it->second;
}

void GateUserManager::DelUser(const std::string& strUUID)
{
    GateUserPtr user = FindUser(strUUID);
    if(user == NULL || user.get() == NULL)
    {
        LOG_INFO << "DelUser The user not exist. UUID=" << strUUID;
        return;
    }

    uuidUserMap_.erase(user->m_strUUID);
    connUserMap_.erase(user->connPtr_);
}

const std::map<std::string, GateUserPtr>& GateUserManager::GetAllUsers()
{
    return uuidUserMap_;
}


std::string GateUserManager::generateKey()
{
    static std::string strSeed = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*(){}[];,.?|";
    std::string strKey;
    for(int i = 0; i < 32; i++)
    {
        int iRand = L_Rand(0, static_cast<int>(strSeed.length()-1));
        strKey += strSeed.at(iRand);
    }

    return strKey;
}