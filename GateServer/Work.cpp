//
// Created by root on 8/17/17.
//

#include "Work.h"
#include "Business.h"
#include "ServerNet.h"
#include "Config.h"

using namespace muduo::net;

Work::Work()
{
    if (!init())
    {
        LOG_ERROR << "GateServer init failed.";
        clearData();
        exit(-1);
    }
}

bool Work::init()
{
    gLMsgFactory.InitMsg();
    return (gConfig.Init() && gBusiness.Init());
}

void Work::clearData()
{
    gLMsgFactory.ClearMsg();
}

void Work::Start()
{
    gBusiness.Start();

    EventLoop loop;
    uint16_t listenPort = static_cast<uint16_t>(gConfig.GetListenPort());
    LOG_INFO << "GateServer Start, listen port = " << listenPort;
    InetAddress listenAddr(listenPort);
    ServerNet gateServer(&loop, listenAddr, std::bind(&Business::PushMsg, &gBusiness, _1), RECV_FROM_CLIENT, 4);
    gateServer.Start();
    loop.loop();
}
