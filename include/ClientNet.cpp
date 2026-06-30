//
// Created by root on 9/11/17.
//

#include "ClientNet.h"


ClientNet::ClientNet(MsgCallback cb)
    : loopThreadPtr_(new EventLoopThread(std::bind(&ClientNet::onThreadInit, this, _1), "ConnBEndThread")),
      loop_(loopThreadPtr_->startLoop()),
      msgCb_(cb),
      decode_(std::bind(&ClientNet::onRecvMsg, this, _1))
{
}

void ClientNet::Connect(const muduo::string& ip, uint16_t port, const muduo::string& obj)
{
    InetAddress connAddr(ip, port);
    TcpClient* client(new TcpClient(loop_, connAddr, obj));
    if (client)
    {
        //LOG_INFO << "Connect to [" << obj << ":" << ip << "/" << port << "]";
        client->setConnectionCallback(std::bind(&ClientNet::onConnect, this, _1));
        client->setMessageCallback(std::bind(&ClientNet::onMessage, this, _1, _2, _3));
        client->enableRetry();
        client->connect();
        clientList_.push_back(std::unique_ptr<TcpClient>(client));
    }
}

void ClientNet::onConnect(const TcpConnectionPtr& conn)
{
    LOG_INFO << conn->peerAddress().toIpPort() << " -> "
             << conn->localAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
    if (conn->connected())
    {
        conn->setTcpNoDelay(true);
        LMsgPtr pMsg(new LMsgSocketIn);
        pMsg->connPtr_ = conn;
        onRecvMsg(pMsg);
    }
}

void ClientNet::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp)
{
    decode_.OnRecv(conn, buf, Decode::RECV_SERVER);
}

void ClientNet::onThreadInit(EventLoop* loop)
{
    LOG_INFO << "ClientNet::onThreadInit loop = " << loop
             << ", tid=" << muduo::CurrentThread::tid();
}

void ClientNet::onRecvMsg(const LMsgPtr& msg)
{
    if (msg)
    {
        msgCb_(msg);
    }
}
