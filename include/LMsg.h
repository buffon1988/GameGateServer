//
// Created by root on 8/18/17.
//

#ifndef GAMESERVER_LMSG_H
#define GAMESERVER_LMSG_H

#include "CommonDefine.h"
#include "GameDefine.h"
#include "PackUnpack.h"
#include "BufReadWrite.h"
#include "LVideo.h"
#include "LUser.h"

#include <muduo/base/Logging.h>
#include <muduo/net/Buffer.h>
#include <muduo/base/Singleton.h>
#include <muduo/net/TcpConnection.h>

#include <memory>
#include <unordered_map>

using namespace muduo::net;


struct LMsg;
typedef std::shared_ptr<LMsg> LMsgPtr;
typedef std::shared_ptr<Buffer> BufferPtr;

/// 消息工厂
class LMsgFactory
{
public:
    /// 为所有种类消息创建缓冲区
    void InitMsg();

    /// 清空映射表中的所有消息
    void ClearMsg();

    /// 注册消息（消息入队）
    void RegistMsg(const LMsgPtr& pMsg);

    /// 创建消息（msgId在映射中必须存在）
    LMsgPtr CreateMsg(int msgId);

private:
    std::unordered_map<int, LMsgPtr> msgMap_;//保存各种消息的映射
};

#define gLMsgFactory muduo::Singleton<LMsgFactory>::instance()

//////////////////////////////////////////////////////////////////////////
struct LMsg
{
    LMsg(int msgId);
    virtual ~LMsg();

    /// 获取发送缓冲区指针
    BufferPtr GetSendBufferPtr();
    Buffer* GetSendBuffer();

    int GetMsgId() {return m_msgId;}

    virtual void ReadMsg(Buffer& buf){}
    virtual void WriteMsg(Buffer& buf){}

    virtual void ReadPack(msgpack::object& obj){}
    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack){}

    virtual LMsgPtr	Clone() {return nullptr;}

    int				    m_msgId;    //消息id
    bool				bPacked_;   //是否为打包的消息
    TcpConnectionPtr	connPtr_;   //发送该消息者的连接
    BufferPtr			bufPtr_;    //缓冲区指针
};

struct LMsgSC : public LMsg
{
    LMsgSC(int msgId) :LMsg(msgId)
    {
        bPacked_ = true;
    }
};


////////////////////////////// 程序内部生成的消息 //////////////////////////////////
/// 有socket连接上了生成的消息
struct LMsgSocketIn : public LMsg
{
    LMsgSocketIn():LMsg(MSG_SOCKET_IN){}
};

/// 有socket断开连接生成的消息
struct LMsgSocketOut : public LMsg
{
    LMsgSocketOut():LMsg(MSG_SOCKET_OUT){}
};

/// 有HTTP请求生成的消息
struct LMsgHttp : public LMsg
{
    std::string head_;//消息头
    std::string body_;//消息体
    LMsgHttp():LMsg(MSG_HTTP_REQ){}
};

struct LMsgC2SVerifyHead;
/// 客户端数据包经过解析后生成的消息
struct LMsgConvertClientMsg : public LMsg
{
    std::shared_ptr<LMsgC2SVerifyHead> verifyHeadPtr_;
    LMsgPtr	    msgEntityPtr_;
    BufferPtr   msgBufPtr_;
    LMsgConvertClientMsg() : LMsg(MSG_CONVERT_CLIENTMSG){}
};


#endif //GAMESERVER_LMSG_H
