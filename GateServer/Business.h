//
// Created by root on 3/19/18.
// 处理业务逻辑主线程
//

#ifndef PROJECT_BUSINESS_H
#define PROJECT_BUSINESS_H


#include "LMsg.h"
#include "LMsgSS.h"
#include "ClientNet.h"
#include "GateUser.h"
#include <muduo/base/Thread.h>
#include <muduo/base/BlockingQueue.h>
#include <muduo/base/Singleton.h>

using namespace muduo;


class Business
{
public:
    Business();
    bool Init();
    void Start();

    void PushMsg(const LMsgPtr& msg);

private:
    // 线程执行函数
    void run();
    // 定时处理
    void timer();
    // 消息分发器
    void handleMsg(const LMsgPtr& msg);

    ///////////////////////////////////////////////////////////////
    // 处理 socket 连接上来
    void handleSocketIn(const std::shared_ptr<LMsgSocketIn>& msg);
    // 处理 socket 断开连接
    void handleSocketOut(const std::shared_ptr<LMsgSocketOut>& msg);
    // 处理后端的心跳包应答
    void handleHeartbeatReply(const std::shared_ptr<LMsgHeartBeatReplyMsg>& msg);
    // 转换客户端发过来的消息
    void handleConverCLientMsg(const std::shared_ptr<LMsgConvertClientMsg>& msg);
    // 处理LogicManager同步的Logic消息，有逻辑服务器连接到逻辑管理上时，逻辑管理会发消息通知网关服务
    void handleLogicInfo(const std::shared_ptr<LMsgLMG2GateLogicInfo>& msg);
    // 处理逻辑管理服务器发来的跑马灯消息
    void handleHorseInfo(const std::shared_ptr<LMsgLMG2GHorseInfo>& msg);
    // 处理逻辑服务器发过来的玩家状态改变消息
    void handleModifyUserState(const std::shared_ptr<LMsgL2GModifyUserState>& msg);

    ///////////////////////////////////////////////////////////////
    // 处理客户端登录网关的消息
    void doClientLoginGate(const std::shared_ptr<LMsgC2SLoginGate>& msg);
    // 处理与客户端之间到心跳消息
    void doClientHeartbeat(const std::shared_ptr<LMsgC2SHeart>& msg);
    // 处理客户端发送其他消息
    void doClientOtherMsg(const LMsgPtr& msg, const BufferPtr& msgOriginData);
    // 验证客户端到消息
    bool isClientMsgSafe(const std::shared_ptr<LMsgC2SVerifyHead>& msg);

    // 检测未验证客户端的连接
    void checkClientConn();
    // 检测验证通过登录成功的客户端
    void checkGateUserState();
    // 检测和发送逻辑管理服务器的心跳
    void checkLManagerHeartbeat();
    // 检测和发送逻辑服务器的心跳
    void checkLServerHeartbeat();

    // 处理玩家离开
    void dealUserOut(const GateUserPtr& user);
    // 发送消息给所有玩家
    void sendMsgToAllUser(const LMsgPtr& msg);
    // 获取逻辑服务器信息
    LogicInfo* getLogicInfoById(int logicId);
    // 某个连接是否为后端的连接
    bool isBackendConn(const TcpConnectionPtr& conn);

private:
    std::unique_ptr<muduo::Thread> BusinessThread_;//业务线程
    Timestamp time_;//时间戳
    BlockingQueue<LMsgPtr> msgQueue_;//消息队列

    ClientNet backend_; //与后端的连接对象，后端包括逻辑管理服务器和N个逻辑服务器
    InetAddress lManagerAddr_; //逻辑管理服务器到地址
    TcpConnectionPtr lManagerConn_; //与逻辑管理的连接器

    std::map<muduo::string, int> logicIpPortList_;//逻辑服务器IP/端口列表,对应逻辑服务器ID
    std::map<int, LogicInfo> logicInfoMap_; //逻辑服务相关信息，key为逻辑服务ID

    std::map<TcpConnectionPtr, Timestamp> unAuthClientInMap_; //记录所有没有被验证的客户端连接
    std::set<TcpConnectionPtr> m_heartBeat_; //连接的心跳数据
};

#define gBusiness muduo::Singleton<Business>::instance()



#endif //PROJECT_BUSINESS_H
