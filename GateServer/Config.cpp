//
// Created by root on 3/23/18.
//

#include "Config.h"

bool Config::Init()
{
    // Try common paths, fall back to defaults
    const char* paths[] = {
        "./config/gateserver_config.ini",
        "config/gateserver_config.ini",
        "../config/gateserver_config.ini",
        nullptr
    };
    for (int i = 0; paths[i]; ++i)
    {
        std::ifstream test(paths[i]);
        if (test.good())
        {
            icfg_.LoadFile(paths[i]);
            return true;
        }
    }
    // If no config file found, use defaults
    LOG_WARN << "GateServer config file not found, using defaults.";
    return true;
}

Lshort Config::GetListenPort()
{
    return icfg_.GetShort("ListenPort", 8115);
}

Lstring	Config::GetGateServerIp()
{
    return icfg_.GetString("GateServerIp", "127.0.0.1");
}

Lint Config::GetGateServerId()
{
    return icfg_.GetInt("GateServerId", 115);
}

Lstring Config::GetGateServerKey()
{
    // 也可以不用配置，只要不为空即可
    return icfg_.GetString("GateServerKey", "123231");
}

Lstring	Config::GetLogicManagerIp()
{
    return icfg_.GetString("LogicManagerIp", "127.0.0.1");
}

Lshort Config::GetLogicManagerPort()
{
    return icfg_.GetShort("LogicManagerPort", 11015);
}
