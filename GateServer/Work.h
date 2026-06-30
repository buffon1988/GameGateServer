//
// Created by root on 8/17/17.
// 功能：负责整个服务的初始化工作，启动工作，以及退出时的清理工作
//

#ifndef GAMESERVER_WORK_H
#define GAMESERVER_WORK_H


class Work
{
public:
    Work();
    // 开始启动
    void Start();

private:
    // 初始化工作
    bool init();
    // 清理数据
    void clearData();
};


#endif //GAMESERVER_WORK_H
