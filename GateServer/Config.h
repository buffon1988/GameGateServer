//
// Created by root on 3/23/18.
// 功能：读取配置文件
//

#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include "IniConfig.h"
#include <muduo/base/Singleton.h>

class Config
{
public:
    bool    Init();

    // 网关服务器监听的端口
    Lshort	GetListenPort();

    // 网关服务器的IP
    Lstring	GetGateServerIp();

    // 网关服务器的ID
    Lint	GetGateServerId();

    // 网关服务器的KEY
    Lstring GetGateServerKey();

    // 逻辑管理服务器的IP
    Lstring	GetLogicManagerIp();

    // 逻辑管理服务器的端口
    Lshort	GetLogicManagerPort();

private:
    IniConfig	icfg_;
};

#define gConfig muduo::Singleton<Config>::instance()

#endif //PROJECT_CONFIG_H
