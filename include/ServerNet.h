//
// Created by root on 9/11/17.
// 功能：网络IO处理服务器，支持多线程，支持http连接
//

#ifndef PROJECT_SERVERNET_H
#define PROJECT_SERVERNET_H


#include "Decode.h"
#include <muduo/base/Timestamp.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

using namespace muduo;
using namespace muduo::net;


class ServerNet
{
public:
    ServerNet(EventLoop* loop, const InetAddress& listenAddr,
              MsgCallback cb, int recvFromType, int numThreads = 0);

    // 启动服务
    void Start();

private:
    // 连接到达或连接断开的回调处理
    void onConnect(const TcpConnectionPtr& conn);
    // 连接上有数据到达的回调处理
    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp);
    // 线程创建时初始化的回调处理（根据需求可选）
    void onThreadInit(EventLoop* loop);
    // 解码成内部使用的消息格式后的回调处理
    void onRecvMsg(const LMsgPtr& msg);

private:
    TcpServer server_;  //服务器对象
    MsgCallback msgCb_; //业务层的回调函数
    int recvFromType_;  //接收数据来源类型，客户端/服务端/http
    int numThreads_;    //sub IO线程个数
    bool bHttp_;        //标记是否处理http数据
    Decode decode_;     //解码层
};


#endif //PROJECT_SERVERNET_H
