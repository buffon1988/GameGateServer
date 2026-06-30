//
// Created by root on 9/14/17.
//

#ifndef PROJECT_LMSGSS_H
#define PROJECT_LMSGSS_H

#include "LMsg.h"
#include "LMsgCS.h"


///////////////////////////// 各个服务器进程间的消息 ////////////////////////////////
/// 心跳请求包
struct LMsgHeartBeatRequestMsg : public LMsg
{
    HEARBEAT_WHO    m_fromWho;
    int             m_iServerId; //服务器标识

    LMsgHeartBeatRequestMsg() : LMsg(MSG_HEARDBEAT_REQUEST)
    {
        m_fromWho   = HEARBEAT_WHO_UNKNOWN;
        m_iServerId = -1;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_fromWho);
        Read(buf, m_iServerId);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_fromWho);
        Write(buf, m_iServerId);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgHeartBeatRequestMsg>();
    }
};

/// 心跳应答包
struct LMsgHeartBeatReplyMsg : public LMsg
{
    HEARBEAT_WHO    m_fromWho;
    int             m_iServerId; //服务器标识

    LMsgHeartBeatReplyMsg() : LMsg(MSG_HEARDBEAT_REPLY)
    {
        m_fromWho   = HEARBEAT_WHO_UNKNOWN;
        m_iServerId = -1;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        int nFromWho = 0;
        Read(buf, nFromWho);
        Read(buf, m_iServerId);
        m_fromWho = static_cast<HEARBEAT_WHO>(nFromWho);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, static_cast<int>(m_fromWho));
        Write(buf, m_iServerId);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgHeartBeatReplyMsg>();
    }
};

struct LMsgToLoginServer : public LMsg
{
    std::string	 m_strUUID;
    BufferPtr    m_dataBuff;

    LMsgToLoginServer() : LMsg(MSG_TO_LOGINSERVER) {}

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        m_dataBuff.reset(new Buffer);
        m_dataBuff->append(buf.peek(), buf.readableBytes());
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        buf.append(m_dataBuff->peek(), m_dataBuff->readableBytes());
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgToLoginServer>();
    }
};

struct LMsgFromLoginServer : public LMsg
{
    //以下字段由LoginServer填写
    std::string m_strUUID;
    BufferPtr   m_dataBuff;

    //以下字段由read中解析(即解析出m_dataBuff的数据)
    int			m_userMsgId;
    LMsgPtr		m_userMsg;

    LMsgFromLoginServer() : LMsg(MSG_FROM_LOGINSERVER)
    {
        m_userMsgId = 0;
        m_userMsg = NULL;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        m_userMsgId = MSG_ERROR_MSG;
        try
        {
            msgpack::unpacked unpack;
            msgpack::unpack(unpack, buf.peek(), buf.readableBytes());
            msgpack::object obj = unpack.get();
            ReadMapData(obj, "m_msgId", m_userMsgId);
            m_userMsg = gLMsgFactory.CreateMsg(m_userMsgId);
            if (m_userMsg)
            {
                m_userMsg->ReadPack(obj);
            }
            else
            {
                LOG_ERROR << "Msg read msgId not exiest, msgid = " << m_userMsgId;
            }
        }
        catch (...)
        {
            LOG_ERROR << "ReadMsg unpack exception";
        }
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        buf.append(m_dataBuff->peek(), m_dataBuff->readableBytes());
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgFromLoginServer>();
    }
};

//////////////////////////////////////////////////////////////////////////
struct LogicInfo
{
    int		            m_id;
    int		            m_deskCount;
    std::string         m_ip;
    short		        m_port;
    int		            m_flag;	//0,normal; -1,failover
    TcpConnectionPtr	connPtr_;

    LogicInfo()
    {
        m_id = 0;
        m_deskCount = 0;
        m_port = 0;
        m_flag = 0;
        connPtr_ = NULL;
    }
};

struct LMsgLMG2GateLogicInfo : public LMsg
{
    int		    m_ID;
    int		    m_count;
    LogicInfo	m_logic[256];

    LMsgLMG2GateLogicInfo() :LMsg(MSG_LMG_2_G_SYNC_LOGIC)
    {
        m_ID = 0;
        m_count = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_ID);
        Read(buf, m_count);
        for (int i = 0; i < m_count && i < 256; ++i)
        {
            Read(buf, m_logic[i].m_id);
            Read(buf, m_logic[i].m_deskCount);
            Read(buf, m_logic[i].m_ip);
            Read(buf, m_logic[i].m_port);
        }
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_ID);
        Write(buf, m_count);
        for (int i = 0; i < m_count && i < 256; ++i)
        {
            Write(buf, m_logic[i].m_id);
            Write(buf, m_logic[i].m_deskCount);
            Write(buf, m_logic[i].m_ip);
            Write(buf, m_logic[i].m_port);
        }
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2GateLogicInfo>();
    }
};

struct GateInfo
{
    int		            m_id;
    int		            m_userCount;
    std::string		    m_ip;
    short		        m_port;
    TcpConnectionPtr	m_sp;
    std::string		    m_strOldUserIp;
    short		        m_uOldUserPort;
    GateInfo()
    {
        m_id = 0;
        m_userCount = 0;
        m_port = 0;
        m_uOldUserPort = 0;
    }
};

struct LMsgL2CeGateInfo : public LMsg
{
    int		    m_ID;
    int		    m_count;
    GateInfo	m_gate[256];

    LMsgL2CeGateInfo() :LMsg(MSG_L_2_CE_GATE_INFO)
    {
        m_ID = 0;
        m_count = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_ID);
        Read(buf, m_count);
        for (int i = 0; i < m_count && i < 256; ++i)
        {
            Read(buf, m_gate[i].m_id);
            Read(buf, m_gate[i].m_userCount);
            Read(buf, m_gate[i].m_ip);
            Read(buf, m_gate[i].m_port);
            Read(buf, m_gate[i].m_strOldUserIp);
            Read(buf, m_gate[i].m_uOldUserPort);
        }
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_ID);
        Write(buf, m_count);
        for (int i = 0; i < m_count && i < 256; ++i)
        {
            Write(buf, m_gate[i].m_id);
            Write(buf, m_gate[i].m_userCount);
            Write(buf, m_gate[i].m_ip);
            Write(buf, m_gate[i].m_port);
            Write(buf, m_gate[i].m_strOldUserIp);
            Write(buf, m_gate[i].m_uOldUserPort);
        }
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2CeGateInfo>();
    }
};

//gate 发送玩家消息到 LogicServer or LogicManager
struct LMsgG2LUserMsg :public LMsg
{
    // 以下字段由gate进行填充
    std::string		m_strUUID;	//玩家uuid
    std::string		m_ip;
    BufferPtr		m_dataBuff;

    //以下字段由read中解析(即解析出m_dataBuff的数据)
    int			    m_userMsgId;
    LMsgPtr			m_userMsg;

    LMsgG2LUserMsg() :LMsg(MSG_G_2_L_USER_MSG)
    {
        m_userMsgId = 0;
        m_userMsg = NULL;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_ip);

        m_userMsgId = MSG_ERROR_MSG;
        try
        {
            msgpack::unpacked unpack;
            msgpack::unpack(unpack, buf.peek(), buf.readableBytes());
            msgpack::object obj = unpack.get();

            ReadMapData(obj, "m_msgId", m_userMsgId);
            m_userMsg = gLMsgFactory.CreateMsg(m_userMsgId);
            if (m_userMsg)
            {
                m_userMsg->ReadPack(obj);
            }
            else
            {
                LOG_ERROR << "LMsgG2LUserMsg read msgId =" << m_userMsgId << " not exiest ";
            }
        }
        catch (...)
        {
            LOG_ERROR << "MSG_G_2_L_USER_MSG::RecvMsgPack error";
        }
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_ip);
        buf.append(m_dataBuff->peek(), m_dataBuff->readableBytes());
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgG2LUserMsg>();
    }
};


//gate 发送玩家掉线消息
struct LMsgG2LUserOutMsg : public LMsg
{
    std::string	 m_strUUID;		//玩家UUID

    LMsgG2LUserOutMsg():LMsg(MSG_G_2_L_USER_OUT_MSG) {}

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgG2LUserOutMsg>();
    }
};


struct LMsgG2LMGLogin :public LMsg
{
    int		        m_id;
    std::string		m_key;
    std::string		m_ip;
    short		    m_port;
    std::string		m_strOldUserIp;
    short		    m_uOldUserPort;

    LMsgG2LMGLogin() :LMsg(MSG_G_2_LMG_LOGIN)
    {
        m_id = 0;
        m_port = 0;
        m_uOldUserPort = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_id);
        Read(buf, m_key);
        Read(buf, m_ip);
        Read(buf, m_port);
        Read(buf, m_strOldUserIp);
        Read(buf, m_uOldUserPort);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_id);
        Write(buf, m_key);
        Write(buf, m_ip);
        Write(buf, m_port);
        Write(buf, m_strOldUserIp);
        Write(buf, m_uOldUserPort);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgG2LMGLogin>();
    }
};

struct LMsgL2GModifyUserState : public LMsg
{
    std::string m_strUUID;	 //玩家uuid
    int	        m_userstate;
    int	        m_logicID;

    LMsgL2GModifyUserState() :LMsg(MSG_L_2_G_MODIFY_USER_STATE)
    {
        m_userstate = 0;
        m_logicID = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userstate);
        Read(buf, m_logicID);

    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_userstate);
        Write(buf, m_logicID);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2GModifyUserState>();
    }
};

struct LMsgLMG2GHorseInfo : public LMsg
{
    std::string	m_str;
    int         m_type;  //1= 免费收费消息推送 2= 推送金币房卡信息

    LMsgLMG2GHorseInfo():LMsg(MSG_LMG_2_G_HORSE_INFO)
    {
        m_type = 0;   //默认跑马灯 消息模式
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_str);
        Read(buf, m_type);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_str);
        Write(buf, m_type);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2GHorseInfo>();
    }
};

//logic 发现玩家消息到 gate
struct LMsgL2GUserMsg : public LMsg
{
    std::string		m_strUUID;
    BufferPtr		m_dataBuff;

    LMsgL2GUserMsg() :LMsg(MSG_L_2_G_USER_MSG) {}

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        m_dataBuff = BufferPtr(new Buffer);
        m_dataBuff->append(buf.peek(), buf.readableBytes());
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        buf.append(m_dataBuff->peek(), m_dataBuff->readableBytes());
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2GUserMsg>();
    }
};

struct LMsgL2GUserOutMsg : public LMsg
{
    std::string		m_strUUID;	//玩家uuid

    LMsgL2GUserOutMsg():LMsg(MSG_L_2_G_USER_OUT_MSG) {}

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2GUserOutMsg>();
    }
};


struct LMsgL2CeLogin:public LMsg
{
    int	m_ID;//类型
    std::string m_key;
    std::string m_ServerName;
    int	m_needLoadUserInfo;  //center是否需要发送当前所有的玩家的信息 0:不发送 1：发送

    LMsgL2CeLogin() :LMsg(MSG_L_2_CE_LOGIN)
    {
        m_ID = 0;
        m_needLoadUserInfo = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_ID);
        Read(buf, m_key);
        Read(buf, m_ServerName);
        Read(buf, m_needLoadUserInfo);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_ID);
        Write(buf, m_key);
        Write(buf, m_ServerName);
        Write(buf, m_needLoadUserInfo);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2CeLogin>();
    }
};


struct LMsgCe2LUserLogin:public LMsg
{
    int		m_seed;
    LUser	user;

    LMsgCe2LUserLogin() :LMsg(MSG_CE_2_L_USER_LOGIN)
    {
        m_seed = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_seed);
        user.ReadData(buf);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_seed);
        user.WriteData(buf);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCe2LUserLogin>();
    }
};


//center发送玩家id信息到
//玩家基本信息
struct UserIdInfo
{
    int		m_id;
    std::string		m_openId;
    std::string		m_nike;
    std::string		m_headImageUrl;
    int		m_sex;

    UserIdInfo()
    {
        m_id = 0;
        m_sex = 1;
    }
};

struct LMsgCe2LUserIdInfo:public LMsg
{
    int	m_count;
    std::vector<UserIdInfo> m_info;
    int	m_hasSentAll; //已经发送所有了的id。 0：没有 1：已经发送所有

    LMsgCe2LUserIdInfo() :LMsg(MSG_CE_2_L_USER_ID_INFO)
    {
        m_count = 0;
        m_hasSentAll = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_count);
        for(int i = 0 ; i < m_count; ++i)
        {
            UserIdInfo info;
            Read(buf, info.m_id);
            Read(buf, info.m_openId);
            Read(buf, info.m_nike);
            Read(buf, info.m_sex);
            Read(buf, info.m_headImageUrl);
            m_info.push_back(info);
        }
        Read(buf, m_hasSentAll);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        m_count = static_cast<int>(m_info.size());
        Write(buf, m_count);
        for(int i = 0 ; i < m_count; ++i)
        {
            Write(buf, m_info[i].m_id);
            Write(buf, m_info[i].m_openId);
            Write(buf, m_info[i].m_nike);
            Write(buf, m_info[i].m_sex);
            Write(buf, m_info[i].m_headImageUrl);
        }
        Write(buf, m_hasSentAll);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCe2LUserIdInfo>();
    }

    virtual int preAllocSendMemorySize() //重新分配64K,由于消息长度是short型
    {
        return 1024 * 64;
    }
};


//logic 发送保存 玩家信息
struct LMsgL2CeModifyUserNew:public LMsg
{
    std::string	m_strUUID;
    int			m_value;	// 0表示新生成的用户 1表示老用户并给玩家送过卡
    LMsgL2CeModifyUserNew() :LMsg(MSG_L_2_CE_MODIFY_USER_NEW)
    {
        m_value = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_value);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_value);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2CeModifyUserNew>();
    }
};


//////////////////////////////////////////////////////////////////////////
//logic 发送保存 桌子信息
struct LMsgL2CeSaveLogItem:public LMsg
{
    int		m_type;//类型,0-插入,1-更新
    std::string		m_sql;

    LMsgL2CeSaveLogItem() :LMsg(MSG_L_2_CE_ROOM_SAVE)
    {
        m_type = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_type);
        Read(buf, m_sql);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_type);
        Write(buf, m_sql);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2CeSaveLogItem>();
    }
};

//////////////////////////////////////////////////////////////////////////
//logicM 通知中心服务器玩家登陆服务器
struct LMsgL2CeUserServerLogin :public LMsg
{
    int		        m_serverID;//逻辑管理服务ID
    std::string		m_openID;

    LMsgL2CeUserServerLogin() :LMsg(MSG_L_2_CE_USER_LOGIN)
    {
        m_serverID = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_serverID);
        Read(buf, m_openID);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_serverID);
        Write(buf, m_openID);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2CeUserServerLogin>();
    }
};

//////////////////////////////////////////////////////////////////////////
//logic 通知玩家登出服务器
struct LMsgL2CeUserServerLogout:public LMsg
{
    int		m_serverID;//类型,0-插入,1-更新
    std::string		m_openID;

    LMsgL2CeUserServerLogout() :LMsg(MSG_L_2_CE_USER_LOGOUT)
    {
        m_serverID = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_serverID);
        Read(buf, m_openID);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_serverID);
        Write(buf, m_openID);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2CeUserServerLogout>();
    }
};

//////////////////////////////////////////////////////////////////////////
//center 发送玩家充值
struct LMsgCe2LGMCharge:public LMsg
{
    std::string		m_strUUID;
    int		m_userid;
    int		m_cardType;
    int		m_cardCount;
    int		m_oper;
    std::string		m_admin;

    LMsgCe2LGMCharge() :LMsg(MSG_CE_2_L_GM_CHARGE)
    {
        m_userid = 0;
        m_cardType = 0;
        m_cardCount = 0;
        m_oper = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userid);
        Read(buf, m_cardType);
        Read(buf, m_cardCount);
        Read(buf, m_oper);
        Read(buf, m_admin);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_userid);
        Write(buf, m_cardType);
        Write(buf, m_cardCount);
        Write(buf, m_oper);
        Write(buf, m_admin);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCe2LGMCharge>();
    }
};


//////////////////////////////////////////////////////////////////////////
//center 发送gm设置跑马灯
struct LMsgCe2LGMHorse :public LMsg
{
    std::string		m_str;

    LMsgCe2LGMHorse() :LMsg(MSG_CE_2_L_GM_HORSE)
    {
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_str);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_str);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCe2LGMHorse>();
    }
};

//////////////////////////////////////////////////////////////////////////
//center 发送gm够买提示
struct LMsgCe2LGMBuyInfo:public LMsg
{
    std::string		m_str;

    LMsgCe2LGMBuyInfo() :LMsg(MSG_CE_2_L_GM_BUYINFO)
    {
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_str);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_str);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCe2LGMBuyInfo>();
    }
};

struct LMsgCe2LGMCoins:public LMsg
{
    std::string		m_strUUID;
    int		m_userid;
    int		m_coins;		// 充值的金币
    int		m_totalcoins;	// 充值后的总金币

    LMsgCe2LGMCoins() :LMsg(MSG_CE_2_L_GM_COINS)
    {
        m_userid = 0;
        m_coins = 0;
        m_totalcoins = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userid);
        Read(buf, m_coins);
        Read(buf, m_totalcoins);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_userid);
        Write(buf, m_coins);
        Write(buf, m_totalcoins);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCe2LGMCoins>();
    }
};

//////////////////////////////////////////////////////////////////////////
//center 发送gm够买提示
struct LMsgCe2LGMHide:public LMsg
{
    int		m_hide;

    LMsgCe2LGMHide() :LMsg(MSG_CE_2_L_GM_HIDE)
    {
        m_hide = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_hide);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_hide);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCe2LGMHide>();
    }
};
//////////////////////////////////////////////////////////////////////////
//logic 跟 gate 之间的消息交互
//////////////////////////////////////////////////////////////////////////

struct LMsgG2LLogin :public LMsg
{
    int		        m_id;
    std::string		m_key;
    std::string		m_ip;
    short		    m_port;
    std::string     m_strOldUserIp;
    short           m_uOldUserPort;

    LMsgG2LLogin() :LMsg(MSG_G_2_L_LOGIN)
    {
        m_id = 0;
        m_port = 0;
        m_uOldUserPort = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_id);
        Read(buf, m_key);
        Read(buf, m_ip);
        Read(buf, m_port);
        Read(buf, m_strOldUserIp);
        Read(buf, m_uOldUserPort);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_id);
        Write(buf, m_key);
        Write(buf, m_ip);
        Write(buf, m_port);
        Write(buf, m_strOldUserIp);
        Write(buf, m_uOldUserPort);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgG2LLogin>();
    }
};


struct LMsgL2LMGLogin :public LMsg
{
    int		m_id;
    std::string		m_key;
    std::string		m_ip;
    short		m_port;

    LMsgL2LMGLogin() :LMsg(MSG_L_2_LMG_LOGIN)
    {
        m_id = 0;
        m_port = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_id);
        Read(buf, m_key);
        Read(buf, m_ip);
        Read(buf, m_port);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_id);
        Write(buf, m_key);
        Write(buf, m_ip);
        Write(buf, m_port);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2LMGLogin>();
    }
};

struct LMsgLMG2LRLogInfo :public LMsg
{
    int m_managerID;
    bool bopenrlog;
    std::string m_url;
    size_t m_max_cached_size;
    size_t m_max_cached_num;

    LMsgLMG2LRLogInfo() :LMsg(MSG_LMG_2_L_RLOG_INFO)
    {
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_managerID);
        Read(buf, bopenrlog);
        Read(buf, m_url);
        Read(buf, m_max_cached_size);
        Read(buf, m_max_cached_num);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_managerID);
        Write(buf, bopenrlog);
        Write(buf, m_url);
        Write(buf, m_max_cached_size);
        Write(buf, m_max_cached_num);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2LRLogInfo>();
    }
};


//logicmanager 跟 logicdb 之间的消息交互
struct LMsgLMG2LdbUserLogin:public LMsg
{
    int			m_userId;
    std::string m_strUUID;

    LMsgLMG2LdbUserLogin() :LMsg(MSG_LMG_2_LDB_USER_LOGIN)
    {
        m_userId = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_userId);
        Read(buf, m_strUUID);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_userId);
        Write(buf, m_strUUID);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2LdbUserLogin>();
    }
};

struct LMsgLDB2LMGUserMsg : public LMsg
{
    std::string		m_strUUID;
    BufferPtr		m_dataBuff;

    LMsgLDB2LMGUserMsg() :LMsg(MSG_LDB_2_LMG_USER_MSG)
    {
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        m_dataBuff = BufferPtr(new Buffer);
        m_dataBuff->append(buf.peek(), buf.readableBytes());
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        buf.append(m_dataBuff->peek(), m_dataBuff->readableBytes());
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLDB2LMGUserMsg>();
    }
};

//logic 登录 logicdb
struct LMsgLMG2LdbLogin : public LMsg
{
    std::string	m_key;
    int			m_serverID;

    LMsgLMG2LdbLogin():LMsg(MSG_LMG_2_LDB_LOGIN)
    {
        m_serverID = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_key);
        Read(buf, m_serverID);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_key);
        Write(buf, m_serverID);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2LdbLogin>();
    }
};

//////////////////////////////////////////////////////////////////////////
//logic 登录 logicdb
struct LMsgL2LdbLogin : public LMsg
{
    std::string	m_key;
    int			m_serverID;

    LMsgL2LdbLogin():LMsg(MSG_L_2_LDB_LOGIN)
    {
        m_serverID = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_key);
        Read(buf, m_serverID);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_key);
        Write(buf, m_serverID);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2LdbLogin>();
    }
};

//////////////////////////////////////////////////////////////////////////
//logic 发送保存 桌子信息
struct LMsgL2LDBSaveLogItem:public LMsg
{
    int		m_type;//类型,0-插入,1-更新
    std::string		m_sql;
    int		m_serverID;
    int     m_userId;  //add

    LMsgL2LDBSaveLogItem() :LMsg(MSG_L_2_LDB_ROOM_SAVE)
    {
        m_type = 0;
        m_serverID = 0;
        m_userId = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_type);
        Read(buf, m_sql);
        Read(buf, m_serverID);
        Read(buf, m_userId);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_type);
        Write(buf, m_sql);
        Write(buf, m_serverID);
        Write(buf, m_userId);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2LDBSaveLogItem>();
    }
};


//////////////////////////////////////////////////////////////////////////
//logic 发送保存录像
struct LMsgL2LDBSaveVideo :public LMsg
{
    int		m_type;//类型,0-插入,1-更新
    std::string		m_sql;
    int		m_serverID;
    int     m_userId;  //add

    LMsgL2LDBSaveVideo() :LMsg(MSG_L_2_LDB_VIDEO_SAVE)
    {
        m_type = 0;
        m_serverID = 0;
        m_userId = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_type);
        Read(buf, m_sql);
        Read(buf, m_serverID);
        Read(buf, m_userId);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_type);
        Write(buf, m_sql);
        Write(buf, m_serverID);
        Write(buf, m_userId);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2LDBSaveVideo>();
    }
};

//////////////////////////////////////////////////////////////////////////
//logic 发送玩家请求战绩
struct LMsgL2LBDReqVipLog:public LMsg
{
    std::string m_strUUID;
    int	m_userId;
    int	m_reqUserId;
    int m_time;

    LMsgL2LBDReqVipLog() :LMsg(MSG_L_2_LDB_VIP_LOG)
    {
        m_userId = 0;
        m_reqUserId = 0;
        m_time = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userId);
        Read(buf, m_reqUserId);
        Read(buf, m_time);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_userId);
        Write(buf, m_reqUserId);
        Write(buf, m_time);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2LBDReqVipLog>();
    }
};


//////////////////////////////////////////////////////////////////////////
//logicdb 返回 玩家战绩
struct LogInfo
{
    std::string	m_id;		//id
    int			m_time;		//时间
    int			m_flag;		//房间规则
    int			m_deskId;	//桌子id
    std::string	m_secret;	//密码
    int			m_maxCircle;//总圈数
    int			m_curCircle;//当前圈数
    int			m_posUserId[4];//0-3各个位置上的玩家id
    int			m_curZhuangPos;//当前庄家
    int			m_score[4];// 各个位置上面的积分
    int			m_reset;//是否解算
    std::string	m_data;
    std::string	m_playtype;	//玩法
    Lint		m_nHouseOwnerId = 0; //房主ID;

    LogInfo()
    {
        m_time = 0;
        m_flag = 0;
        m_deskId = 0;
        m_maxCircle = 0;
        m_curCircle = 0;
        memset(m_posUserId, 0, sizeof(m_posUserId));
        m_curZhuangPos = 0;
        memset(m_score, 0, sizeof(m_score));
        m_reset = false;
    }
};

struct LMsgLBD2LReqVipLog:public LMsg
{
    std::string		m_strUUID;
    int		m_userid;
    int		m_count;
    LogInfo		m_info[21];

    LMsgLBD2LReqVipLog() :LMsg(MSG_LDB_2_L_VIP_LOG)
    {
        m_userid = 0;
        m_count = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userid);
        Read(buf, m_count);
        for(int i = 0 ; i < m_count; ++i)
        {
            LogInfo& info = m_info[i];
            Read(buf, info.m_id);
            Read(buf, info.m_time);
            Read(buf, info.m_flag);
            Read(buf, info.m_deskId);
            Read(buf, info.m_secret);
            Read(buf, info.m_maxCircle);
            Read(buf, info.m_curCircle);
            Read(buf, info.m_posUserId[0]);
            Read(buf, info.m_posUserId[1]);
            Read(buf, info.m_posUserId[2]);
            Read(buf, info.m_posUserId[3]);

            Read(buf, info.m_curZhuangPos);
            Read(buf, info.m_score[0]);
            Read(buf, info.m_score[1]);
            Read(buf, info.m_score[2]);
            Read(buf, info.m_score[3]);
            Read(buf, info.m_reset);
            Read(buf, info.m_data);
            Read(buf, info.m_playtype);
            Read(buf, info.m_nHouseOwnerId);
        }

    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_userid);
        Write(buf, m_count);
        for(int i = 0 ; i < m_count; ++i)
        {
            LogInfo& info = m_info[i];
            Write(buf, info.m_id);
            Write(buf, info.m_time);
            Write(buf, info.m_flag);
            Write(buf, info.m_deskId);
            Write(buf, info.m_secret);
            Write(buf, info.m_maxCircle);
            Write(buf, info.m_curCircle);
            Write(buf, info.m_posUserId[0]);
            Write(buf, info.m_posUserId[1]);
            Write(buf, info.m_posUserId[2]);
            Write(buf, info.m_posUserId[3]);

            Write(buf, info.m_curZhuangPos);
            Write(buf, info.m_score[0]);
            Write(buf, info.m_score[1]);
            Write(buf, info.m_score[2]);
            Write(buf, info.m_score[3]);
            Write(buf, info.m_reset);
            Write(buf, info.m_data);
            Write(buf, info.m_playtype);
            Write(buf, info.m_nHouseOwnerId);
        }
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLBD2LReqVipLog>();
    }
};

//LogicDB 2 LogicManager
struct LMsgLBD2LReqRoomLog:public LMsg
{
    std::string		m_strUUID;
    int		        m_userid;
    int		        m_flag;
    int		        m_size;
    VipLog		    m_item[32];

    LMsgLBD2LReqRoomLog() :LMsg(MSG_LDB_2_L_ROOM_LOG)
    {
        m_userid = 0;
        m_flag = 0;
        m_size = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userid);
        Read(buf, m_flag);
        Read(buf, m_size);
        for (Lint i = 0; i < m_size; ++i)
        {
            VipLog& vipLog = m_item[i];
            Read(buf, vipLog.m_time);
            Read(buf, vipLog.m_videoId);
            for (Lint j = 0; j < 4; ++j)
            {
                Read(buf, vipLog.m_score[j]);
            }
        }
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_userid);
        Write(buf, m_flag);
        Write(buf, m_size);
        for (Lint i = 0; i < m_size; ++i)
        {
            VipLog& vipLog = m_item[i];
            Write(buf, vipLog.m_time);
            Write(buf, vipLog.m_videoId);
            for (Lint j = 0; j < 4; ++j)
            {
                Write(buf, vipLog.m_score[j]);
            }
        }
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLBD2LReqRoomLog>();
    }
};

//////////////////////////////////////////////////////////////////////////
//logic 发送玩家请求录像
struct LMsgL2LDBReqVideo:public LMsg
{
    std::string m_strUUID;
    int	m_userId;
    std::string	m_videoId;

    LMsgL2LDBReqVideo() :LMsg(MSG_L_2_LDB_VIDEO)
    {
        m_userId = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userId);
        Read(buf, m_videoId);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_userId);
        Write(buf, m_videoId);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2LDBReqVideo>();
    }
};

//////////////////////////////////////////////////////////////////////////
//center 返回 玩家战绩
struct LMsgLDB2LReqVideo:public LMsg
{
    std::string		m_strUUID;
    int		m_userid;
    int		m_count;
    VideoLog	m_video;

    LMsgLDB2LReqVideo() :LMsg(MSG_LDB_2_L_VIDEO)
    {
        m_userid = 0;
        m_count = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userid);
        Read(buf, m_count);
        m_video.ReadData(buf);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_userid);
        Write(buf, m_count);
        m_video.WriteData(buf);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLDB2LReqVideo>();
    }
};

struct LMsgLDB2LLogInfo:public LMsg
{
    int	m_lastId;
    int	m_videoId;
    int	m_count;
    std::vector<LogInfo> m_info;

    LMsgLDB2LLogInfo() :LMsg(MSG_LDB_2_L_LOG_INFO)
    {
        m_lastId = 0;
        m_videoId = 0;
        m_count = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_lastId);
        Read(buf, m_videoId);
        Read(buf, m_count);
        for(int i = 0 ; i < m_count; ++i)
        {
            LogInfo info;
            Read(buf, info.m_id);
            Read(buf, info.m_time);
            Read(buf, info.m_flag);
            Read(buf, info.m_deskId);
            Read(buf, info.m_secret);
            Read(buf, info.m_maxCircle);
            Read(buf, info.m_curCircle);
            Read(buf, info.m_posUserId[0]);
            Read(buf, info.m_posUserId[1]);
            Read(buf, info.m_posUserId[2]);
            Read(buf, info.m_posUserId[3]);

            Read(buf, info.m_curZhuangPos);
            Read(buf, info.m_score[0]);
            Read(buf, info.m_score[1]);
            Read(buf, info.m_score[2]);
            Read(buf, info.m_score[3]);
            Read(buf, info.m_reset);
            Read(buf, info.m_data);
            Read(buf, info.m_playtype);

            m_info.push_back(info);
        }
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_lastId);
        Write(buf, m_videoId);
        m_count = static_cast<int>(m_info.size());
        Write(buf, m_count);
        for(int i = 0 ; i < m_count; ++i)
        {
            LogInfo& info = m_info[i];
            Write(buf, info.m_id);
            Write(buf, info.m_time);
            Write(buf, info.m_flag);
            Write(buf, info.m_deskId);
            Write(buf, info.m_secret);
            Write(buf, info.m_maxCircle);
            Write(buf, info.m_curCircle);
            Write(buf, info.m_posUserId[0]);
            Write(buf, info.m_posUserId[1]);
            Write(buf, info.m_posUserId[2]);
            Write(buf, info.m_posUserId[3]);

            Write(buf, info.m_curZhuangPos);
            Write(buf, info.m_score[0]);
            Write(buf, info.m_score[1]);
            Write(buf, info.m_score[2]);
            Write(buf, info.m_score[3]);
            Write(buf, info.m_reset);
            Write(buf, info.m_data);
            Write(buf, info.m_playtype);
        }
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLDB2LLogInfo>();
    }
};


struct LMsgLMG2LLogin:public LMsg
{
    int		m_userid;
    int		m_gateId;
    std::string		m_strUUID;
    std::string		m_ip;
    std::string		m_buyInfo;//够买信息
    int		m_hide;//购买隐藏0-隐藏 1- 不隐藏
    int		m_card_num;	//房卡数 （Card2）

    LMsgLMG2LLogin() :LMsg(MSG_LMG_2_L_USER_LOGIN)
    {
        m_userid = 0;
        m_gateId = 0;
        m_hide = 0;
        m_card_num = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_userid);
        Read(buf, m_gateId);
        Read(buf, m_strUUID);
        Read(buf, m_ip);
        Read(buf, m_buyInfo);
        Read(buf, m_hide);
        Read(buf, m_card_num);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_userid);
        Write(buf, m_gateId);
        Write(buf, m_strUUID);
        Write(buf, m_ip);
        Write(buf, m_buyInfo);
        Write(buf, m_hide);
        Write(buf, m_card_num);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2LLogin>();
    }
};

struct LMsgL2LMGModifyUserState:public LMsg
{
    std::string m_strUUID;
    int	m_userid;
    int	m_userstate;
    int	m_logicID;

    LMsgL2LMGModifyUserState() :LMsg(MSG_L_2_LMG_MODIFY_USER_STATE)
    {
        m_userid = 0;
        m_userstate = 0;
        m_logicID = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userid);
        Read(buf, m_userstate);
        Read(buf, m_logicID);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_userid);
        Write(buf, m_userstate);
        Write(buf, m_logicID);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2LMGModifyUserState>();
    }
};




struct LMsgLMG2LCreateDesk : public LMsg
{
    int				m_userid;
    int				m_gateId;
    std::string		m_strUUID;
    int				m_deskID;
    std::string		m_ip;
    LUser			m_usert;
    int				m_flag;				//房间类型，1-2圈，2-四圈，3-8圈
    int				m_payment;			//付费方式： 1 房主付费，2 平均付费， 3 大赢家付费
    int				m_nIPAntiCheat;		//IP防作弊
    int				m_nGPSEarlyWarning;	//GPS预警
    std::string		m_GPSLat;			//GPS经度
    std::string		m_GPSLng;			//GPS纬度
    std::string		m_secret;			//房间密码，如果为空，服务器会随机一个密码
    int				m_gold;				//房间抵住，单位元，最低为1元，最多不能超过100
    int				m_state;			//玩法规则 0 转转   1 长沙
    int				m_robotNum;			// 0,不加机器人，1，2，3加机器人数量
    CardValue		m_cardValue[CARD_CONFIG];
    int				m_playTypeCount;
    std::vector<int> m_playType;			//玩法

    LMsgLMG2LCreateDesk() :LMsg(MSG_LMG_2_L_CREATE_DESK)
    {
        m_userid = 0;
        m_gateId = 0;
        m_deskID = 0;
        m_flag = 0;
        m_gold = 0;
        m_state = 0;
        m_robotNum = 0;
        m_playTypeCount = 0;
        m_payment = 0;
        m_nGPSEarlyWarning = 0;
        m_nIPAntiCheat = 0;
    }
    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_userid);
        Read(buf, m_gateId);
        Read(buf, m_strUUID);
        Read(buf, m_deskID);
        Read(buf, m_ip);
        m_usert.ReadData(buf);
        Read(buf, m_flag);

        Read(buf, m_gold);
        Read(buf, m_state);
        Read(buf, m_robotNum);
        for (int i = 0; i < CARD_CONFIG; ++i)
        {
            Read(buf, m_cardValue[i].m_color);
            Read(buf, m_cardValue[i].m_number);
        }
        Read(buf, m_playTypeCount);
        for (int i = 0 ; i < m_playTypeCount; i ++ )
        {
            int playType;
            Read(buf, playType);
            m_playType.push_back(playType);
        }
        Read(buf, m_payment);
        Read(buf, m_nIPAntiCheat);
        Read(buf, m_nGPSEarlyWarning);
        Read(buf, m_GPSLat);
        Read(buf, m_GPSLng);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_userid);
        Write(buf, m_gateId);
        Write(buf, m_strUUID);
        Write(buf, m_deskID);
        Write(buf, m_ip);
        m_usert.WriteData(buf);
        Write(buf, m_flag);
        Write(buf, m_gold);
        Write(buf, m_state);
        Write(buf, m_robotNum);
        for (int i = 0; i < CARD_CONFIG; ++i)
        {
            Write(buf, m_cardValue[i].m_color);
            Write(buf, m_cardValue[i].m_number);
        }
        Write(buf, m_playTypeCount);
        for (int i = 0 ; i < m_playTypeCount; i ++ )
        {
            Write(buf, m_playType[i]);
        }
        Write(buf, m_payment);
        Write(buf, m_nIPAntiCheat);
        Write(buf, m_nGPSEarlyWarning);
        Write(buf, m_GPSLat);
        Write(buf, m_GPSLng);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2LCreateDesk>();
    }
};

struct LMsgLMG2LAddToDesk : public LMsg
{
    int			m_userid;
    int			m_gateId;
    std::string			m_strUUID;
    int			m_deskID;
    std::string			m_ip;
    std::string			m_GPSLat;			//GPS经度
    std::string			m_GPSLng;			//GPS纬度
    LUser			m_usert;

    LMsgLMG2LAddToDesk() :LMsg(MSG_LMG_2_L_ADDTO_DESK)
    {
        m_userid = 0;
        m_gateId = 0;
        m_deskID = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_userid);
        Read(buf, m_gateId);
        Read(buf, m_strUUID);
        Read(buf, m_deskID);
        Read(buf, m_ip);
        Read(buf, m_GPSLat);
        Read(buf, m_GPSLng);
        m_usert.ReadData(buf);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_userid);
        Write(buf, m_gateId);
        Write(buf, m_strUUID);
        Write(buf, m_deskID);
        Write(buf, m_ip);
        Write(buf, m_GPSLat);
        Write(buf, m_GPSLng);
        m_usert.WriteData(buf);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2LAddToDesk>();
    }
};


struct LMsgL2LMGModifyCard :public LMsg
{
    int			m_userid;
    std::string	m_strUUID;
    int			isAddCard;
    int			cardType;
    int			cardNum;
    int			operType;	// 参考 CARDS_OPER_TYPE
    std::string	admin;
    int		    serverID;
    int		    gameType;

    LMsgL2LMGModifyCard() :LMsg(MSG_L_2_LMG_MODIFY_USER_CARD)
    {
        m_userid = 0;
        isAddCard = 0;
        cardType = 0;
        cardNum = 0;
        operType = 0;
        serverID = 0;
        gameType = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_userid);
        Read(buf, m_strUUID);
        Read(buf, isAddCard);
        Read(buf, cardType);
        Read(buf, cardNum);
        Read(buf, operType);
        Read(buf, admin);
        Read(buf, serverID);
        Read(buf, gameType);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_userid);
        Write(buf, m_strUUID);
        Write(buf, isAddCard);
        Write(buf, cardType);
        Write(buf, cardNum);
        Write(buf, operType);
        Write(buf, admin);
        Write(buf, serverID);
        Write(buf, gameType);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2LMGModifyCard>();
    }
};


struct LMsgL2LMGAddUserPlayCount :public LMsg
{
    int			m_userid;
    std::string			m_strUUID;

    LMsgL2LMGAddUserPlayCount() :LMsg(MSG_L_2_LMG_ADD_USER_PLAYCOUNT)
    {
        m_userid = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_userid);
        Read(buf, m_strUUID);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_userid);
        Write(buf, m_strUUID);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2LMGAddUserPlayCount>();
    }
};

struct PXActiveItem
{
    int m_iPXId;		//牌型ID
    int m_iCardNum;		//卡片数量
    PXActiveItem()
    {
        m_iPXId = -1;
        m_iCardNum = 0;
    }

    PXActiveItem(int id, int cards)
    {
        m_iPXId = id;
        m_iCardNum = cards;
    }
};

struct PXActive
{
    int		m_iGameId;	//游戏ID
    std::string		m_strBeginTime;	//开始时间
    std::string		m_strEndTime;		//结束时间
    int		m_iRewardCount;		//奖品的数量
    std::vector<PXActiveItem> m_vecPXAndReward;	//牌型和奖品

    PXActive()
    {
        m_iGameId = -1;
        m_strBeginTime = "";
        m_strEndTime = "";
        m_iRewardCount = 0;
    }
};

// LMtoL 发送牌型活动信息
struct LMsgLMG2LPXActive :public LMsg
{
    int     m_iGameSetCount;
    std::vector<PXActive> m_vecPXActive;		//保存所有牌型活动信息

    LMsgLMG2LPXActive() :LMsg(MSG_LMG_2_L_PAIXING_ACTIVE)
    {
        m_iGameSetCount = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_iGameSetCount);
        for(int i = 0 ; i < m_iGameSetCount; ++i)
        {
            PXActive Active;
            Read(buf, Active.m_iGameId);
            Read(buf, Active.m_strBeginTime);
            Read(buf, Active.m_strEndTime);
            Read(buf, Active.m_iRewardCount);
            for(int j = 0; j < Active.m_iRewardCount; ++j)
            {
                PXActiveItem item;
                Read(buf, item.m_iPXId);
                Read(buf, item.m_iCardNum);
                Active.m_vecPXAndReward.emplace_back(item);
            }
            m_vecPXActive.emplace_back(Active);
        }
    }

    virtual void WriteMsg(Buffer& buf)
    {
        m_iGameSetCount = static_cast<int>(m_vecPXActive.size());
        Write(buf, m_iGameSetCount);
        for(int i = 0 ; i < m_iGameSetCount; ++i)
        {
            Write(buf, m_vecPXActive[i].m_iGameId);
            Write(buf, m_vecPXActive[i].m_strBeginTime);
            Write(buf, m_vecPXActive[i].m_strEndTime);
            m_vecPXActive[i].m_iRewardCount = static_cast<int>(m_vecPXActive[i].m_vecPXAndReward.size());
            Write(buf, m_vecPXActive[i].m_iRewardCount);
            std::vector<PXActiveItem>& vec = m_vecPXActive[i].m_vecPXAndReward;
            for(int j = 0; j < m_vecPXActive[i].m_iRewardCount; ++j)
            {
                Write(buf, vec[j].m_iPXId);
                Write(buf, vec[j].m_iCardNum);
            }
        }
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2LPXActive>();
    }
};

struct LMsgL2LMGRecyleDeskID :public LMsg
{
    int			m_serverID;
    int			m_deskID;

    LMsgL2LMGRecyleDeskID() :LMsg(MSG_L_2_LMG_RECYLE_DESKID)
    {
        m_serverID = 0;
        m_deskID = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_serverID);
        Read(buf, m_deskID);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_serverID);
        Write(buf, m_deskID);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2LMGRecyleDeskID>();
    }
};

//logic 通知活动免费
struct LMsgCE2LSetGameFree:public LMsg
{
    int		m_ServerID;
    std::string		m_strFreeSet;
    LMsgCE2LSetGameFree() :LMsg(MSG_CE_2_L_SET_GAME_FREE)
    {
        m_ServerID = 0;
        m_strFreeSet = "";
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_ServerID);
        Read(buf, m_strFreeSet);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_ServerID);
        Write(buf, m_strFreeSet);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCE2LSetGameFree>();
    }
};

//logic 通知设置牌型活动配置
struct LMsgCE2LSetPXActive:public LMsg
{
    int	m_ServerID;
    std::string	m_strActiveSet;

    LMsgCE2LSetPXActive() :LMsg(MSG_CE_2_L_SET_PXACTIVE)
    {
        m_ServerID = 0;
        m_strActiveSet = "";
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_ServerID);
        Read(buf, m_strActiveSet);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_ServerID);
        Write(buf, m_strActiveSet);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCE2LSetPXActive>();
    }
};

//logic 通知设置老玩家送卡活动配置
struct LMsgCE2LSetOUGCActive:public LMsg
{
    int	m_ServerID;
    std::string	m_strActiveSet;

    LMsgCE2LSetOUGCActive() :LMsg(MSG_CE_2_L_SET_OUGCACTIVE)
    {
        m_ServerID = 0;
        m_strActiveSet = "";
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_ServerID);
        Read(buf, m_strActiveSet);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_ServerID);
        Write(buf, m_strActiveSet);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCE2LSetOUGCActive>();
    }
};

//logic 通知设置老玩家送卡活动配置
struct LMsgCE2LSetExchActive:public LMsg
{
    int	m_ServerID;
    std::string	m_strActiveSet;

    LMsgCE2LSetExchActive() :LMsg(MSG_CE_2_L_SET_EXCHACTIVE)
    {
        m_ServerID = 0;
        m_strActiveSet = "";
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_ServerID);
        Read(buf, m_strActiveSet);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_ServerID);
        Write(buf, m_strActiveSet);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCE2LSetExchActive>();
    }
};

// 和后的ID对应
enum ActivityID
{
    ActivityID_Draw	 = 1002,	// 新装盘
    ActivityID_Share = 1003,	// 分享
};

// 设置活懂内容
struct LMsgCE2LSetActivity:public LMsg
{
    int	m_ServerID;
    ActivityID m_activityId;
    std::string	m_strActivity;

    LMsgCE2LSetActivity() :LMsg(MSG_CE_2_L_SET_ACTIVITY)
    {
        m_ServerID = 0;
        m_strActivity = "";
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_ServerID);
        Read(buf, m_activityId);
        Read(buf, m_strActivity);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_ServerID);
        Write(buf, m_activityId);
        Write(buf, m_strActivity);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCE2LSetActivity>();
    }
};

//inviter
struct LMsgLM_2_LDBBindInviter : public LMsgSC
{
    int				m_inviterId;
    int             m_userId;

    LMsgLM_2_LDBBindInviter() : LMsgSC(MSG_LM_2_LDB_BIND_INVITER)
    {
        m_inviterId = 0;
        m_userId = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_inviterId), m_inviterId);
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_inviterId), m_inviterId);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgLM_2_LDBBindInviter>(); }
};

struct LMsgLDB2LMGBindingRelationships : public LMsg
{
#define    NUM_MAX_INVITEE   40
    std::string		m_strUUID;
    int		m_userId;
    int		m_inviterId;
    int		m_finished;
    int		m_inviteeCount;
    int		m_invitees[NUM_MAX_INVITEE];
    int		m_taskFinished[NUM_MAX_INVITEE];

    LMsgLDB2LMGBindingRelationships() : LMsg(MSG_LDB_2_LMG_REQ_BINDING_RELATIONSHIPS)
    {
        m_userId = 0;
        m_inviterId = 0;
        m_finished = 0;
        m_inviteeCount = 0;
        memset(m_taskFinished,0,sizeof(m_taskFinished));
        memset(m_invitees,0,sizeof(m_invitees));
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userId);
        Read(buf, m_inviterId);
        Read(buf, m_finished);
        Read(buf, m_inviteeCount);
        for (int i = 0; i < m_inviteeCount; i++)
        {
            Read(buf, m_invitees[i]);
            Read(buf, m_taskFinished[i]);
        }
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_userId);
        Write(buf, m_inviterId);
        Write(buf, m_finished);
        Write(buf, m_inviteeCount);
        for (int i = 0; i < m_inviteeCount; i++)
        {
            Write(buf, m_invitees[i]);
            Write(buf, m_taskFinished[i]);
        }
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLDB2LMGBindingRelationships>();
    }
};

//请求 邀请人id及任务完成
struct LMsgLM_2_LDB_ReqInfo : public LMsgSC
{
    enum ReqCode
    {
        ReqInviterInfo = 1,
        TaskFinished = 2,
        DeleteUser = 3,
    };

    int         m_reqCode;    //1:req 2:task finished
    std::string	m_strUUID;
    int         m_userId;
    int         m_param;

    LMsgLM_2_LDB_ReqInfo() : LMsgSC(MSG_LM_2_LDB_REQ_INFO)
    {
        m_reqCode = 0;
        m_userId = 0;
        m_param = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_reqCode), m_reqCode);
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
        ReadMapData(obj, NAME_TO_STR(m_strUUID), m_strUUID);
        ReadMapData(obj, NAME_TO_STR(m_param), m_param);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 5);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_strUUID), m_strUUID);
        WriteKeyValue(pack, NAME_TO_STR(m_reqCode), m_reqCode);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
        WriteKeyValue(pack, NAME_TO_STR(m_param), m_param);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgLM_2_LDB_ReqInfo>(); }
};

//返回 邀请人id及任务 完成情况
struct LMsgLDB_2_LM_RetInfo : public LMsgSC
{
    std::string		m_strUUID;
    int             m_userId;
    int             m_inviterId;   //邀请人的id
    int             m_finished;

    LMsgLDB_2_LM_RetInfo() : LMsgSC(MSG_LDB_2_LM_RET_INFO)
    {
        m_userId = 0;
        m_inviterId = 0;
        m_finished = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_strUUID), m_strUUID);
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
        ReadMapData(obj, NAME_TO_STR(m_inviterId), m_inviterId);
        ReadMapData(obj, NAME_TO_STR(m_finished), m_finished);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 5);
        WriteKeyValue(pack, NAME_TO_STR(m_strUUID), m_strUUID);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_inviterId), m_inviterId);
        WriteKeyValue(pack, NAME_TO_STR(m_finished), m_finished);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgLDB_2_LM_RetInfo>(); }
};



// 验证用户请求
struct LMsgNewUserVerifyRequest : public LMsg
{
    void* m_pUser;
    TcpConnectionPtr m_gameloginSP;
    int m_severID;
    bool m_bWechatLogin;
    std::string m_openId;
    std::string m_accessToken;
    std::string m_uuid; //新用户注册时需要用到客户端传过来的m_uuid的值去查询wx表

    LMsgNewUserVerifyRequest() : LMsg(MSG_NEWUSER_VERIFY_REQUEST)
    {
        m_pUser = NULL;
        m_severID = 0;
        m_bWechatLogin = false;
    }
    ~LMsgNewUserVerifyRequest()
    {

    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgNewUserVerifyRequest>();
    }
};

struct LMsgNewUserVerifyReply : public LMsg
{
    void* m_pUser;
    TcpConnectionPtr m_gameloginSP;
    int m_severID;
    int m_errorCode;	// 结果 和LMsgS2CMsg结果一直
    std::string m_errorMsg;
    LMsgNewUserVerifyReply() : LMsg(MSG_NEWUSER_VERIFY_REPLY)
    {
        m_pUser = NULL;
        m_severID = 0;
        m_errorCode = -1;
    }
    ~LMsgNewUserVerifyReply()
    {

    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgNewUserVerifyReply>();
    }
};


// 金币服务器信息
struct CoinsInfo
{
    TcpConnectionPtr	m_sp;			//不为空表示金币服务器连接上了
    std::string		    m_ip;			//m_sp有值时有效
    short		        m_port;			//m_sp有值时有效
    Llong	            m_closeTickTime;	//m_sp有值时有效，不为0，表示金币服务器断开了连接，3分钟不恢复才进行剔除.默认值为0

    CoinsInfo()
    {
        m_port = 0;
        m_closeTickTime = 0;
    }
};

struct LMsgCN2LMGLogin :public LMsg
{
    std::string	m_key;	// 用作验证
    std::string	m_ip;
    short		m_port;
    int			m_deskcount;	// 已经分配的桌子的数量 CoinsServer断线时 用来简单的判断

    LMsgCN2LMGLogin() :LMsg(MSG_CN_2_LMG_LOGIN)
    {
        m_port = 0;
        m_deskcount = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_key);
        Read(buf, m_ip);
        Read(buf, m_port);
        Read(buf, m_deskcount);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_key);
        Write(buf, m_ip);
        Write(buf, m_port);
        Write(buf, m_deskcount);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCN2LMGLogin>();
    }
};

// 登录结果
struct LMsgLMG2CNLogin :public LMsg
{
    int		m_result;		// 0成功
    int		m_deskcount;	// 已经分配的桌子的数量 CoinsServer断线时 用来简单的判断

    LMsgLMG2CNLogin() :LMsg(MSG_LMG_2_CN_LOGIN)
    {
        m_result = -1;
        m_deskcount = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_result);
        Read(buf, m_deskcount);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_result);
        Write(buf, m_deskcount);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2CNLogin>();
    }
};

struct LMsgCN2LMGFreeDeskReq :public LMsg
{
    int		m_gameType;
    int		m_count;	// 请求的数量

    LMsgCN2LMGFreeDeskReq() :LMsg(MSG_CN_2_LMG_FREE_DESK_REQ)
    {
        m_gameType = 0;
        m_count = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_gameType);
        Read(buf, m_count);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_gameType);
        Write(buf, m_count);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCN2LMGFreeDeskReq>();
    }
};

struct CoinsDesk
{
    CoinsDesk()
    {
        m_id = 0;
        m_logicID = 0;
    }

    int	m_id;		// 桌子号
    int	m_logicID;	// 要分配到的逻辑服务器

    void ReadMsg(Buffer& buf)
    {
        Read(buf, m_id);
        Read(buf, m_logicID);
    }

    void WriteMsg(Buffer& buf)
    {
        Write(buf, m_id);
        Write(buf, m_logicID);
    }
};

struct LMsgLMG2CNFreeDeskReply :public LMsg
{
    int		m_gameType;
    std::vector<CoinsDesk> m_desk;

    LMsgLMG2CNFreeDeskReply() :LMsg(MSG_LMG_2_CN_FREE_DESK_REPLY)
    {
        m_gameType = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_gameType);
        int count = 0;
        Read(buf, count);
        for (int i = 0 ; i < count; i ++ )
        {
            CoinsDesk desk;
            desk.ReadMsg(buf);
            m_desk.push_back(desk);
        }
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_gameType);
        Write(buf, (int)m_desk.size());
        for (size_t i = 0 ; i < m_desk.size(); i ++ )
        {
            m_desk[i].WriteMsg(buf);
        }
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2CNFreeDeskReply>();
    }
};

struct LMsgCN2LMGRecycleDesk :public LMsg
{
    std::vector<int> m_deskid;

    LMsgCN2LMGRecycleDesk() :LMsg(MSG_CN_2_LMG_RECYCLE_DESK)
    {
    }

    virtual void ReadMsg(Buffer& buf)
    {
        int count = 0;
        Read(buf, count);
        for (int i = 0 ; i < count; i ++ )
        {
            int deskid;
            Read(buf, deskid);
            m_deskid.push_back(deskid);
        }
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, (int)m_deskid.size());
        for (size_t i = 0 ; i < m_deskid.size(); i ++ )
        {
            Write(buf, m_deskid[i]);
        }
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCN2LMGRecycleDesk>();
    }
};

struct LMsgLMG2CNUserLogin:public LMsg
{
    std::string		m_strUUID;
    int				m_gateId;
    std::string		m_ip;
    LUser			m_usert;	// 玩家基础数据 同步

    LMsgLMG2CNUserLogin() :LMsg(MSG_LMG_2_CN_USER_LOGIN)
    {
        m_gateId = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_gateId);
        Read(buf, m_ip);
        m_usert.ReadData(buf);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_gateId);
        Write(buf, m_ip);
        m_usert.WriteData(buf);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2CNUserLogin>();
    }
};

struct LMsgLMG2CNEnterCoinDesk : public LMsg
{
    std::string			m_strUUID;
    int					m_gateId;
    std::string			m_ip;
    LUser				m_usert;
    int					m_state;			//玩法规则 0 转转   1 长沙
    int					m_robotNum;			// 0,不加机器人，1，2，3加机器人数量
    CardValue			m_cardValue[CARD_CONFIG];
    std::vector<int>	m_playType;			//玩法
    int					m_coins;			// 玩家的金币

    LMsgLMG2CNEnterCoinDesk() :LMsg(MSG_LMG_2_CN_ENTER_COIN_DESK)
    {
        m_gateId = 0;
        m_state = 0;
        m_robotNum = 0;
        m_coins = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_gateId);
        Read(buf, m_ip);
        m_usert.ReadData(buf);
        Read(buf, m_state);
        Read(buf, m_robotNum);
        for (int i = 0; i < CARD_CONFIG; ++i)
        {
            Read(buf, m_cardValue[i].m_color);
            Read(buf, m_cardValue[i].m_number);
        }
        int playTypeCount = 0;
        Read(buf, playTypeCount);
        for (int i = 0 ; i < playTypeCount; i ++ )
        {
            int playType;
            Read(buf, playType);
            m_playType.push_back(playType);
        }
        Read(buf, m_coins);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_gateId);
        Write(buf, m_ip);
        m_usert.WriteData(buf);
        Write(buf, m_state);
        Write(buf, m_robotNum);
        for (int i = 0; i < CARD_CONFIG; ++i)
        {
            Write(buf, m_cardValue[i].m_color);
            Write(buf, m_cardValue[i].m_number);
        }
        Write(buf, (int)m_playType.size());
        for (size_t i = 0 ; i < m_playType.size(); i ++ )
        {
            Write(buf, m_playType[i]);
        }
        Write(buf, m_coins);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2CNEnterCoinDesk>();
    }
};

struct LMsgCN2LMGModifyUserCoins :public LMsg
{
    int			m_userid;
    std::string	m_strUUID;
    int			isAddCoins;
    int			coinsNum;
    int			operType;	// 参考COINS_OPER_TYPE

    LMsgCN2LMGModifyUserCoins() :LMsg(MSG_CN_2_LMG_MODIFY_USER_COINS)
    {
        m_userid = 0;
        isAddCoins = 0;
        coinsNum = 0;
        operType = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_userid);
        Read(buf, m_strUUID);
        Read(buf, isAddCoins);
        Read(buf, coinsNum);
        Read(buf, operType);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_userid);
        Write(buf, m_strUUID);
        Write(buf, isAddCoins);
        Write(buf, coinsNum);
        Write(buf, operType);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCN2LMGModifyUserCoins>();
    }
};

struct LMsgLMG2CNGetCoins :public LMsg
{
    std::string	m_strUUID;
    int			m_userid;
    int			m_gateid;

    LMsgLMG2CNGetCoins() :LMsg(MSG_LMG_2_CN_GET_COINS)
    {
        m_userid = 0;
        m_gateid = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userid);
        Read(buf, m_gateid);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_userid);
        Write(buf, m_gateid);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2CNGetCoins>();
    }
};

struct LMsgLMG2GLCoinsServerInfo :public LMsg
{
    CoinsInfo	m_conis;

    LMsgLMG2GLCoinsServerInfo() :LMsg(MSG_LMG_2_GL_COINS_SERVER_INFO)
    {
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_conis.m_ip);
        Read(buf, m_conis.m_port);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_conis.m_ip);
        Write(buf, m_conis.m_port);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLMG2GLCoinsServerInfo>();
    }
};

struct LMsgG2CNLogin :public LMsg
{
    int			m_id;
    std::string	m_key;
    std::string	m_ip;
    short		m_port;
    std::string	m_strOldUserIp;
    short		m_uOldUserPort;

    LMsgG2CNLogin() :LMsg(MSG_G_2_CN_LOGIN)
    {
        m_id = 0;
        m_port = 0;
        m_uOldUserPort = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_id);
        Read(buf, m_key);
        Read(buf, m_ip);
        Read(buf, m_port);
        Read(buf, m_strOldUserIp);
        Read(buf, m_uOldUserPort);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_id);
        Write(buf, m_key);
        Write(buf, m_ip);
        Write(buf, m_port);
        Write(buf, m_strOldUserIp);
        Write(buf, m_uOldUserPort);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgG2CNLogin>();
    }
};

struct LMsgL2CNLogin :public LMsg
{
    int			m_id;
    std::string	m_key;
    std::string	m_ip;
    short		m_port;

    LMsgL2CNLogin() :LMsg(MSG_L_2_CN_LOGIN)
    {
        m_id = 0;
        m_port = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_id);
        Read(buf, m_key);
        Read(buf, m_ip);
        Read(buf, m_port);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_id);
        Write(buf, m_key);
        Write(buf, m_ip);
        Write(buf, m_port);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2CNLogin>();
    }
};

#define DIFFOPOUTTIME 10	// 服务器倒计时和客户端倒计时的差距

struct LMsgCN2LCreateCoinDesk : public LMsg
{
    struct User
    {
        User()
        {
            m_gateId = 0;
            m_pos = INVAILD_POS;
        }

        int				m_gateId;
        std::string		m_ip;
        LUser			m_usert;
        int				m_pos;

        void ReadMsg(Buffer& buf)
        {
            Read(buf, m_gateId);
            Read(buf, m_ip);
            m_usert.ReadData(buf);
            Read(buf, m_pos);
        }

        void WriteMsg(Buffer& buf)
        {
            Write(buf, m_gateId);
            Write(buf, m_ip);
            m_usert.WriteData(buf);
            Write(buf, m_pos);
        }
    };
    std::vector<User>  m_users;

    int             m_deskId;
    int				m_state;			// GameType
    int				m_robotNum;			// 0,不加机器人，1，2，3加机器人数量
    CardValue		m_cardValue[CARD_CONFIG];
    std::vector<int>	m_playType;			//玩法
    int				m_baseScore;		// 底分
    int				m_changeOutTime;	// 换牌的超时时间 <=0 不限制
    int				m_opOutTime;		// 操作倒计时 单位秒 <=0 表示不倒计时


    LMsgCN2LCreateCoinDesk() :LMsg(MSG_CN_2_L_CREATE_COIN_DESK)
    {
        m_deskId = 0;
        m_state = 0;
        m_robotNum = 0;
        m_baseScore = 0;
        m_changeOutTime = 0;
        m_opOutTime = 0;
    }
    virtual void ReadMsg(Buffer& buf)
    {
        int usercount = 0;
        Read(buf, usercount);
        for ( int i = 0; i < usercount; ++i )
        {
            m_users.push_back( User() );
            m_users.back().ReadMsg(buf);
        }

        Read(buf, m_deskId);
        Read(buf, m_state);
        Read(buf, m_robotNum);
        for (int i = 0; i < CARD_CONFIG; ++i)
        {
            Read(buf, m_cardValue[i].m_color);
            Read(buf, m_cardValue[i].m_number);
        }
        int playTypeCount = 0;
        Read(buf, playTypeCount);
        for (int i = 0 ; i < playTypeCount; i ++ )
        {
            int playType;
            Read(buf, playType);
            m_playType.push_back(playType);
        }
        Read(buf, m_baseScore);
        Read(buf, m_changeOutTime);
        Read(buf, m_opOutTime);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, (int)m_users.size());
        for ( size_t i = 0; i < m_users.size(); ++i )
        {
            m_users[i].WriteMsg(buf);
        }
        Write(buf, m_deskId);
        Write(buf, m_state);
        Write(buf, m_robotNum);
        for (int i = 0; i < CARD_CONFIG; ++i)
        {
            Write(buf, m_cardValue[i].m_color);
            Write(buf, m_cardValue[i].m_number);
        }
        Write(buf, (int)m_playType.size());
        for (size_t i = 0 ; i < m_playType.size(); i ++ )
        {
            Write(buf, m_playType[i]);
        }
        Write(buf, m_baseScore);
        Write(buf, m_changeOutTime);
        Write(buf, m_opOutTime);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgCN2LCreateCoinDesk>();
    }
};

struct LMsgL2CNCreateCoinDeskRet: public LMsg
{
    enum ErrorCode
    {
        CoinDesk_NoError = 0,
        CoinDesk_SizeError,
        CoinDesk_UserError,
        CoinDesk_PosError,
        CoinDesk_Unknown,
    };
    int             m_deskId;
    int				m_errorCode;			//0=成功

    LMsgL2CNCreateCoinDeskRet() :LMsg(MSG_L_2_CN_CREATE_COIN_DESK_RET)
    {
        m_deskId = 0;
        m_errorCode = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_deskId);
        Read(buf, m_errorCode);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_deskId);
        Write(buf, m_errorCode);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2CNCreateCoinDeskRet>();
    }
};

struct LMsgL2CNGameResult :public LMsg
{
    int			m_deskId;
    BufferPtr	m_dataResult;		//结算消息 有金币服务器发给客户端

    LMsgL2CNGameResult() :LMsg(MSG_L_2_CN_GAME_RESULT)
    {
        m_deskId = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_deskId);
        m_dataResult = BufferPtr(new Buffer);
        m_dataResult->append(buf.peek(), buf.readableBytes());
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_deskId);
        buf.append(m_dataResult->peek(), m_dataResult->readableBytes());
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2CNGameResult>();
    }
};

struct LMsgL2CNEndCoinDesk :public LMsg
{
    int			m_deskId;
    int			m_score[4];     //金币场结算得分
    BufferPtr	m_dataResult;		//结算消息 有金币服务器发给客户端

    LMsgL2CNEndCoinDesk() :LMsg(MSG_L_2_CN_END_COIN_DESK)
    {
        m_deskId = 0;
        memset(m_score,0,sizeof(m_score));
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_deskId);
        for(int i = 0; i < DESK_USER_COUNT; ++i)
        {
            Read(buf, m_score[i]);
        }
        m_dataResult = BufferPtr(new Buffer);
        m_dataResult->append(buf.peek(), buf.readableBytes());
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_deskId);
        for(int i = 0; i < DESK_USER_COUNT; ++i)
        {
            Write(buf, m_score[i]);
        }
        buf.append(m_dataResult->peek(), m_dataResult->readableBytes());
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2CNEndCoinDesk>();
    }
};

struct LMsgC2SGoOnCoinsDesk :public LMsgSC
{
    int		m_state;				//玩法规则: 0 转转  1  长沙  101-血战到底  102-血流成河
    std::vector<int>	m_playType;		//玩法: 1-自摸加底 2-自摸加番

    LMsgC2SGoOnCoinsDesk() :LMsgSC(MSG_C_2_S_GOON_COINS_ROOM)
    {
        m_state = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_state), m_state);
        msgpack::object array;
        ReadMapData(obj, NAME_TO_STR(m_playType), array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            //int m_count = array.via.array.size;
            for (size_t i = 0; i < array.via.array.size; ++i)
            {
                int v = 0;
                msgpack::object& obj =  *(array.via.array.ptr+i);
                obj.convert(v);
                m_playType.push_back(v);
            }
        }
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SGoOnCoinsDesk>(); }
};

struct LMsgS2CGoOnCoinsDeskRet :public LMsgSC
{
    int		m_errorCode;//0-成功，1-金币不够，2-玩家不处于结算状态，3-未知错误 4-创建桌子失败,5-创建参数有问题
    int		m_remainCount; //m_errorCode=1时有效 表示玩家可以领取的金币的次数

    LMsgS2CGoOnCoinsDeskRet() :LMsgSC(MSG_S_2_C_GOON_COINS_ROOM)
    {
        m_errorCode = 0;
        m_remainCount = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_errorCode), m_errorCode);
        ReadMapData(obj, NAME_TO_STR(m_remainCount), m_remainCount);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
        WriteKeyValue(pack, NAME_TO_STR(m_remainCount), m_remainCount);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CGoOnCoinsDeskRet>(); }
};

struct LMsgS2CKickCoinsDesk :public LMsgSC
{
    int		m_reasonCode;//0-准备超时，1-金币不够，2-玩家已在金币场，3-未知原因 4-房间被解散了

    LMsgS2CKickCoinsDesk() :LMsgSC(MSG_S_2_C_KICK_COINS_ROOM)
    {
        m_reasonCode = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_reasonCode), m_reasonCode);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_reasonCode), m_reasonCode);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CKickCoinsDesk>(); }
};

struct LMsgS2CGiveCoins :public LMsgSC
{
    int		m_reasonCode;// 赠送原因 1-金币不足赠送
    int		m_coins;	 //金币数
    int		m_remainCount; // 剩余次数

    LMsgS2CGiveCoins() :LMsgSC(MSG_S_2_C_GIVE_COINS)
    {
        m_reasonCode = 0;
        m_coins = 0;
        m_remainCount = 0;
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 4);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_reasonCode), m_reasonCode);
        WriteKeyValue(pack, NAME_TO_STR(m_coins), m_coins);
        WriteKeyValue(pack, NAME_TO_STR(m_remainCount), m_remainCount);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CGiveCoins>(); }
};

struct LMsgC2SGetCoins :public LMsgSC
{
    int m_userid;	// 用户ID 目前为了方便需要客户端填写下

    LMsgC2SGetCoins() :LMsgSC(MSG_C_2_S_GET_COINS)
    {
        m_userid = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_msgId), m_msgId);
        ReadMapData(obj, NAME_TO_STR(m_userid), m_userid);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_userid), m_userid);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SGetCoins>(); }
};

struct LMsgS2CGetCoins :public LMsgSC
{
    int		m_result;// 0-成功 1-剩余次数为0
    int		m_coins;	 //金币数
    int		m_remainCount; // 剩余次数

    LMsgS2CGetCoins() :LMsgSC(MSG_S_2_C_GET_COINS)
    {
        m_result = 0;
        m_coins = 0;
        m_remainCount = 0;
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 4);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
        WriteKeyValue(pack, NAME_TO_STR(m_coins), m_coins);
        WriteKeyValue(pack, NAME_TO_STR(m_remainCount), m_remainCount);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CGetCoins>(); }
};

// 金币桌其他玩家的金币数
struct LMsgS2COtherCoins :public LMsgSC
{
    int	m_pos;
    int	m_coins;

    LMsgS2COtherCoins() :LMsgSC(MSG_S_2_C_OTHER_COINS)
    {
        m_pos = INVAILD_POS;
        m_coins = 0;
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_coins), m_coins);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2COtherCoins>(); }
};

//////////////////////////////////////////////////////////////////////////
//logic 发送玩家请求 点赞记录
struct LMsgL2LBDReqCRELog:public LMsg
{
    std::string m_strUUID;
    int	m_userId;
    int    m_time;

    LMsgL2LBDReqCRELog() :LMsg(MSG_L_2_LDB_CRE_LOG)
    {
        m_userId = 0;
        m_time = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userId);
        Read(buf, m_time);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_userId);
        Write(buf, m_time);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2LBDReqCRELog>();
    }
};

//////////////////////////////////////////////////////////////////////////
//一局结束，保存 本次可点赞记录
struct LMsgL2LDBSaveCRELog :public LMsg
{
    std::string m_strUUID;
    int	  		m_id[4];    //玩家id
    int      	m_deskID;   //桌子id
    std::string m_strLogId; //log id
    int      	m_time;     //比赛时间

    LMsgL2LDBSaveCRELog() :LMsg(MSG_L_2_LDB_ENDCRE_LOG)
    {
        memset(m_id,0,sizeof(m_id));
        m_deskID = 0;
        m_time = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        for(int i = 0; i < DESK_USER_COUNT; ++i)
        {
            Read(buf, m_id[i]);
        }
        Read(buf, m_deskID);
        Read(buf, m_strLogId);
        Read(buf, m_time);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        for(int i = 0; i < DESK_USER_COUNT; ++i)
        {
            Write(buf, m_id[i]);
        }
        Write(buf, m_deskID);
        Write(buf, m_strLogId);
        Write(buf, m_time);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2LDBSaveCRELog>();
    }
};


//logicdb 2 logicManger 返回点赞记录
struct LMsgLDB2LM_RecordCRELog :public LMsg
{
    enum
    {
        Length = 64,                   //最大容纳长度
    };
    std::string             m_strUUID;     //用户uuid
    int                m_userId;      //用户id
    int                m_user[Length];//记录中的需要点赞玩家id
    int                m_count;       //记录条数
    std::vector<std::string> m_record; //记录

    LMsgLDB2LM_RecordCRELog() :LMsg(MSG_LDB_2_LM_RET_CRELOGHIS)
    {
        m_record.clear();
        m_count = 0;
        m_userId = 0;
        memset(m_user,0,sizeof(m_user));
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userId);
        for(int i = 0; i < Length; ++i)
        {
            Read(buf, m_user[i]);
        }
        Read(buf, m_count);
        for(int i = 0; i < m_count; ++i)
        {
            std::string log;
            Read(buf, log);

            m_record.push_back(log);
        }
    }

    virtual void WriteMsg(Buffer& buf)
    {
        int count = static_cast<int>(m_record.size());
        Write(buf, m_strUUID);
        Write(buf, m_userId);
        for(int i = 0; i < Length; ++i)
        {
            Write(buf, m_user[i]);
        }
        Write(buf, count);
        for(size_t i = 0; i < m_record.size(); ++i)
        {
            std::string& log = m_record[i];
            Write(buf, log);
        }
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLDB2LM_RecordCRELog>();
    }
};

// logicdb 删除点赞记录
struct LMsgL2LDBDEL_GTU :public LMsg
{
    std::string   m_strUUID;
    int	  m_userId;    //玩家id
    std::string   m_strLog;    //点赞记录json

    LMsgL2LDBDEL_GTU() :LMsg(MSG_L_2_LDB_REQ_DELGTU)
    {
        m_userId = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userId);
        Read(buf, m_strLog);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_userId);
        Write(buf, m_strLog);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2LDBDEL_GTU>();
    }
};

//vip比赛结束
struct LMsgLM2CEN_ADD_CRE :public LMsg
{
    int      m_user[DESK_USER_COUNT];    //logicmanager2center 增加信用值

    LMsgLM2CEN_ADD_CRE() :LMsg(MSG_LM_2_CEN_ADD_CRE)
    {
        memset(m_user,0,sizeof(m_user));
    }

    virtual void ReadMsg(Buffer& buf)
    {
        for(int i = 0; i < DESK_USER_COUNT; ++i)
        {
            Read(buf, m_user[i]);
        }
    }

    virtual void WriteMsg(Buffer& buf)
    {
        for(int i = 0; i < DESK_USER_COUNT; ++i)
        {
            Write(buf, m_user[i]);
        }
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgLM2CEN_ADD_CRE>();
    }
};

//centerserver 保存用户 点赞记录入库
struct LMsgC2C_ADD_CRE :public LMsg
{
    std::string m_strUUID;
    int	  		m_userId;    //玩家id   //桌子id

    LMsgC2C_ADD_CRE() :LMsg(MSG_C_2_C_ADD_CRE)
    {
        m_userId = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
        Read(buf, m_userId);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
        Write(buf, m_userId);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgC2C_ADD_CRE>();
    }
};

struct LMsgL2LMGExchCard :public LMsg
{
    int			m_userid;
    std::string	m_strUUID;
    int			m_add;
    int			m_del;
    int			cardType;
    int			operType;
    std::string	admin;

    LMsgL2LMGExchCard() :LMsg(MSG_LMG_2_L_EXCH_CARD)
    {
        m_userid = 0;
        m_add = 0;
        cardType = 0;
        m_del = 0;
        operType = 0;
        cardType = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_userid);
        Read(buf, m_strUUID);
        Read(buf, m_add);
        Read(buf, m_del);
        Read(buf, cardType);
        Read(buf, operType);
        Read(buf, admin);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_userid);
        Write(buf, m_strUUID);
        Write(buf, m_add);
        Write(buf, m_del);
        Write(buf, cardType);
        Write(buf, operType);
        Write(buf, admin);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgL2LMGExchCard>();
    }
};


struct LMsgCe2LGMFengHao : public LMsg
{
    std::string		m_strUUID; //玩家UID

    LMsgCe2LGMFengHao() :LMsg(MSG_CE_2_L_GM_FENGHAO)
    {
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_strUUID);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_strUUID);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgCe2LGMFengHao>(); }
};

struct LMsgLMG2LKickDesk : public LMsg
{
    int		m_userid; //玩家UID

    LMsgLMG2LKickDesk() :LMsg(MSG_LMG_2_L_KICK_DESK)
    {
        m_userid = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_userid);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_userid);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgLMG2LKickDesk>(); }
};

struct LMsgL2LMGDeskListUpdate : public LMsg
{
    int		m_type;   //0桌子创建失败回收，1桌子人数变化，2桌子解散，3玩家在桌子中掉线重连
    int		m_flag;   //人数变化时：1进入桌子， 2离开桌子
    int		m_userId; //房主ID
    int		m_deskId; //桌子号

    LMsgL2LMGDeskListUpdate() :LMsg(MSG_L_2_LMG_DESK_LIST_UPDATE)
    {
        m_type = 0;
        m_flag = 0;
        m_userId = 0;
        m_deskId = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_type);
        Read(buf, m_flag);
        Read(buf, m_userId);
        Read(buf, m_deskId);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_type);
        Write(buf, m_flag);
        Write(buf, m_userId);
        Write(buf, m_deskId);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgL2LMGDeskListUpdate>(); }
};

// 增加后台强制解散房间功能
struct LMsgCe2LGMDissolveRoom : public LMsg
{
    int		m_gameID;		//游戏ID
    int		m_roomID;		//房间ID

    LMsgCe2LGMDissolveRoom() :LMsg(MSG_CE_2_L_GM_DISSOLVE_ROOM)
    {
        m_gameID = 0;
        m_roomID = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_gameID);
        Read(buf, m_roomID);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_gameID);
        Write(buf, m_roomID);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgCe2LGMDissolveRoom>(); }
};

struct LMsgLMG2LGMDissolveRoom : public LMsg
{
    int		m_roomID;		//房间ID

    LMsgLMG2LGMDissolveRoom() :LMsg(MSG_LMG_2_L_GM_DISSOLVE_ROOM)
    {
        m_roomID = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_roomID);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_roomID);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgLMG2LGMDissolveRoom>(); }
};

//追加再来一局功能
struct LMsgLMG2LRematch : public LMsg
{
    int				m_userid;
    int				m_gateId;
    std::string		m_strUUID;
    int				m_deskID;
    std::string		m_ip;
    LUser			m_usert;
    int				m_flag;				//房间类型，1-2圈，2-四圈，3-8圈
    int				m_payment;			//付费方式： 1 房主付费，2 平均付费， 3 大赢家付费
    int				m_nIPAntiCheat;		//IP防作弊
    int				m_nGPSEarlyWarning;	//GPS预警
    std::string		m_GPSLat;			//GPS经度
    std::string		m_GPSLng;			//GPS纬度
    std::string		m_secret;			//房间密码，如果为空，服务器会随机一个密码
    int				m_gold;				//房间抵住，单位元，最低为1元，最多不能超过100
    int				m_state;			//玩法规则 0 转转   1 长沙
    int				m_robotNum;			// 0,不加机器人，1，2，3加机器人数量
    CardValue		m_cardValue[CARD_CONFIG];
    int				m_playTypeCount;
    std::vector<int> m_playType;			//玩法
    std::string		m_rematchKey;		//再来一局key 上一局房间id+玩家id(东)+玩家id(南)+玩家id(西)+玩家id(北)

    LMsgLMG2LRematch() :LMsg(MSG_LMG_2_L_REMATCH)
    {
        m_userid = 0;
        m_gateId = 0;
        m_deskID = 0;
        m_flag = 0;
        m_gold = 0;
        m_state = 0;
        m_robotNum = 0;
        m_playTypeCount = 0;
        m_payment = 0;
        m_nGPSEarlyWarning = 0;
        m_nIPAntiCheat = 0;
    }
    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_userid);
        Read(buf, m_gateId);
        Read(buf, m_strUUID);
        Read(buf, m_deskID);
        Read(buf, m_ip);
        m_usert.ReadData(buf);
        Read(buf, m_flag);

        Read(buf, m_gold);
        Read(buf, m_state);
        Read(buf, m_robotNum);
        for (int i = 0; i < CARD_CONFIG; ++i)
        {
            Read(buf, m_cardValue[i].m_color);
            Read(buf, m_cardValue[i].m_number);
        }
        Read(buf, m_playTypeCount);
        for (int i = 0; i < m_playTypeCount; i++)
        {
            int playType;
            Read(buf, playType);
            m_playType.push_back(playType);
        }
        Read(buf, m_payment);
        Read(buf, m_nIPAntiCheat);
        Read(buf, m_nGPSEarlyWarning);
        Read(buf, m_GPSLat);
        Read(buf, m_GPSLng);
        Read(buf, m_rematchKey);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_userid);
        Write(buf, m_gateId);
        Write(buf, m_strUUID);
        Write(buf, m_deskID);
        Write(buf, m_ip);
        m_usert.WriteData(buf);
        Write(buf, m_flag);
        Write(buf, m_gold);
        Write(buf, m_state);
        Write(buf, m_robotNum);
        for (int i = 0; i < CARD_CONFIG; ++i)
        {
            Write(buf, m_cardValue[i].m_color);
            Write(buf, m_cardValue[i].m_number);
        }
        Write(buf, m_playTypeCount);
        for (int i = 0; i < m_playTypeCount; i++)
        {
            Write(buf, m_playType[i]);
        }
        Write(buf, m_payment);
        Write(buf, m_nIPAntiCheat);
        Write(buf, m_nGPSEarlyWarning);
        Write(buf, m_GPSLat);
        Write(buf, m_GPSLng);
        Write(buf, m_rematchKey);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgLMG2LRematch>(); }
};

struct LMsgL2LMGRematchRet : public LMsg
{
    int			m_errorCode;	//错误码 0-成功，1-创建房间失败
    std::string	m_strUUID;		//UUID
    int			m_userId;		//用户ID
    std::string	m_rematchKey;	//再来一局key 上一局房间id+玩家id(东)+玩家id(南)+玩家id(西)+玩家id(北)
    int			m_deskId;		//桌子ID

    LMsgL2LMGRematchRet() :LMsg(MSG_L_2_LMG_REMATCH)
    {
        m_errorCode = 0;
    }

    virtual void ReadMsg(Buffer& buf)
    {
        Read(buf, m_errorCode);
        Read(buf, m_strUUID);
        Read(buf, m_userId);
        Read(buf, m_rematchKey);
        Read(buf, m_deskId);
    }

    virtual void WriteMsg(Buffer& buf)
    {
        Write(buf, m_errorCode);
        Write(buf, m_strUUID);
        Write(buf, m_userId);
        Write(buf, m_rematchKey);
        Write(buf, m_deskId);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgL2LMGRematchRet>(); }
};


#endif //PROJECT_LMSGSS_H
