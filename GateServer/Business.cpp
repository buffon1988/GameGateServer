//
// Created by root on 3/19/18.
//

#include "Business.h"
#include "Config.h"


Business::Business()
    : BusinessThread_(new muduo::Thread(std::bind(&Business::run, this), "BusinessThread")),
      time_(Timestamp::now()),
      backend_(std::bind(&Business::PushMsg, this, _1))
{
}

bool Business::Init()
{
    std::string lManagerIp = gConfig.GetLogicManagerIp();
    uint16_t port = static_cast<uint16_t>(gConfig.GetLogicManagerPort());
    lManagerAddr_ = InetAddress(lManagerIp, port);
    return true;
}

void Business::Start()
{
    BusinessThread_->start();
    backend_.Connect(lManagerAddr_.toIp(), lManagerAddr_.toPort(), "backend_LogicManager");
}

void Business::PushMsg(const LMsgPtr& msg)
{
    if (msg)
    {
        msgQueue_.put(msg);
    }
}

void Business::run()
{
    while (BusinessThread_->started())
    {
        if (msgQueue_.size() > 0)
        {
            handleMsg(msgQueue_.take());
        }
        else
        {
            muduo::CurrentThread::sleepUsec(1000); // 1ms
        }
        // 执行定时任务
        timer();
    }
}

void Business::timer()
{
    Timestamp now(Timestamp::now());
    if (timeDifference(now, time_) > 30)
    {
        time_ = now;
        checkClientConn();
        checkGateUserState();
        checkLManagerHeartbeat();
        checkLServerHeartbeat();
    }
}

void Business::handleMsg(const LMsgPtr& msg)
{
    //LOG_INFO << "----handleMsg:" << msg->m_msgId << ", msg in tid = " << muduo::CurrentThread::tid();
    switch (msg->m_msgId)
    {
        case MSG_SOCKET_IN:
            handleSocketIn(std::static_pointer_cast<LMsgSocketIn>(msg));
            break;
        case MSG_SOCKET_OUT:
            handleSocketOut(std::static_pointer_cast<LMsgSocketOut>(msg));
            break;
        case MSG_LMG_2_G_SYNC_LOGIC:
            handleLogicInfo(std::static_pointer_cast<LMsgLMG2GateLogicInfo>(msg));
            break;
        case MSG_LMG_2_GL_COINS_SERVER_INFO:
            break;
        case MSG_LMG_2_G_HORSE_INFO:
            handleHorseInfo(std::static_pointer_cast<LMsgLMG2GHorseInfo>(msg));
            break;
        case  MSG_L_2_G_MODIFY_USER_STATE:
            handleModifyUserState(std::static_pointer_cast<LMsgL2GModifyUserState>(msg));
            break;
        case MSG_CONVERT_CLIENTMSG:
            handleConverCLientMsg(std::static_pointer_cast<LMsgConvertClientMsg>(msg));
            break;
        case MSG_L_2_G_USER_MSG:
        {
            LMsgL2GUserMsg* lmsg = std::static_pointer_cast<LMsgL2GUserMsg>(msg).get();
            GateUserPtr user = gGateUserManager.FindUser(lmsg->m_strUUID);
            if (user)
            {
                user->connPtr_->send(lmsg->m_dataBuff.get());
            }
            else
            {
                LOG_DEBUG << "handleMsg MSG_L_2_G_USER_MSG user not exiest: "<< lmsg->m_strUUID;
            }
            break;
        }
        case MSG_L_2_G_USER_OUT_MSG:
        {
            LMsgL2GUserOutMsg*lmsg = std::static_pointer_cast<LMsgL2GUserOutMsg>(msg).get();
            GateUserPtr user = gGateUserManager.FindUser(lmsg->m_strUUID);
            if (user)
            {
                user->connPtr_->shutdown();
                gGateUserManager.DelUser(user->m_strUUID);
            }
            else
            {
                LOG_DEBUG << "handleMsg MSG_L_2_G_USER_OUT_MSG user not exiest: " << lmsg->m_strUUID;
            }
            break;
        }
        case MSG_HEARDBEAT_REPLY:
            handleHeartbeatReply(std::static_pointer_cast<LMsgHeartBeatReplyMsg>(msg));
            break;
        default:
        {
            LOG_ERROR << "Don't do with the message id. msgId = " << msg->m_msgId;
            if (!isBackendConn(msg->connPtr_))
                msg->connPtr_->shutdown();
            break;
        }
    }
}

void Business::handleSocketIn(const std::shared_ptr<LMsgSocketIn>& msg)
{
    if (!msg) return;
    muduo::string peerIpPort = msg->connPtr_->peerAddress().toIpPort();

    // 与逻辑管理服务器连接成功，发送网关信息到逻辑管理服务器
    if (lManagerAddr_.toIpPort() == peerIpPort)
    {
        lManagerConn_ = msg->connPtr_;
        LMsgG2LMGLogin login;
        login.m_id = gConfig.GetGateServerId();
        login.m_ip = gConfig.GetGateServerIp();
        login.m_port = gConfig.GetListenPort();
        login.m_key = gConfig.GetGateServerKey();
        lManagerConn_->send(login.GetSendBuffer());
        LOG_INFO << "Connect LogicManager " << peerIpPort << " success.";
    }
    else
    {
        // 与某个逻辑服务器连接成功，发送网关信息到这个逻辑服务器
        auto it = logicIpPortList_.find(peerIpPort);
        if (it != logicIpPortList_.end())
        {
            auto itLogic = logicInfoMap_.find(it->second);
            if (itLogic != logicInfoMap_.end())
            {
                itLogic->second.connPtr_ = msg->connPtr_;

                LMsgG2LLogin login;
                login.m_id = gConfig.GetGateServerId();
                login.m_ip = gConfig.GetGateServerIp();
                login.m_port = gConfig.GetListenPort();
                login.m_key = gConfig.GetGateServerKey();
                msg->connPtr_->send(login.GetSendBuffer());
                LOG_INFO << "Connect LogicServer " << peerIpPort << " success.";
            }
        }
        else
        {
            // 其他的连接上来都暂时认为是客户端，连接对象保存在一个容器中，后续该连接
            // 验证通过的话会从这个容器中剔除，没有验证通过到会定时断开连接并从容器中清理掉
            unAuthClientInMap_[msg->connPtr_] = muduo::Timestamp::now();
        }
    }
}

void Business::handleSocketOut(const std::shared_ptr<LMsgSocketOut>& msg)
{
    if (!msg) return;
    unAuthClientInMap_.erase(msg->connPtr_);
    GateUserPtr user = gGateUserManager.FindUser(msg->connPtr_);
    if (user)
    {
        if (user->m_login)
        {
            dealUserOut(user);
        }
        gGateUserManager.DelUser(user->m_strUUID);
    }
}

void Business::handleHeartbeatReply(const std::shared_ptr<LMsgHeartBeatReplyMsg>& msg)
{
    if(!msg)
    {
        return;
    }

    int fromwho = msg->m_fromWho;
    if ( fromwho < HEARBEAT_WHO_UNKNOWN || fromwho > HEARBEAT_WHO_NUM )
    {
        fromwho = HEARBEAT_WHO_UNKNOWN;
    }
    LOG_INFO << "Reply heartbeat from " << HearBeat_Name[fromwho] << "--" << msg->m_iServerId;
    m_heartBeat_.erase(msg->connPtr_);
}

void Business::handleLogicInfo(const std::shared_ptr<LMsgLMG2GateLogicInfo>& msg)
{
    if (!msg)
    {
        return;
    }
    if (msg->m_ID <= 0)
    {
        LOG_ERROR << "Business::HandleLogicInfo m_ID = " << msg->m_ID;
        return;
    }

    std::map<int, LogicInfo> mapFromLM;
    for(int i = 0; i < msg->m_count; ++i)
    {
        int logicId = msg->m_logic[i].m_id;
        mapFromLM[logicId] = msg->m_logic[i];
    }

    //遍历哪些 LogicServer 已经不在 LogicManager 上了
    for(auto it = logicInfoMap_.begin(); it != logicInfoMap_.end();)
    {
        if(mapFromLM.find(it->first) == mapFromLM.end())
        {
            //这个 LogicServer 已经不在 LogicManager 上了
            it->second.connPtr_->shutdown();
            logicInfoMap_.erase(it++);
        }
        else
        {
            //如果依然存在
            mapFromLM.erase(it->first);
            it++;
        }
    }
    //加入新进入的
    for(auto it = mapFromLM.begin(); it != mapFromLM.end(); ++it)
    {
        logicInfoMap_[it->first] = it->second;
        uint16_t port = static_cast<uint16_t>(it->second.m_port);
        InetAddress logicAddr(it->second.m_ip, port);
        backend_.Connect(logicAddr.toIp(), port, "backend_LogicServer");
        logicIpPortList_.insert(std::make_pair(logicAddr.toIpPort(), it->first));
    }
}

void Business::handleHorseInfo(const std::shared_ptr<LMsgLMG2GHorseInfo>& msg)
{
    if (!msg)
    {
        return;
    }
    LMsgS2CHorseInfo horse;
    horse.m_str = msg->m_str;
    horse.m_type = msg->m_type;
    sendMsgToAllUser(LMsgPtr(&horse));
}

void Business::handleModifyUserState(const std::shared_ptr<LMsgL2GModifyUserState>& msg)
{
    if (!msg)
    {
        return;
    }

    GateUserPtr user = gGateUserManager.FindUser(msg->m_strUUID);
    if (user)
    {
        LOG_INFO << "handleModifyUserState userid:" << user->m_strUUID
                 << ", State:" << msg->m_userstate << ", serverID:" << msg->m_logicID;
        user->SetUserState(msg->m_userstate);
        user->SetUserLogicID(msg->m_logicID);
    }
    else
    {
        LOG_ERROR << "BsToLServer::handleModifyUserState Not Exist userid=" << msg->m_strUUID;
    }
}

void Business::handleConverCLientMsg(const std::shared_ptr<LMsgConvertClientMsg>& msg)
{
    if(!msg)
    {
        return;
    }

    //登录gate消息不用判断消息安全
    if(msg->msgEntityPtr_->m_msgId == MSG_C_2_S_LOGIN_GATE)
    {
        doClientLoginGate(std::static_pointer_cast<LMsgC2SLoginGate>(msg->msgEntityPtr_));
        return;
    }

    if (!isClientMsgSafe(msg->verifyHeadPtr_))
    {
        LOG_ERROR << "The message format is wrong. MsgId = " << msg->verifyHeadPtr_->m_msgId;
        msg->connPtr_->shutdown();
        return;
    }

    if(msg->msgEntityPtr_->m_msgId == MSG_C_2_S_HEART)
    {
        doClientHeartbeat(std::static_pointer_cast<LMsgC2SHeart>(msg->msgEntityPtr_));
    }
    else
    {
        doClientOtherMsg(msg->msgEntityPtr_, msg->msgBufPtr_);
    }
}

void Business::doClientLoginGate(const std::shared_ptr<LMsgC2SLoginGate>& msg)
{
    if(!msg)
    {
        return;
    }

    GateUserPtr user = gGateUserManager.FindUser(msg->m_strUserUUID);
    if(user.get())
    {
        if(user->m_login)
        {
            dealUserOut(user);
        }

        LMsgS2CShutDown msgShutDown;
        msgShutDown.m_nShutDownInfo = 1;// 1 - 用户在别处登陆
        user->connPtr_->send(msgShutDown.GetSendBuffer());

        //删除以前的
        user->connPtr_->shutdown();
        gGateUserManager.DelUser(user->m_strUUID);
    }

    user = gGateUserManager.CreateUser(msg->m_strUserUUID, msg->connPtr_);
    if(user.get() == NULL)
    {
        LOG_ERROR << "Fail to create user";
        return;
    }

    LMsgS2CLoginGate msgRet;
    msgRet.m_strUserUUID = user->m_strUUID;
    msgRet.m_strKey      = user->m_strKey;
    msgRet.m_uMsgOrder   = user->m_uMsgOrder;
    user->connPtr_->send(msgRet.GetSendBuffer());

    //从没有被认证过的客户端连接列表中移除
    unAuthClientInMap_.erase(msg->connPtr_);
}

void Business::doClientHeartbeat(const std::shared_ptr<LMsgC2SHeart>& msg)
{
    if(!msg)
    {
        return;
    }

    GateUserPtr user = gGateUserManager.FindUser(msg->connPtr_);
    if(user.get() == NULL)
    {
        LOG_ERROR << "User is not exist " <<  msg->m_msgId;
        msg->connPtr_->shutdown();
        return;
    }

    LMsgS2CHeart msgHeartBeat;
    msgHeartBeat.m_time = static_cast<int>(Timestamp::now().microSecondsSinceEpoch());
    msg->connPtr_->send(msgHeartBeat.GetSendBuffer());

    //消息时间
    user->m_login += 1;
    user->m_timeActive = Timestamp::now();
}

void Business::doClientOtherMsg(const LMsgPtr& msg, const BufferPtr& msgOriginData)
{
    if(!msg || !msgOriginData)
    {
        return;
    }

    GateUserPtr user = gGateUserManager.FindUser(msg->connPtr_);
    if(!user)
    {
        LOG_ERROR << "User is not exist " << msg->m_msgId;
        msg->connPtr_->shutdown();
        return;
    }

    LMsgG2LUserMsg msgG2L;
    msgG2L.m_strUUID	= user->m_strUUID;
    msgG2L.m_ip			= std::string(msg->connPtr_->peerAddress().toIp().data());
    msgG2L.m_dataBuff   = msgOriginData;

    //区分玩家状态，将消息发送至 LogicServer 或 LogicManager
    if(user->GetUserLogicID() > 0 && user->GetUserState() == LGU_STATE_DESK)
    {
        //玩家在桌子中 发送消息到 LogicServer
        LogicInfo* logic = getLogicInfoById(user->GetUserLogicID());
        if (logic)
        {
            if (logic->connPtr_)
                logic->connPtr_->send(msgG2L.GetSendBuffer());
        }
        else
        {
            if (user->m_login)
            {
                dealUserOut(user);
            }
            //当前逻辑不存在，踢下该用户，让他重连
            user->connPtr_->shutdown();
            gGateUserManager.DelUser(user->m_strUUID);
            return;
        }
    }
    else
    {
        //玩家在大厅中 发送消息到 LogicManager
        if (lManagerConn_)
        {
            lManagerConn_->send(msgG2L.GetSendBuffer());
        }
    }

    user->m_login += 1;
    user->m_timeActive = Timestamp::now();
}

bool Business::isClientMsgSafe(const std::shared_ptr<LMsgC2SVerifyHead>& msg)
{
    GateUserPtr user = gGateUserManager.FindUser(msg->connPtr_);
    if(!user)
    {
        LOG_ERROR << "A invalid user";
        msg->connPtr_->shutdown();
        return false;
    }

    if(msg->m_strVerifyKey.compare(user->m_strKey) != 0)
    {
        LOG_ERROR << "Key is wrong";
        msg->connPtr_->shutdown();
        return false;
    }

    unsigned long msgOrder = static_cast<unsigned long>(msg->m_lMsgOrder);
    if(msgOrder <= user->m_uMsgOrder)
    {
        LOG_ERROR << "Wrong msg order. C=" << msg->m_lMsgOrder << ", S=" << user->m_uMsgOrder;
        msg->connPtr_->shutdown();
        return false;
    }
    user->m_uMsgOrder = msgOrder;

    return true;
}

void Business::checkClientConn()
{
    Timestamp now(Timestamp::now());
    for (auto it = unAuthClientInMap_.begin(); it != unAuthClientInMap_.end(); )
    {
        //超过30秒则踢出
        if (timeDifference(now, it->second) >= 30)
        {
            it->first->shutdown();
            unAuthClientInMap_.erase(it++);
        }
        else
        {
            it++;
        }
    }
}

void Business::checkGateUserState()
{
#ifdef UNCHECKHEARTBEAT
    return;
#endif

    Timestamp now(Timestamp::now());
    std::list<TcpConnectionPtr> kickList;//记录不活动也没心跳数据的连接
    const std::map<std::string, GateUserPtr>& allUsers = gGateUserManager.GetAllUsers();
    for(auto it = allUsers.begin(); it != allUsers.end(); ++it)
    {
        if (timeDifference(now, it->second->m_timeActive) >= 30)
        {
            kickList.push_back(it->second->connPtr_);
        }
    }

    for(auto it = kickList.begin(); it != kickList.end(); it++)
    {
        TcpConnectionPtr conn = *it;
        if (conn)
            conn->shutdown();
    }
}


void Business::checkLManagerHeartbeat()
{
    if (!lManagerConn_)
        return;

    if(m_heartBeat_.find(lManagerConn_) != m_heartBeat_.end())
    {
        //如果存在上次的心跳也就是说没有收到心跳应答包，说明网络存在问题
        //网络断开后 ClientNet backend_ 会自动重连 LogicManager
        LOG_ERROR << "GateServer to LogicManager network had problem. Reconnect ...";
        return;
    }
    LMsgHeartBeatRequestMsg msgHeatbeat;
    msgHeatbeat.m_fromWho = HEARBEAT_WHO_GATESERVER;
    msgHeatbeat.m_iServerId = gConfig.GetGateServerId();
    lManagerConn_->send(msgHeatbeat.GetSendBuffer());
    m_heartBeat_.insert(lManagerConn_);
}

void Business::checkLServerHeartbeat()
{
    for(auto it = logicInfoMap_.begin(); it != logicInfoMap_.end(); ++it)
    {
        if (!it->second.connPtr_)
            continue;
        if(m_heartBeat_.find(it->second.connPtr_) != m_heartBeat_.end())
        {
            //如果存在上次的心跳也就是说没有收到心跳应答包，说明网络存在问题
            //网络断开后 ClientNet backend_ 会自动重连 LogicServer
            LOG_ERROR << "GateServer to LogicServer network had problem. Reconnect ...";
            continue;
        }
        LMsgHeartBeatRequestMsg msgHeatbeat;
        msgHeatbeat.m_fromWho = HEARBEAT_WHO_GATESERVER;
        msgHeatbeat.m_iServerId = gConfig.GetGateServerId();
        it->second.connPtr_->send(msgHeatbeat.GetSendBuffer());
        m_heartBeat_.insert(it->second.connPtr_);
    }
}

void Business::dealUserOut(const GateUserPtr& user)
{
    LMsgG2LUserOutMsg msg;
    msg.m_strUUID = user->m_strUUID;
    if (lManagerConn_)
    {
        //通知LogicManager下线
        lManagerConn_->send(msg.GetSendBuffer());
    }

    if (user->GetUserLogicID() > 0)
    {
        LogicInfo* logic = getLogicInfoById(user->GetUserLogicID());
        if (logic)
        {
            //通知LogicServer下线
            logic->connPtr_->send(msg.GetSendBuffer());
        }
    }
}

void Business::sendMsgToAllUser(const LMsgPtr& msg)
{
    if(!msg)
    {
        return;
    }

    const std::map<std::string, GateUserPtr>& allUsers = gGateUserManager.GetAllUsers();
    for(auto it = allUsers.begin(); it != allUsers.end(); ++it)
    {
        if (it->second->connPtr_)
        {
            it->second->connPtr_->send(msg->GetSendBuffer());
        }
    }
}

LogicInfo* Business::getLogicInfoById(int id)
{
    if (logicInfoMap_.count(id))
        return &logicInfoMap_[id];
    return NULL;
}

bool Business::isBackendConn(const TcpConnectionPtr& conn)
{
    if (conn == lManagerConn_)
        return true;

    for (auto it = logicInfoMap_.begin(); it != logicInfoMap_.end(); ++it)
    {
        if (conn == it->second.connPtr_)
            return true;
    }

    return false;
}

