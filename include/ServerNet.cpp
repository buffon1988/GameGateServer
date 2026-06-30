//
// Created by root on 9/11/17.
//

#include "ServerNet.h"


ServerNet::ServerNet(EventLoop* loop, const InetAddress& listenAddr, MsgCallback cb,
                     int recvFromType, int numThreads)
    : server_(loop, listenAddr, "ServerNet"),
      msgCb_(cb),
      recvFromType_(recvFromType),
      numThreads_(numThreads),
      bHttp_(RECV_FROM_HTTP == recvFromType ? true : false),
      decode_(std::bind(&ServerNet::onRecvMsg, this, _1))
{
    server_.setConnectionCallback(std::bind(&ServerNet::onConnect, this, _1));
    server_.setMessageCallback(std::bind(&ServerNet::onMessage, this, _1, _2, _3));
    server_.setThreadInitCallback(std::bind(&ServerNet::onThreadInit, this, _1));
    if (numThreads_ > 0)
        server_.setThreadNum(numThreads_);
}

void ServerNet::Start()
{
    server_.start();
}

void ServerNet::onConnect(const TcpConnectionPtr& conn)
{
    LOG_INFO << conn->peerAddress().toIpPort() << " -> "
             << conn->localAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
    bool isConnect = conn->connected();
    if (isConnect)
        conn->setTcpNoDelay(true);

    if(!bHttp_)
    {
        if (isConnect)
        {
            LMsgPtr pMsg(new LMsgSocketIn);
            pMsg->connPtr_ = conn;
            onRecvMsg(pMsg);
        }
        else
        {
            LMsgPtr pMsg(new LMsgSocketOut);
            pMsg->connPtr_ = conn;
            onRecvMsg(pMsg);
        }
    }
}

void ServerNet::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp)
{
    if (recvFromType_ == RECV_FROM_CLIENT)
    {
        decode_.OnRecv(conn, buf, Decode::RECV_CLIENT);
    }
    else if (recvFromType_ == RECV_FROM_SERVER)
    {
        decode_.OnRecv(conn, buf, Decode::RECV_SERVER);
    }
    else if (recvFromType_ == RECV_FROM_HTTP)
    {
        decode_.OnRecv(conn, buf, Decode::RECV_HTTP);
    }
}

void ServerNet::onThreadInit(EventLoop* loop)
{
    LOG_INFO << "ServerNet::onThreadInit loop = " << loop
             << ", tid=" << muduo::CurrentThread::tid();
}

void ServerNet::onRecvMsg(const LMsgPtr& msg)
{
    if (msg)
    {
        msgCb_(msg);
    }
}
