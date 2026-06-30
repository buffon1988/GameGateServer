//
// Created by root on 9/11/17.
// 功能：能够主动发起连接到其他服务器，相对其他服务器本服务就是一个客户端，在连接断开时能够主动重连。
//

#ifndef PROJECT_CLIENTNET_H
#define PROJECT_CLIENTNET_H


#include "Decode.h"
#include <muduo/net/TcpClient.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>

using namespace muduo;
using namespace muduo::net;


class ClientNet
{
public:
    explicit ClientNet(MsgCallback cb);
    // 发起连接(异步连接)
    void Connect(const muduo::string& ip, uint16_t port, const muduo::string& obj);

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
    std::unique_ptr<EventLoopThread> loopThreadPtr_;//主动发起连接的IO线程
    EventLoop* loop_;//发起连接的IO线程事件循环
    std::vector<std::unique_ptr<TcpClient>> clientList_;//保存发起连接对象的列表
    MsgCallback msgCb_;//业务层的回调函数
    Decode decode_;//解码层
};


#endif //PROJECT_CLIENTNET_H
