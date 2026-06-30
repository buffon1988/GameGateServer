//
// Created by root on 9/14/17.
//

#ifndef PROJECT_LMSGCS_H
#define PROJECT_LMSGCS_H

#include "LMsg.h"


/////////////////////////// 客户端与服务器之间的消息 ////////////////////////////////
/// 客户端发送消息
struct LMsgC2SMsg : public LMsgSC
{
    std::string			m_openId;
    std::string			m_nike;
    std::string			m_sign;
    std::string			m_plate;
    std::string			m_accessToken;
    std::string			m_refreshToken;
    std::string			m_md5;
    int     			m_severID;
    std::string			m_uuid;
    int     			m_sex;
    std::string			m_imageUrl;
    std::string			m_nikename;
    int     			m_type;

    LMsgC2SMsg() :LMsgSC(MSG_C_2_S_MSG)
    {
        m_severID = 0;
        m_sex = 0;
        m_type = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, "m_openId", m_openId);
        ReadMapData(obj, "m_nike", m_nike);
        ReadMapData(obj, "m_sign", m_sign);
        ReadMapData(obj, "m_plate", m_plate);
        ReadMapData(obj, "m_accessToken", m_accessToken);
        ReadMapData(obj, "m_refreshToken", m_refreshToken);
        ReadMapData(obj, "m_md5", m_md5);
        ReadMapData(obj, "m_severID", m_severID);
        ReadMapData(obj, "m_uuid", m_uuid);
        ReadMapData(obj, "m_sex", m_sex);
        ReadMapData(obj, "m_imageUrl", m_imageUrl);
        ReadMapData(obj, "m_nikename", m_nikename);
        ReadMapData(obj, "m_type", m_type);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 14);
        WriteKeyValue(pack, "m_msgId", m_msgId);
        WriteKeyValue(pack, "m_openId", m_openId);
        WriteKeyValue(pack, "m_nike", m_nike);
        WriteKeyValue(pack, "m_sign", m_sign);
        WriteKeyValue(pack, "m_plate", m_plate);
        WriteKeyValue(pack, "m_accessToken", m_accessToken);
        WriteKeyValue(pack, "m_refreshToken", m_refreshToken);
        WriteKeyValue(pack, "m_md5", m_md5);
        WriteKeyValue(pack, "m_severID", m_severID);
        WriteKeyValue(pack, "m_uuid", m_uuid);
        WriteKeyValue(pack, "m_sex", m_sex);
        WriteKeyValue(pack, "m_imageUrl", m_imageUrl);
        WriteKeyValue(pack, "m_nikename", m_nikename);
        WriteKeyValue(pack, "m_type", m_type);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgC2SMsg>();
    }
};

/// 客户端发送消息
struct LMsgS2CMsg : public LMsgSC
{
    int			m_errorCode;//0-成功,1-服务器还没启动成功,2-微信登陆失败,3-微信返回失败,4-创建角色失败,5-在原APP未退,6-错误的地区号,7-MD5验证失败,
    int			m_seed;
    int			m_id;
    std::string	m_gateIp;
    short		m_gatePort;
    std::string	m_errorMsg;	//若登陆失败，返回玩家错误原因;
    int         m_totalPlayNum;

    LMsgS2CMsg() :LMsgSC(MSG_S_2_C_MSG)
    {
        m_errorCode = -1;
        m_seed = 0;
        m_id = 0;
        m_gatePort = 0;
        m_totalPlayNum = 0;
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 8);
        WriteKeyValue(pack, "m_msgId", m_msgId);
        WriteKeyValue(pack, "m_errorCode", m_errorCode);
        WriteKeyValue(pack, "m_seed", m_seed);
        WriteKeyValue(pack, "m_id", m_id);
        WriteKeyValue(pack, "m_gateIp", m_gateIp);
        WriteKeyValue(pack, "m_gatePort", m_gatePort);
        WriteKeyValue(pack, "m_errorMsg", m_errorMsg);
        WriteKeyValue(pack, "m_totalPlayNum",m_totalPlayNum);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgS2CMsg>();
    }
};

/// 客户端实体消息验证头
struct LMsgC2SVerifyHead : public LMsgSC
{
    std::string m_strUserId;	//用户id
    int		    m_iMd5Begin;	//消息实体计算MD5的起始位置
    int		    m_iMd5End;		//消息实体计算MD5的结束位置
    std::string m_strMd5;		//消息实体MD5值
    std::string m_strVerifyKey;	//服务器给的一串验证码
    int		    m_lMsgOrder;	//消息实体序列,每个消息包都需要增值

    LMsgC2SVerifyHead() : LMsgSC(MSG_C_2_S_VERIFY_HEAD)
    {
        m_iMd5Begin = -1;
        m_iMd5End   = -1;
        m_lMsgOrder = 0xFFFFFFFF;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, "m_strUserId", m_strUserId);
        ReadMapData(obj, "m_iMd5Begin", m_iMd5Begin);
        ReadMapData(obj, "m_iMd5End", m_iMd5End);
        ReadMapData(obj, "m_strMd5", m_strMd5);
        ReadMapData(obj, "m_strVerifyKey", m_strVerifyKey);
        ReadMapData(obj, "m_lMsgOrder", m_lMsgOrder);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgC2SVerifyHead>();
    }
};

//客户端发送登录Gate消息
struct LMsgC2SLoginGate : public LMsgSC
{
    std::string m_strUserUUID;

    LMsgC2SLoginGate() : LMsgSC(MSG_C_2_S_LOGIN_GATE){}

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, "m_strUserUUID", m_strUserUUID);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, "m_msgId", m_msgId);
        WriteKeyValue(pack, "m_strUserUUID", m_strUserUUID);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgC2SLoginGate>();
    }
};

struct LMsgS2CShutDown : public LMsgSC
{
    int m_nShutDownInfo;

    LMsgS2CShutDown() : LMsgSC(MSG_S_2_C_SHUTDOWN_GATE)
    {
        m_nShutDownInfo = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, "m_msgId", m_msgId);
        WriteKeyValue(pack, "m_nShutDownInfo", m_nShutDownInfo);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgS2CShutDown>();
    }
};

//Gate回客户端登录信息
struct LMsgS2CLoginGate : public LMsgSC
{
    std::string     m_strUserUUID;
    std::string     m_strKey;
    unsigned long   m_uMsgOrder;

    LMsgS2CLoginGate() : LMsgSC(MSG_S_2_C_LOGIN_GATE)
    {
        m_uMsgOrder = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, "m_strUserUUID", m_strUserUUID);
        ReadMapData(obj, "m_strKey", m_strKey);
        ReadMapData(obj, "m_uMsgOrder", m_uMsgOrder);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 4);
        WriteKeyValue(pack, "m_msgId", m_msgId);
        WriteKeyValue(pack, "m_strUserUUID", m_strUserUUID);
        WriteKeyValue(pack, "m_strKey", m_strKey);
        WriteKeyValue(pack, "m_uMsgOrder", m_uMsgOrder);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgS2CLoginGate>();
    }
};

//////////////////////////////////////////////////////////////////////////
//客户端发送心跳包
struct LMsgC2SHeart : public LMsgSC
{
    LMsgC2SHeart() :LMsgSC(MSG_C_2_S_HEART){}

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 1);
        WriteKeyValue(pack, "m_msgId", m_msgId);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgC2SHeart>();
    }
};


//////////////////////////////////////////////////////////////////////////
//服务器发送心跳包
struct LMsgS2CHeart :public LMsgSC
{
    int	m_time;//时间戳

    LMsgS2CHeart() :LMsgSC(MSG_S_2_C_HEART)
    {
        m_time = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, "m_msgId", m_msgId);
        ReadMapData(obj, "m_time", m_time);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack,2);
        WriteKeyValue(pack, "m_msgId", m_msgId);
        WriteKeyValue(pack, "m_time", m_time);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgS2CHeart>();
    }
};

//服务器返回跑马灯信息
struct LMsgS2CHorseInfo : public LMsgSC
{
    enum
    {
        TInfo_Horse = 0,
        TInfo_Free = 1,
        TInfo_Exch = 2,
    };

    std::string	m_str;
    int         m_type;

    LMsgS2CHorseInfo() :LMsgSC(MSG_S_2_C_HORSE_INFO)
    {
        m_type = 0;   //默认0为跑马灯消息推送，1为限时免费
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_str), m_str);
        ReadMapData(obj, NAME_TO_STR(m_type), m_type);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_str), m_str);
        WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgS2CHorseInfo>();
    }
};

//客户端请求登录大厅
struct LMsgC2SLogin : public LMsgSC
{
    int	        m_seed;
    int	        m_id;
    std::string	m_md5;

    LMsgC2SLogin() :LMsgSC(MSG_C_2_S_LOGIN)
    {
        m_seed = 0;
        m_id = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_seed), m_seed);
        ReadMapData(obj, NAME_TO_STR(m_id), m_id);
        ReadMapData(obj, NAME_TO_STR(m_md5), m_md5);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_seed), m_seed);
        WriteKeyValue(pack, NAME_TO_STR(m_id), m_id);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgC2SLogin>();
    }
};


//////////////////////////////////////////////////////////////////////////
//客户端请求通知
struct LMsgC2SNotice :public LMsgSC
{
    LMsgC2SNotice() :LMsgSC(MSG_C_2_S_NOTICE){}

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 1);
        WriteKeyValue(pack, "m_msgId", m_msgId);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SNotice>(); }
};


//////////////////////////////////////////////////////////////////////////
//服务器返回通知内容
struct Notice
{
    Notice()
    {
        m_time = 0;
    }

    int 	    m_time;//时间戳
    std::string	m_title;//标题
    std::string m_content;//内容

    MSGPACK_DEFINE(m_time, m_title, m_content);
};

struct LMsgS2CNotice :public LMsgSC
{
    int	m_count;//时间戳
    Notice	m_data[20];

    LMsgS2CNotice() :LMsgSC(MSG_S_2_C_NOTICE)
    {
        m_count = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        m_count = 0;

        msgpack::object array;
        ReadMapData(obj, "array", array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            for (size_t i = 0; i < array.via.array.size; ++i)
            {
                msgpack::object pv = *(array.via.array.ptr+i);
                pv.convert(m_data[i]);
                m_count++;
            }
        }

    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKey(pack, "array");
        std::vector<Notice>vce;
        for (int i = 0; i < m_count&&i < 20; ++i)
        {
            vce.push_back(m_data[i]);
        }
        WriteKey(pack,vce);

    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CNotice>(); }
};


//////////////////////////////////////////////////////////////////////////
struct DeskListItem
{
    int m_deskId;		            //桌子ID
    int m_state;		            //玩法(溪水麻将、新发财)
    int m_baseScore;	            //底分
    int m_payType;		            //付费方式
    int m_numPlayer;	            //人数
    int m_maxCircle;	            //最大局数
    std::vector<int> m_playType;    //玩法

    MSGPACK_DEFINE(m_deskId, m_state, m_baseScore, m_payType, m_numPlayer, m_maxCircle, m_playType);

    enum EDeskItemType
    {
        DESK_CREATE_FAILED		= 0,	//桌子创建失败
        DESK_USER_NUM_CHANGE	= 1,	//桌子人数变化
        DESK_RESET				= 2,	//桌子解散
        DESK_LEAVE_RECONNECT	= 3,	//玩家在桌子中掉线重连
        DESK_START				= 4,	//桌子开始打牌
        DEKS_MAX_CREATE			= 10,	//最多能代开房间个数
        USER_IN_DESK			= 101,	//玩家进入桌子
        USER_OUT_DESK			= 102	//玩家离开桌子
    };

    DeskListItem()
    {
        m_deskId = 0;
        m_state = 0;
        m_baseScore = 0;
        m_payType = 0;
        m_numPlayer = 0;
        m_maxCircle = 0;
        m_playType.clear();
    }

    void Read(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_deskId), m_deskId);
        ReadMapData(obj, NAME_TO_STR(m_state), m_state);
        ReadMapData(obj, NAME_TO_STR(m_baseScore), m_baseScore);
        ReadMapData(obj, NAME_TO_STR(m_payType), m_payType);
        ReadMapData(obj, NAME_TO_STR(m_numPlayer), m_numPlayer);
        ReadMapData(obj, NAME_TO_STR(m_maxCircle), m_maxCircle);
        ReadMapData(obj, NAME_TO_STR(m_playType), m_playType);

    }

    void Write(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteKeyValue(pack, NAME_TO_STR(m_deskId), m_deskId);
        WriteKeyValue(pack, NAME_TO_STR(m_state), m_state);
        WriteKeyValue(pack, NAME_TO_STR(m_baseScore), m_baseScore);
        WriteKeyValue(pack, NAME_TO_STR(m_payType), m_payType);
        WriteKeyValue(pack, NAME_TO_STR(m_numPlayer), m_numPlayer);
        WriteKeyValue(pack, NAME_TO_STR(m_maxCircle), m_maxCircle);
        WriteKeyValue(pack, NAME_TO_STR(m_playType), m_playType);

    }
};

//返回玩家登陆结果消息
struct LMsgS2CLogin:public LMsgSC
{
    int		m_errorCode;//登录错误码，0-登录成功,1-帐号未注册，2-帐号已登录，3-未知错误，4-桌子中掉线重连拉代开房列表
    int		m_id;
    std::string		m_nike;
    int		m_exp;
    int		m_sex;
    std::string		m_face;//
    std::string		m_provice;
    std::string		m_city;
    std::string		m_ip;
    int		m_new;//0-不是，1-是
    int		m_card1;//新手奖励房卡1
    int		m_card2;//新手奖励房卡2
    int		m_card3;//新手奖励房卡3
    int		m_state;//0-进入大厅，1-进入桌子
    std::string		m_buyInfo;//够买信息
    int		m_hide;//购买隐藏0-隐藏 1- 不隐藏
    int		m_gm;//0-不是，1-是GM
    int     m_oldUser;
    int		m_loginInterval;	// 距离上次登录多少天, 老用户登录送房卡活动使用字段
    std::vector<DeskListItem> m_deskList; //房间列表

    LMsgS2CLogin() :LMsgSC(MSG_S_2_C_LOGIN)
    {
        m_errorCode = -1;
        m_id = 0;
        m_exp = 0;
        m_sex = 1;
        m_new = 0;
        m_card1 = 0;
        m_card2 = 0;
        m_card3 = 0;
        m_state = 0;
        m_hide = 0;
        m_gm = 0;
        m_oldUser = 1;
        m_loginInterval = 0;
        m_deskList.clear();
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_errorCode), m_errorCode);
        ReadMapData(obj, NAME_TO_STR(m_id), m_id);
        ReadMapData(obj, NAME_TO_STR(m_nike), m_nike);
        ReadMapData(obj, NAME_TO_STR(m_exp), m_exp);
        ReadMapData(obj, NAME_TO_STR(m_sex), m_sex);
        ReadMapData(obj, NAME_TO_STR(m_face), m_face);
        ReadMapData(obj, NAME_TO_STR(m_provice), m_provice);
        ReadMapData(obj, NAME_TO_STR(m_city), m_city);
        ReadMapData(obj, NAME_TO_STR(m_ip), m_ip);
        ReadMapData(obj, NAME_TO_STR(m_new), m_new);
        ReadMapData(obj, NAME_TO_STR(m_card1), m_card1);
        ReadMapData(obj, NAME_TO_STR(m_card2), m_card2);
        ReadMapData(obj, NAME_TO_STR(m_card3), m_card3);
        ReadMapData(obj, NAME_TO_STR(m_oldUser),m_oldUser);
        ReadMapData(obj, NAME_TO_STR(m_loginInterval),m_loginInterval);
        ReadMapData(obj, NAME_TO_STR(m_deskList), m_deskList);

    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 21);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
        WriteKeyValue(pack, NAME_TO_STR(m_id), m_id);
        WriteKeyValue(pack, NAME_TO_STR(m_nike), m_nike);
        WriteKeyValue(pack, NAME_TO_STR(m_exp), m_exp);
        WriteKeyValue(pack, NAME_TO_STR(m_sex), m_sex);
        WriteKeyValue(pack, NAME_TO_STR(m_face), m_face);
        WriteKeyValue(pack, NAME_TO_STR(m_provice), m_provice);
        WriteKeyValue(pack, NAME_TO_STR(m_city), m_city);
        WriteKeyValue(pack, NAME_TO_STR(m_ip), m_ip);
        WriteKeyValue(pack, NAME_TO_STR(m_new), m_new);
        WriteKeyValue(pack, NAME_TO_STR(m_card1), m_card1);
        WriteKeyValue(pack, NAME_TO_STR(m_card2), m_card2);
        WriteKeyValue(pack, NAME_TO_STR(m_card3), m_card3);
        WriteKeyValue(pack, NAME_TO_STR(m_state), m_state);
        WriteKeyValue(pack, NAME_TO_STR(m_buyInfo), m_buyInfo);
        WriteKeyValue(pack, NAME_TO_STR(m_hide), m_hide);
        WriteKeyValue(pack, NAME_TO_STR(m_gm), m_gm);
        WriteKeyValue(pack, NAME_TO_STR(m_oldUser), m_oldUser);
        WriteKeyValue(pack, NAME_TO_STR(m_loginInterval), m_loginInterval);
        WriteKeyValue(pack, NAME_TO_STR(m_deskList), m_deskList);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CLogin>();}
};

//////////////////////////////////////////////////////////////////////////
//服务器返回玩家物品信息
struct LMsgS2CItemInfo:public LMsgSC
{
    int	m_card1;
    int	m_card2;
    int	m_card3;
    int	m_coins;
    int m_credit;

    LMsgS2CItemInfo() :LMsgSC(MSG_S_2_C_ITEM_INFO)
    {
        m_card1 = 0;
        m_card2 = 0;
        m_card3 = 0;
        m_coins = 0;
        m_credit = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_card1), m_card1);
        ReadMapData(obj, NAME_TO_STR(m_card2), m_card2);
        ReadMapData(obj, NAME_TO_STR(m_card3), m_card3);
        ReadMapData(obj, NAME_TO_STR(m_coins), m_coins);
        ReadMapData(obj, NAME_TO_STR(m_credit), m_credit);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 6);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_card1), m_card1);
        WriteKeyValue(pack, NAME_TO_STR(m_card2), m_card2);
        WriteKeyValue(pack, NAME_TO_STR(m_card3), m_card3);
        WriteKeyValue(pack, NAME_TO_STR(m_coins), m_coins);
        WriteKeyValue(pack, NAME_TO_STR(m_credit), m_credit);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CItemInfo>();}
};

//////////////////////////////////////////////////////////////////////////
//服务器返回玩家进入桌子
struct LMsgS2CIntoDesk:public LMsgSC
{
    int			m_deskId;
    int			m_pos;	//我的位置，0-3 分别代表东，南，西，北
    int			m_ready;//我是否准备，0-未准备，1-已经准备
    int			m_score;//我的积分
    int			m_coins;//我的金币
    int         m_credits; //信用值
    int			m_state;//玩法  0 转转 3 长沙 101 血流  102 血战
    int			m_maxCircle;//最大局数
    int			m_changeOutTime;// 换牌的超时时间 <=0 不限制
    int			m_opOutTime; // 桌内操作超时时间 <=0 不限制
    int			m_baseScore; // 基础分
    int			m_nPayMentType;//付费方式
    std::vector<int>	m_playtype;
    bool		m_bOwner; //标记进入房间的玩家是否为房主
    Lint		m_isStart = 0;	//游戏是否已经开始了

    LMsgS2CIntoDesk():LMsgSC(MSG_S_2_C_INTO_DESK)
    {
        m_deskId = 0;
        m_pos = INVAILD_POS;
        m_ready = 0;
        m_score = 0;
        m_coins = 0;
        m_state = 0;
        m_maxCircle = 0;
        m_changeOutTime = 0;
        m_opOutTime = 0;
        m_baseScore = 1;
        m_credits = 0;
        m_nPayMentType = 0;
        m_bOwner = false;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_deskId), m_deskId);
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
        ReadMapData(obj, NAME_TO_STR(m_ready), m_ready);
        ReadMapData(obj, NAME_TO_STR(m_score), m_score);
        ReadMapData(obj, NAME_TO_STR(m_coins), m_coins);
        ReadMapData(obj, NAME_TO_STR(m_credits), m_credits);
        ReadMapData(obj, NAME_TO_STR(m_state), m_state);
        ReadMapData(obj, NAME_TO_STR(m_maxCircle), m_maxCircle);
        ReadMapData(obj, NAME_TO_STR(m_playtype), m_playtype);
        ReadMapData(obj, NAME_TO_STR(m_changeOutTime), m_changeOutTime);
        ReadMapData(obj, NAME_TO_STR(m_opOutTime), m_opOutTime);
        ReadMapData(obj, NAME_TO_STR(m_baseScore), m_baseScore);
        ReadMapData(obj, NAME_TO_STR(m_nPayMentType), m_nPayMentType);
        ReadMapData(obj, NAME_TO_STR(m_bOwner), m_bOwner);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 16);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_deskId), m_deskId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_ready), m_ready);
        WriteKeyValue(pack, NAME_TO_STR(m_score), m_score);
        WriteKeyValue(pack, NAME_TO_STR(m_coins), m_coins);
        WriteKeyValue(pack, NAME_TO_STR(m_credits), m_credits);
        WriteKeyValue(pack, NAME_TO_STR(m_state), m_state);
        WriteKeyValue(pack, NAME_TO_STR(m_maxCircle), m_maxCircle);
        WriteKeyValue(pack, NAME_TO_STR(m_playtype), m_playtype);
        WriteKeyValue(pack, NAME_TO_STR(m_changeOutTime), m_changeOutTime);
        WriteKeyValue(pack, NAME_TO_STR(m_opOutTime), m_opOutTime);
        WriteKeyValue(pack, NAME_TO_STR(m_baseScore), m_baseScore);
        WriteKeyValue(pack, NAME_TO_STR(m_nPayMentType), m_nPayMentType);
        WriteKeyValue(pack, NAME_TO_STR(m_bOwner), m_bOwner);
        WriteKeyValue(pack, NAME_TO_STR(m_isStart), m_isStart);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CIntoDesk>();}
};

//////////////////////////////////////////////////////////////////////////
//服务器返回桌子添加一个玩家
struct LMsgS2CDeskAddUser:public LMsgSC
{
    int			m_userId;
    std::string	m_nike;//玩家昵称
    std::string	m_face;//玩家图形，为空就为默认图形
    int			m_sex;//玩家性别
    int			m_pos;	//位置，0-3 分别代表东，南，西，北
    int			m_ready;//0-未准备，1-已准备
    int			m_score;//玩家积分
    std::string	m_ip;
    std::string	m_GPSLat;				//GPS经度
    std::string	m_GPSLng;				//GPS纬度
    bool		m_online;//是否在线，true在线，false不在线
    int         m_bBusy;	//用户是否忙碌
    int			m_coins;//玩家金币
    int         m_credits; //信用值

    LMsgS2CDeskAddUser() :LMsgSC(MSG_S_2_C_ADD_USER)
    {
        m_userId = 0;
        m_sex = 1;
        m_pos = INVAILD_POS;
        m_ready = 0;
        m_score = 0;
        m_online = false;
        m_coins = 0;
        m_credits = 0;
        m_bBusy = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
        ReadMapData(obj, NAME_TO_STR(m_nike), m_nike);
        ReadMapData(obj, NAME_TO_STR(m_face), m_face);
        ReadMapData(obj, NAME_TO_STR(m_sex), m_sex);
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
        ReadMapData(obj, NAME_TO_STR(m_ready), m_ready);
        ReadMapData(obj, NAME_TO_STR(m_score), m_score);
        ReadMapData(obj, NAME_TO_STR(m_ip), m_ip);
        ReadMapData(obj, NAME_TO_STR(m_GPSLat), m_GPSLat);
        ReadMapData(obj, NAME_TO_STR(m_GPSLng), m_GPSLng);
        ReadMapData(obj, NAME_TO_STR(m_online), m_online);
        ReadMapData(obj, NAME_TO_STR(m_coins), m_coins);
        ReadMapData(obj, NAME_TO_STR(m_credits), m_credits);
        ReadMapData(obj, NAME_TO_STR(m_bBusy), m_bBusy);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 15);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
        WriteKeyValue(pack, NAME_TO_STR(m_nike), m_nike);
        WriteKeyValue(pack, NAME_TO_STR(m_face), m_face);
        WriteKeyValue(pack, NAME_TO_STR(m_sex), m_sex);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_ready), m_ready);
        WriteKeyValue(pack, NAME_TO_STR(m_score), m_score);
        WriteKeyValue(pack, NAME_TO_STR(m_ip), m_ip);
        WriteKeyValue(pack, NAME_TO_STR(m_GPSLat), m_GPSLat);
        WriteKeyValue(pack, NAME_TO_STR(m_GPSLng), m_GPSLng);
        WriteKeyValue(pack, NAME_TO_STR(m_online), m_online);
        WriteKeyValue(pack, NAME_TO_STR(m_coins), m_coins);
        WriteKeyValue(pack, NAME_TO_STR(m_credits), m_credits);
        WriteKeyValue(pack, NAME_TO_STR(m_bBusy), m_bBusy);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CDeskAddUser>();}
};

//////////////////////////////////////////////////////////////////////////
//服务器返回删除玩家
struct LMsgS2CDeskDelUser:public LMsgSC
{
    int	m_pos;//删除玩家位置 如果pos == 我的位置，代表我自己被踢出桌子，把桌子其他人清掉

    LMsgS2CDeskDelUser() :LMsgSC(MSG_S_2_C_DEL_USER)
    {
        m_pos = INVAILD_POS;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CDeskDelUser>();}
};

//////////////////////////////////////////////////////////////////////////
//客户端请求推出房间
struct LMsgC2SLeaveDesk:public LMsgSC
{
    int			m_pos;//我的位置

    LMsgC2SLeaveDesk() :LMsgSC(MSG_C_2_S_LEAVE_ROOM)
    {
        m_pos = INVAILD_POS;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgC2SLeaveDesk>();}
};

//////////////////////////////////////////////////////////////////////////
//服务器返回结果
struct LMsgS2CLeaveDesk:public LMsgSC
{
    int			m_errorCode;//0-成功，1-失败

    LMsgS2CLeaveDesk() :LMsgSC(MSG_S_2_C_LEAVE_ROOM)
    {
        m_errorCode = -1;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_errorCode), m_errorCode);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CLeaveDesk>();}
};

//////////////////////////////////////////////////////////////////////////
//客户端申请解算房间
struct LMsgC2SResetDesk:public LMsgSC
{
    int	m_pos;//位置
    int	m_userId; //玩家id
    LMsgC2SResetDesk():LMsgSC(MSG_C_2_S_RESET_ROOM)
    {
        m_pos = INVAILD_POS;
        m_userId = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgC2SResetDesk>();}
};

//////////////////////////////////////////////////////////////////////////
struct LMsgS2CResetDesk:public LMsgSC
{
    int					m_errorCode;//0-等待操作中，1-未开始直接解算，2-三个人同意，解算成功，3-时间到，解算成功，4-有一个人拒绝，解算失败,5-未开始时，非房主解散房间
    int					m_time;//倒计时
    std::string			m_applay;//申请的玩家
    std::string			m_refluse;//拒绝的玩家,有一个玩家拒绝，则解算房间失败
    int					m_flag;//0-等待我同意或者拒绝，1-我已经同意或者拒绝
    std::vector<std::string>	m_agree;//同意的玩家
    std::vector<std::string>	m_wait;//等待操作的玩家
    int						    m_nReflusePos = INVAILD_POS;	//拒绝玩家的位置
    std::vector<int>			m_vecAgreePos;	//同意状态玩家的位置
    std::vector<int>			m_vecWaitPos;	//等待状态玩家的位置

    LMsgS2CResetDesk() :LMsgSC(MSG_S_2_C_RESET_ROOM)
    {
        m_errorCode = -1;
        m_time = 0;
        m_flag = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_errorCode), m_errorCode);
        ReadMapData(obj, NAME_TO_STR(m_flag), m_flag);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 11);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
        WriteKeyValue(pack, NAME_TO_STR(m_time), m_time);
        WriteKeyValue(pack, NAME_TO_STR(m_apply), m_applay);
        WriteKeyValue(pack, NAME_TO_STR(m_flag), m_flag);

        WriteKey(pack,"m_agree");
        WriteKey(pack, m_agree);

        WriteKey(pack,"m_refuse");
        WriteKey(pack,m_refluse);

        WriteKey(pack, "m_wait");
        WriteKey(pack, m_wait);

        WriteKeyValue(pack, NAME_TO_STR(m_nReflusePos), m_nReflusePos);
        WriteKeyValue(pack, NAME_TO_STR(m_vecAgreePos), m_vecAgreePos);
        WriteKeyValue(pack, NAME_TO_STR(m_vecWaitPos), m_vecWaitPos);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CResetDesk>();}
};

//////////////////////////////////////////////////////////////////////////
//客户端同意或者拒绝操作
struct LMsgC2SSelectResetDesk:public LMsgSC
{
    int	m_pos;//位置
    int	m_flag;//1-同意，2-拒绝
    LMsgC2SSelectResetDesk():LMsgSC(MSG_C_2_S_RESET_ROOM_SELECT)
    {
        m_pos = INVAILD_POS;
        m_flag = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
        ReadMapData(obj, NAME_TO_STR(m_flag), m_flag);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_flag), m_flag);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgC2SSelectResetDesk>();}
};

//////////////////////////////////////////////////////////////////////////
//服务器发送桌子状态，这个消息主要用于重连用
struct LMsgS2CDeskState:public LMsgSC
{
#define   MAX_CARD_NUM   (64)
    int		m_state;  //当前桌子状态 1-等待，2-游戏中，会有后面的数据
    int		m_time;   //倒计时
    int		m_flag;   //0-等待别人，1-该我出牌，2-该我思考
    int		m_zhuang; //庄的位置
    int		m_dice1;  //色子1
    int		m_dice2;  //色子2
    int		m_dCount; //桌面剩余的牌
    int		m_pos;    //我的位置
    int		m_cardCount[4];//0-3号玩家手上的牌数量
    int		m_myPos;
    int     m_hType[4];

    //我手上的牌
    CardValue	m_cardValue[MAX_CARD_NUM];

    //碰的牌
    int		m_pCount[4];
    CardValue	m_pCard[4][MAX_CARD_NUM];
    CardValue	m_phCard[4][MAX_CARD_NUM];	// 带混的牌 给客户端填充四张 填充m_pCard就不能填充m_phCard

    //暗杠的牌
    int		m_aCount[4];
    CardValue	m_aCard[4][MAX_CARD_NUM];
    CardValue	m_ahCard[4][MAX_CARD_NUM];	// 带混的牌 给客户端填充四张 填充m_aCard就不能填充m_ahCard

    //明杠的牌
    int		m_mCount[4];
    CardValue	m_mCard[4][MAX_CARD_NUM];
    CardValue	m_mhCard[4][MAX_CARD_NUM];	// 带混的牌 给客户端填充四张 填充m_mCard就不能填充m_mhCard

    //出出去的牌
    int		m_oCount[4];
    CardValue	m_oCard[4][MAX_CARD_NUM];

    //吃的牌
    int		m_eCount[4];
    CardValue	m_eCard[4][MAX_CARD_NUM];

    CardValue	m_winCard[4];	// 玩家的胡牌
    bool		m_bChange[4];	// 玩家换牌
    CardValue	m_changeCard[3];	// 玩家换的牌

    int        m_hCount[4];                //卡五星 中胡的牌数量
    CardValue   m_hCard[4][MAX_CARD_NUM];   //

    int        m_winList[4];

    std::vector<int>	m_playtype;	//玩法

    //躺牌
    int        m_tCount[4];    //躺牌数量
    CardValue   m_tCard[4][MAX_CARD_NUM];   //躺牌

    int		m_changeOutTime;// 换牌的超时时间 <=0 不限制
    int		m_opOutTime; // 桌内操作超时时间 <=0 不限制
    int		m_baseScore; // 基础分

    CardValue   m_hunCard[4];

    int		m_laiZiGangNum[4];		//	赖子杠的数量
    int		m_hongZhongGangNum[4];	//	红中杠的数量

    bool	m_first;

    int		m_affirmType[4];	//玩家确认某一色(对应 ENUM_AFFIRM_TYPE)
    int		m_option;			//客户端显示一色按钮项(对应 ENUM_AFFIRM_FLAG 按位操作)

    int		m_totalGangFen[DESK_USER_COUNT];	//所有玩家杠的总分数
    bool	m_liangNum[DESK_USER_COUNT];//大于0亮牌，等于0没亮牌
    int		m_lastOutCardPos;

    LMsgS2CDeskState() :LMsgSC(MSG_S_2_C_DESK_STATE)
    {
        m_state = 0;
        m_time = 0;
        m_flag = 0;
        m_zhuang = 0;
        m_dice1 = 0;
        m_dice2 = 0;
        m_dCount = 0;
        m_pos = INVAILD_POS;
        m_myPos = INVAILD_POS;

        memset( m_cardCount, 0, sizeof(m_cardCount) );
        memset( m_hType, 0, sizeof(m_hType) );
        memset( m_pCount, 0, sizeof(m_pCount) );
        memset( m_aCount, 0, sizeof(m_aCount) );
        memset( m_mCount, 0, sizeof(m_mCount) );
        memset( m_oCount, 0, sizeof(m_oCount) );
        memset( m_eCount, 0, sizeof(m_eCount) );
        memset( m_bChange, 0, sizeof(m_bChange) );
        memset( m_hCount, 0, sizeof(m_hCount) );
        memset( m_winList,0, sizeof(m_winList));
        memset( m_tCount, 0, sizeof(m_tCount));
        memset( m_laiZiGangNum, 0, sizeof(m_laiZiGangNum));
        memset( m_hongZhongGangNum, 0, sizeof(m_hongZhongGangNum));
        memset(m_affirmType, 0, sizeof(m_affirmType));

        m_changeOutTime = 0;
        m_opOutTime = 0;
        m_baseScore = 1;
        m_first = false;
        m_option = 0;

        memset(m_totalGangFen, 0, sizeof(m_totalGangFen));
        memset(m_liangNum, 0, sizeof(m_liangNum));
        m_lastOutCardPos = INVAILD_POS;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
        ReadMapData(obj, NAME_TO_STR(m_state), m_state);
        ReadMapData(obj, NAME_TO_STR(m_flag), m_flag);
        ReadMapData(obj, NAME_TO_STR(m_zhuang), m_zhuang);
        ReadMapData(obj, NAME_TO_STR(m_dice1), m_dice1);
        ReadMapData(obj, NAME_TO_STR(m_dice2), m_dice2);
        ReadMapData(obj, NAME_TO_STR(m_dCount), m_dCount);
        //ReadMapData(obj, NAME_TO_STR(m_myPos), m_myPos);

        msgpack::object array;
        ReadMapData(obj, NAME_TO_STR(m_myCard), array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            if ( m_pos >= 0 && m_pos < 4 )
            {
                m_cardCount[m_pos] = array.via.array.size;
            }
            for (size_t i = 0; i < array.via.array.size; ++i)
            {
                msgpack::object& pv = *(array.via.array.ptr + i);
                pv.convert(m_cardValue[i]);
            }
        }
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 56);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_state), m_state);

        WriteKeyValue(pack, NAME_TO_STR(m_flag), m_flag);
        WriteKeyValue(pack, NAME_TO_STR(m_zhuang), m_zhuang);
        WriteKeyValue(pack, NAME_TO_STR(m_dice1), m_dice1);
        WriteKeyValue(pack, NAME_TO_STR(m_dice2), m_dice2);
        WriteKeyValue(pack, NAME_TO_STR(m_dCount), m_dCount);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);

        std::vector<CardValue> vec;
        WriteKey(pack, "m_CardCount");
        std::vector<int> veci;
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_cardCount[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_winList");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_winList[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_hType");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_hType[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_myCard");
        vec.clear();
        if ( m_myPos >= 0 && m_myPos < 4 )
        {
            for (int i = 0; i < m_cardCount[m_myPos]; ++i)
            {
                vec.push_back(m_cardValue[i]);
            }
        }
        WriteKey(pack,vec);

        for (int i = 0; i < 4; ++i)
        {
            char name[32];
            vec.clear();
            if ( m_pCard[i][0].m_color != 0 || m_pCount[i] == 0 )
            {
                sprintf(name,"m_pCard%d",i);
                WriteKey(pack, name);
                for (int j = 0; j < m_pCount[i]; j+=3)
                {
                    vec.push_back(m_pCard[i][j]);
                }
            }
            else
            {
                sprintf(name,"m_phCard%d",i);
                WriteKey(pack, name);
                for (int j = 0; j < m_pCount[i]; j++)
                {
                    vec.push_back(m_phCard[i][j]);
                }
            }
            WriteKey(pack,vec);
        }

        for (int i = 0; i < 4; ++i)
        {
            char name[32];
            vec.clear();
            if ( m_aCard[i][0].m_color != 0 || m_aCount[i] == 0 )
            {
                sprintf(name,"m_aCard%d",i);
                WriteKey(pack, name);
                for (int j = 0; j < m_aCount[i]; j+=4)
                {
                    vec.push_back(m_aCard[i][j]);
                }
            }
            else
            {
                sprintf(name,"m_ahCard%d",i);
                WriteKey(pack, name);
                for (int j = 0; j < m_aCount[i]; j++)
                {
                    vec.push_back(m_ahCard[i][j]);
                }
            }
            WriteKey(pack,vec);
        }

        for (int i = 0; i < 4; ++i)
        {
            char name[32];
            vec.clear();
            if ( m_mCard[i][0].m_color != 0 || m_mCount[i] == 0 )
            {
                sprintf(name,"m_mCard%d",i);
                WriteKey(pack, name);
                for (int j = 0; j < m_mCount[i]; j+=4)
                {
                    vec.push_back(m_mCard[i][j]);
                }
            }
            else
            {
                sprintf(name,"m_mhCard%d",i);
                WriteKey(pack, name);
                for (int j = 0; j < m_mCount[i]; j++)
                {
                    vec.push_back(m_mhCard[i][j]);
                }
            }
            WriteKey(pack,vec);
        }

        for (int i = 0; i < 4; ++i)
        {
            char name[32];
            sprintf(name, "m_oCard%d", i);
            WriteKey(pack, std::string(name));
            vec.clear();
            for (int j = 0; j < m_oCount[i]; ++j)
            {
                vec.push_back(m_oCard[i][j]);
            }
            WriteKey(pack,vec);
        }

        for (int i = 0; i < 4; ++i)
        {
            char name[32];
            sprintf(name, "m_eCard%d", i);
            WriteKey(pack, std::string(name));
            vec.clear();
            for (int j = 0; j < m_eCount[i]; ++j)
            {
                vec.push_back(m_eCard[i][j]);
            }
            WriteKey(pack,vec);
        }

        WriteKey(pack, "m_winCard");
        vec.clear();
        for (int i = 0; i < 4; ++i)
        {
            vec.push_back(m_winCard[i]);
        }
        WriteKey(pack,vec);

        WriteKey(pack, "m_bchange");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_bChange[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_changeCard");
        vec.clear();
        for (int i = 0; i < 3; ++i)
        {
            vec.push_back(m_changeCard[i]);
        }
        WriteKey(pack,vec);

        for (int i = 0; i < 4; ++i)
        {
            char name[32];
            sprintf(name, "m_hCard%d", i);
            WriteKey(pack, std::string(name));
            vec.clear();
            for (int j = 0; j < m_hCount[i] && (j < 23); ++j)
            {
                vec.push_back(m_hCard[i][j]);
            }
            WriteKey(pack,vec);
        }

        for (int i = 0; i < 4; ++i)
        {
            char name[32];
            sprintf(name, "m_tCard%d", i);
            WriteKey(pack, std::string(name));
            vec.clear();
            for (int j = 0; j < m_tCount[i] && (j < 23); ++j)
            {
                vec.push_back(m_tCard[i][j]);
            }
            WriteKey(pack,vec);
        }

        WriteKey(pack, "m_hunCard");
        vec.clear();
        for (int i = 0; i < 4; ++i)
        {
            vec.push_back(m_hunCard[i]);
        }
        WriteKey(pack,vec);

        WriteKey(pack, "m_laiZiGangNum");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_laiZiGangNum[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_hongZhongGangNum");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_hongZhongGangNum[i]);
        }
        WriteKey(pack,veci);

        WriteKeyValue(pack, NAME_TO_STR(m_playtype), m_playtype);
        WriteKeyValue(pack, NAME_TO_STR(m_changeOutTime), m_changeOutTime);
        WriteKeyValue(pack, NAME_TO_STR(m_opOutTime), m_opOutTime);
        WriteKeyValue(pack, NAME_TO_STR(m_baseScore), m_baseScore);
        WriteKeyValue(pack, NAME_TO_STR(m_first), m_first);
        WriteKey(pack, "m_affirmType");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_affirmType[i]);
        }
        WriteKey(pack, veci);
        WriteKeyValue(pack, NAME_TO_STR(m_option), m_option);
        veci.clear();
        for (Lint i = 0; i < DESK_USER_COUNT; ++i)
        {
            veci.push_back(m_totalGangFen[i]);
        }
        WriteKeyValue(pack, NAME_TO_STR(m_totalGangFen), veci);
        veci.clear();
        for (Lint i = 0; i < DESK_USER_COUNT; ++i)
        {
            veci.push_back(m_liangNum[i]);
        }
        WriteKeyValue(pack, NAME_TO_STR(m_liangNum), veci);
        WriteKeyValue(pack, NAME_TO_STR(m_lastOutCardPos), m_lastOutCardPos);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CDeskState>(); }
};

//////////////////////////////////////////////////////////////////////////
//客户端请求准备
struct LMsgC2SUserReady:public LMsgSC
{
    int m_pos;
    int m_state;		//0-取消准备 1-准备

    LMsgC2SUserReady() :LMsgSC(MSG_C_2_S_READY)
    {
        m_pos = INVAILD_POS;
        m_state = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
        ReadMapData(obj, NAME_TO_STR(m_state), m_state);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgC2SUserReady>();}
};

//////////////////////////////////////////////////////////////////////////
//服务器返回玩家准备
struct LMsgS2CUserReady:public LMsgSC
{
    int			m_pos;//准备玩家的位置
    int			m_state;		//0-取消准备 1-准备

    LMsgS2CUserReady() :LMsgSC(MSG_S_2_C_READY)
    {
        m_pos = INVAILD_POS;
        m_state = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_state), m_state);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CUserReady>();}
};


////////////////////////////////////////////////////////////////////
//客户端忙碌状态（是否忙碌）
struct LMsgC2SUserBusy:public LMsgSC
{
    int m_bBusy;	//用户忙碌状态
    int m_nPos;		//用户的位置
    LMsgC2SUserBusy() :LMsgSC(MSG_C_2_S_BUSY)
    {
        m_bBusy = 0;
        m_nPos = INVAILD_POS;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_bBusy), m_bBusy);
        ReadMapData(obj, NAME_TO_STR(m_nPos), m_nPos);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_bBusy), m_bBusy);
        WriteKeyValue(pack, NAME_TO_STR(m_nPos), m_nPos);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgC2SUserBusy>(); }
};

//服务器广播用户忙碌状态
struct LMsgS2CUserBusy :public LMsgSC
{
    int m_bBusy;	//用户忙碌状态
    int m_nPos;		//用户的位置
    LMsgS2CUserBusy() :LMsgSC(MSG_S_2_C_BUSY)
    {
        m_bBusy = 0;
        m_nPos = INVAILD_POS;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_bBusy), m_bBusy);
        ReadMapData(obj, NAME_TO_STR(m_nPos), m_nPos);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_bBusy), m_bBusy);
        WriteKeyValue(pack, NAME_TO_STR(m_nPos), m_nPos);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgS2CUserBusy>(); }
};

//////////////////////////////////////////////////////////////////////////
//服务器返回玩家准备
struct LMsgS2CUserOnlineInfo:public LMsgSC
{
    int			m_flag;//0-掉线了，1-回来了
    int			m_pos;//

    LMsgS2CUserOnlineInfo() :LMsgSC(MSG_S_2_C_USER_ONLINE_INFO)
    {
        m_flag = 0;
        m_pos = INVAILD_POS;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_flag), m_flag);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CUserOnlineInfo>(); }
};
//////////////////////////////////////////////////////////////////////////
//服务器广播游戏开始

struct LMsgS2CPlayStart:public LMsgSC
{
    int		m_zhuang;
    int		m_dCount;//桌面剩余的牌
    int		m_cardCount[4];//0-3号玩家手上的牌数量
    int		m_pos;
    int     m_hType[4];
    int		m_dice1;
    int		m_dice2;

    //我手上的牌
    CardValue	m_cardValue[14];
    int		m_dingQue[4];

    CardValue   m_hunCard[4];

    int		m_laiZiGangNum[4];		//	赖子杠的数量
    int		m_hongZhongGangNum[4];	//	红中杠的数量

    bool		m_first;

    LMsgS2CPlayStart() :LMsgSC(MSG_S_2_C_START)
    {
        memset(m_hType,0,sizeof(m_hType));
        m_zhuang = INVAILD_POS;
        m_dCount = 0;
        m_pos = INVAILD_POS;
        m_first = false;
        m_dice1 = 0;
        m_dice2 = 0;
        for ( int i = 0; i < 4; ++i )
        {
            m_cardCount[i] = 0;
            m_dingQue[i] = 0;
            m_laiZiGangNum[i] = 0;
            m_hongZhongGangNum[i] = 0;
        }
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        msgpack::object array;
        ReadMapData(obj, NAME_TO_STR(m_myCard), array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            //m_cardCount[0] = array.via.array.size;
            for (size_t i = 0; i < array.via.array.size; ++i)
            {
                msgpack::object& pv = *(array.via.array.ptr + i);
                pv.convert(m_cardValue[i]);
            }
        }

        msgpack::object array1;
        ReadMapData(obj, NAME_TO_STR(m_CardCount), array1);
        if (array1.type == msgpack::v1::type::ARRAY)
        {
            for (size_t i = 0; i < array1.via.array.size; ++i)
            {
                msgpack::object& pv = *(array1.via.array.ptr + i);
                pv.convert(m_cardCount[i]);
            }
        }

        ReadMapData(obj, NAME_TO_STR(m_zhuang), m_zhuang);
        ReadMapData(obj, NAME_TO_STR(m_dCount), m_dCount);
        ReadMapData(obj, NAME_TO_STR(m_dice1), m_dice1);
        ReadMapData(obj, NAME_TO_STR(m_dice2), m_dice2);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 13);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);

        WriteKey(pack, "m_myCard");
        std::vector<CardValue>vce;
        if ( m_pos >= 0 && m_pos < 4 )
        {
            for (int i = 0; i < m_cardCount[m_pos]; ++i)
            {
                vce.push_back(m_cardValue[i]);
            }
        }
        WriteKey(pack,vce);

        WriteKey(pack, "m_CardCount");
        std::vector<int> veci;
        for(int i = 0 ; i < 4;++i)
        {
            veci.push_back(m_cardCount[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_hType");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_hType[i]);
        }
        WriteKey(pack,veci);

        veci.clear();
        WriteKey(pack, "m_dingQue");
        for(int i = 0 ; i < 4;++i)
        {
            veci.push_back(m_dingQue[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_hunCard");
        vce.clear();
        for (int i = 0; i < 4; ++i)
        {
            vce.push_back(m_hunCard[i]);
        }
        WriteKey(pack,vce);

        veci.clear();
        WriteKey(pack, "m_laiZiGangNum");
        for(int i = 0 ; i < 4; ++i)
        {
            veci.push_back(m_laiZiGangNum[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_hongZhongGangNum");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_hongZhongGangNum[i]);
        }
        WriteKey(pack,veci);

        WriteKeyValue(pack, NAME_TO_STR(m_dCount),m_dCount);
        WriteKeyValue(pack, NAME_TO_STR(m_zhuang), m_zhuang);
        WriteKeyValue(pack, NAME_TO_STR(m_first), m_first);
        WriteKeyValue(pack, NAME_TO_STR(m_dice1), m_dice1);
        WriteKeyValue(pack, NAME_TO_STR(m_dice2), m_dice2);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CPlayStart>();}
};

//////////////////////////////////////////////////////////////////////////
//服务器广播玩家摸牌

struct ThinkData
{
    int	m_type;
    std::vector<CardValue> m_card;

    ThinkData()
    {
        m_type = 0;
    }

    MSGPACK_DEFINE(m_type, m_card);
};

struct TingData
{
    struct TingCard
    {
        CardValue m_card;
        int m_hufan;

        MSGPACK_DEFINE(m_card, m_hufan);
    };

    CardValue	m_card;
    std::vector<TingCard> m_tingcard;

    MSGPACK_DEFINE(m_card, m_tingcard);
};

struct LMsgS2COutCard:public LMsgSC
{
    int		m_pos;//当前出牌中的玩家
    int		m_time;//出牌倒计时
    int		m_deskCard;//桌面剩余的牌
    int		m_flag;//当前是否需要摸牌，0-需要，1-不需要
    int		m_gang;//是否杠过，0-没有，1-杠过
    int		m_end;//是否海底 0-不是，1-是
    CardValue	m_curCard;//摸上来的牌，这个只有 m_pos == mypos m_flag == 0 的时候才有
    bool	m_bOnlyHu;		// 最后四张有胡必胡
    int		m_score[4];//每家分数

    std::vector<ThinkData> m_think;//思考的内容

    //std::vector<TingData> m_ting;	// 听牌数据

    std::vector<CardValue> m_ting;

    bool        m_close;

    LMsgS2COutCard() :LMsgSC(MSG_S_2_C_OUT_CARD)
    {
        m_pos = INVAILD_POS;
        m_time = 0;
        m_deskCard = 0;
        m_flag = 0;
        m_gang = 0;
        m_end = 0;
        m_bOnlyHu = false;
        m_ting.clear();
        m_close = false;
        memset(m_score, 0, sizeof(m_score));
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 14);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_time), m_time);
        WriteKeyValue(pack, NAME_TO_STR(m_dCount), m_deskCard);
        m_curCard.Write(pack);
        WriteKeyValue(pack, NAME_TO_STR(m_flag), m_flag);
        WriteKeyValue(pack, NAME_TO_STR(m_think), m_think);
        WriteKeyValue(pack, NAME_TO_STR(m_ting), m_ting);
        WriteKeyValue(pack, NAME_TO_STR(m_gang), m_gang);
        WriteKeyValue(pack, NAME_TO_STR(m_end), m_end);
        WriteKeyValue(pack, NAME_TO_STR(m_bOnlyHu), m_bOnlyHu);

        //WriteKey(pack,m_ting);

        WriteKey(pack, "m_score");
        std::vector<int> vec;
        for (int i = 0; i < 4; ++i)
        {
            vec.push_back(m_score[i]);
        }
        WriteKey(pack,vec);

        WriteKeyValue(pack, NAME_TO_STR(m_close), m_close);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2COutCard>(); }
};

//通知客户端摸牌
struct LMsgS2CGetCard :public LMsgSC
{
    int		m_pos;//摸牌人的位置
    int		m_deskCard;//桌面剩余的牌的数量
    CardValue	m_curCard;//摸上来的牌

    LMsgS2CGetCard() :LMsgSC(MSG_S_2_C_GET_CARD)
    {
        m_pos = INVAILD_POS;
        m_deskCard = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 5);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_dCount), m_deskCard);
        m_curCard.Write(pack);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgS2COutCard>(); }
};

//////////////////////////////////////////////////////////////////////////
//客户端摸牌后的操作
struct LMsgC2SUserPlay:public LMsgSC
{
    ThinkData m_thinkInfo;

    LMsgC2SUserPlay() :LMsgSC(MSG_C_2_S_PLAY_CARD)
    {
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_type), m_thinkInfo.m_type);

        msgpack::object array;
        ReadMapData(obj, NAME_TO_STR(m_think), array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            //int m_count = array.via.array.size;
            for (size_t i = 0; i < array.via.array.size; ++i)
            {
                CardValue v;
                msgpack::object& obj =  *(array.via.array.ptr+i);
                obj.convert(v);
                m_thinkInfo.m_card.push_back(v);
            }
        }
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_type), m_thinkInfo.m_type);
        WriteKeyValue(pack, NAME_TO_STR(m_think), m_thinkInfo.m_card);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgC2SUserPlay>();}
};

//////////////////////////////////////////////////////////////////////////
//服务器返回操作结果
struct LMsgS2CUserPlay:public LMsgSC
{
    enum
    {
        OutSuccessed = 0,    //0:无错误
        OutNotYourTurn = 1,  //1:不应该由你出牌(当前出牌人由m_pos指定)
        OutOtherTing = 2,    //2:在卡心五中使用,出的牌是别人廊起听的牌,不允许出（除非玩家也廊起了）
        OutTingError = 3,    //3:在卡心五中使用,已经廊起的玩家，打出的牌不是摸到的牌
        OutIsHuncard = 4,    //4:湖北黄冈晃晃 赖子牌不能打出
    };

    int		m_errorCode;   //0: success
    int		m_flag;////0-自摸，1-出牌,2-暗牌,3-明杠(自己碰过之后),4-提
    int		m_qiangGangflag; //标示是否该杠\补操作被抢 0-未被抢/无需等待胡牌 1-被抢 2-等待胡牌
    int		m_pos;//出牌玩家的位置
    ThinkData	m_card;
    std::vector<int>	m_hu;
    int		m_cardCount;			//手牌数量
    std::vector<CardValue> m_cardValue;	//推倒的手牌
    CardValue m_huCard;					//自摸胡的那张牌
    int		m_rewardCardNum;		//胡牌奖励房卡数
    int		m_rewardHu;				//胡牌被奖励的番型

    LMsgS2CUserPlay() :LMsgSC(MSG_S_2_C_PLAY_CARD)
    {
        m_errorCode = -1;
        m_flag = 0;
        m_qiangGangflag=2;
        m_pos = INVAILD_POS;
        m_cardCount = 0;
        m_rewardCardNum = 0;
        m_rewardHu = HU_NULL;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_errorCode), m_errorCode);
        ReadMapData(obj, NAME_TO_STR(m_flag), m_flag);
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
        ReadMapData(obj, NAME_TO_STR(m_card), m_card);
        ReadMapData(obj, NAME_TO_STR(m_hu), m_hu);
        ReadMapData(obj, NAME_TO_STR(m_cardCount), m_cardCount);
        ReadMapData(obj, NAME_TO_STR(m_cardValue), m_cardValue);
        m_huCard.Read(obj);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 13);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
        WriteKeyValue(pack, NAME_TO_STR(m_type), m_card.m_type);
        WriteKeyValue(pack, NAME_TO_STR(m_qianggangflag), m_qiangGangflag);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_think), m_card.m_card);
        WriteKeyValue(pack, NAME_TO_STR(m_hu), m_hu);
        WriteKeyValue(pack, NAME_TO_STR(m_cardCount), m_cardCount);
        WriteKeyValue(pack, NAME_TO_STR(m_cardValue), m_cardValue);
        WriteKeyValue(pack, NAME_TO_STR(m_rewardCardNum), m_rewardCardNum);
        WriteKeyValue(pack, NAME_TO_STR(m_rewardHu), m_rewardHu);
        m_huCard.Write(pack);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CUserPlay>();}
};

//////////////////////////////////////////////////////////////////////////
//服务器发送玩家思考信息

struct LMsgS2CThink:public LMsgSC
{
    int				m_time;//思考倒计时
    int				m_flag;//0-等待其他玩家思考中，1-当前我正在思考
    CardValue		m_card;
    bool			m_bOnlyHu;		// 最后四张有胡必胡
    std::vector<ThinkData>	m_think;

    LMsgS2CThink() :LMsgSC(MSG_S_2_C_USER_THINK)
    {
        m_time = 0;
        m_flag = 0;
        m_bOnlyHu = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_msgId), m_msgId);
        ReadMapData(obj, NAME_TO_STR(m_time), m_time);
        ReadMapData(obj, NAME_TO_STR(m_flag), m_flag);
        ReadMapData(obj, NAME_TO_STR(m_think), m_think);
        m_card.Read(obj);
        //
        msgpack::object array1;
        ReadMapData(obj, NAME_TO_STR(m_think), array1);
        if (array1.type == msgpack::v1::type::ARRAY)
        {
            for (size_t i = 0; i < array1.via.array.size; ++i)
            {
                ThinkData v;
                msgpack::object& pv = *(array1.via.array.ptr + i);
                pv.convert(v);
                m_think.push_back(v);
            }
        }
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 7);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_time), m_time);
        WriteKeyValue(pack, NAME_TO_STR(m_flag), m_flag);
        WriteKeyValue(pack, NAME_TO_STR(m_think), m_think);
        WriteKeyValue(pack, NAME_TO_STR(m_bOnlyHu), m_bOnlyHu);
        m_card.Write(pack);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CThink>();}
};

//////////////////////////////////////////////////////////////////////////
//客户端发送思考操作结果
struct LMsgC2SUserOper:public LMsgSC
{
    ThinkData m_think;
    //int m_think;//思考结果0-过，1-胡，2-杠，3-碰,4-补

    LMsgC2SUserOper() :LMsgSC(MSG_C_2_S_USER_OPER){}

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_type), m_think.m_type);

        msgpack::object array;
        ReadMapData(obj, NAME_TO_STR(m_think), array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            //int m_count = array.via.array.size;
            for (size_t i = 0; i < array.via.array.size; ++i)
            {
                CardValue v;
                msgpack::object& obj =  *(array.via.array.ptr+i);
                obj.convert(v);
                m_think.m_card.push_back(v);
            }
        }
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_type), m_think.m_type);
        WriteKeyValue(pack, NAME_TO_STR(m_think), m_think.m_card);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgC2SUserOper>();}
};

//////////////////////////////////////////////////////////////////////////
//服务广播操作结果
struct LMsgS2CUserOper:public LMsgSC
{
    int		m_errorCode;//0-成功，1-失败
    int		m_pos;//pos=我自己的时候，需要根据类型响应不同的操作
    ThinkData	m_think;
    CardValue	m_card; //玩家碰，杠，胡的张牌
    std::vector<int>	m_hu;
    int     m_hType;                //1代表抢杠胡，否则为其他
    int		m_cardCount;			//手牌数量
    std::vector<CardValue> m_cardValue;	//推倒的手牌
    int		m_rewardCardNum;		//胡牌奖励房卡数
    int		m_rewardHu;				//胡牌被奖励的牌型

    LMsgS2CUserOper() :LMsgSC(MSG_S_2_C_USER_OPER)
    {
        m_errorCode = -1;
        m_pos = INVAILD_POS;
        m_hType = 0;
        m_cardCount = 0;
        m_rewardCardNum = 0;
        m_rewardHu = HU_NULL;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_errorCode), m_errorCode);
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
        ReadMapData(obj, NAME_TO_STR(m_think), m_think);
        m_card.Read(obj);
        ReadMapData(obj, NAME_TO_STR(m_hu), m_hu);
        ReadMapData(obj, NAME_TO_STR(m_hType), m_hType);
        ReadMapData(obj, NAME_TO_STR(m_cardCount), m_cardCount);
        ReadMapData(obj, NAME_TO_STR(m_cardValue), m_cardValue);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 12);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_think), m_think);
        m_card.Write(pack);
        WriteKeyValue(pack, NAME_TO_STR(m_hu), m_hu);
        WriteKeyValue(pack, NAME_TO_STR(m_hType), m_hType);
        WriteKeyValue(pack, NAME_TO_STR(m_cardCount), m_cardCount);
        WriteKeyValue(pack, NAME_TO_STR(m_cardValue), m_cardValue);
        WriteKeyValue(pack, NAME_TO_STR(m_rewardCardNum), m_rewardCardNum);
        WriteKeyValue(pack, NAME_TO_STR(m_rewardHu), m_rewardHu);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CUserOper>();}
};

//////////////////////////////////////////////////////////////////////////
//服务器广播游戏结束
struct LMsgS2CGameOver:public LMsgSC
{
    int		m_result;//0-自摸，1-点炮，2-慌庄
    int		m_win[4];//4家胡牌情况，0-没胡，1-自摸，2-收炮，3-点炮, 7-有 叫
    int		m_score[4];//东 南 西 北 各家输赢积分，这里需要更新玩家积分
    int		m_total_score[4];//包括前面所有局累计的分数
    int		m_coins[4];//金币
    int		m_totalcoins[4];//玩家结算完的总金币
    int		m_agang[4];//暗杠数量
    int		m_mgang[4];//明杠数量 别人点的
    int     m_mbgang[4];//巴杠(补杠)的数量
    int		m_dgang[4];//点别人杠的数量
    int     m_gsgang[4];           //HB过手杠数量
    int		m_ming[4];             //HB是否明牌
    std::vector<int>		m_hu[4];//胡牌的数组
    std::vector<int>		m_dian[4];		// 点炮类型
    std::vector<int>        m_playType;     //桌子类型

    std::vector<int>		m_myFan[4];     //4x4 赢的玩家(绵阳为番数)(宜宾为分数)
    int                     m_piaoShu[4];   //HB漂
    CardValue				m_hunCard[3];

    //四家手上的牌
    int		m_count[4];
    CardValue	m_card[4][14];

    std::vector<CardValue>   m_hucard[4];

    int		m_fan[4];
    int		m_gen[4];
    int		m_winList[4];

    int		m_checkTing[4];		// 查听
    int		m_rewardCardNum[4];		//胡牌奖励房卡数
    int		m_rewardHu[4];			//胡牌被奖励的牌型
    int		m_changeScore[4];		//买牌的分
    int		m_laiZiGangNum[4];		//赖子杠的数量
    int		m_hongZhongGangNum[4];	//红中杠的数量

    bool	m_fengDing[4];			//封顶玩家
    bool	m_yangGuangDing[4];		//阳光顶玩家
    CardValue	m_xiPai[3];			//喜牌

    int		m_end;
    int		m_maxFan;	// 封顶番数
    int		m_baoPos;				//包牌玩家的位置
    CardValue	m_agangcard[4][4];//一个玩家最多只能暗杠4次

    LMsgS2CGameOver() :LMsgSC(MSG_S_2_C_GAME_OVER)
    {
        m_result = -1;
        for ( int i = 0; i < 4; ++i )
        {
            m_win[i] = 0;
            m_score[i] = 0;
            m_total_score[i] = 0;
            m_coins[i] = 0;
            m_totalcoins[i] = 0;
            m_agang[i] = 0;
            m_mgang[i] = 0;
            m_mbgang[i] = 0;
            m_dgang[i] = 0;
            m_gsgang[i] = 0;
            m_ming[i] = 0;
            m_piaoShu[i] = 0;
            m_count[i] = 0;

            m_fan[i] = 0;
            m_gen[i] = 0;
            m_winList[i] = 0;
            m_checkTing[i] = 0;

            m_rewardCardNum[i] = 0;
            m_rewardHu[i] = HU_NULL;
            m_changeScore[i] = 0;
            m_laiZiGangNum[i] = 0;
            m_hongZhongGangNum[i] = 0;
            m_fengDing[i] = false;
            m_yangGuangDing[i] = false;
        }
        m_end = 0;
        m_maxFan = 0;
        m_baoPos = INVAILD_POS;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 55);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
        WriteKeyValue(pack, NAME_TO_STR(m_end), m_end);

        WriteKey(pack, "m_win");
        std::vector<int> veci;
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_win[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_piaoShu");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_piaoShu[i]);
        }
        WriteKey(pack,veci);

        for(int i = 0 ; i < 4; ++i)
        {
            char name[32] = {0};
            sprintf_s(name,sizeof(name),"m_hu%d",i+1);
            WriteKey(pack, name);
            WriteKey(pack,m_hu[i]);
        }

        for(int i = 0 ; i < 4; ++i)
        {
            char name[32] = {0};
            sprintf_s(name,sizeof(name),"m_myFan%d",i+1);
            WriteKey(pack, name);
            WriteKey(pack,m_myFan[i]);
        }

        for(int i = 0 ; i < 4; ++i)
        {
            char name[32] = {0};
            sprintf_s(name,sizeof(name),"m_dian%d",i+1);
            WriteKey(pack, name);
            WriteKey(pack,m_dian[i]);
        }

        WriteKey(pack, "m_gsgang");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_gsgang[i]);
        }
        WriteKey(pack,veci);

        for(int i = 0 ; i < 4; ++i)
        {
            char name[32] = {0};
            sprintf_s(name,sizeof(name),"m_hucards%d",i+1);
            WriteKeyValue(pack, name, m_hucard[i]);
        }

        WriteKey(pack, "m_playType");  // add for zzg
        WriteKey(pack,m_playType);

        WriteKey(pack, "m_agang");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_agang[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_ming");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_ming[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_mgang");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_mgang[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_mbgang");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_mbgang[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_dgang");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_dgang[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_score");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_score[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_total_score");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_total_score[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_coins");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_coins[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "m_totalcoins");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_totalcoins[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack, "array0");
        std::vector<CardValue> vec;
        for (int i = 0; i < m_count[0]; ++i)
        {
            vec.push_back(m_card[0][i]);
        }
        WriteKey(pack, vec);

        WriteKey(pack, "array1");
        vec.clear();
        for (int i = 0; i < m_count[1]; ++i)
        {
            vec.push_back(m_card[1][i]);
        }
        WriteKey(pack, vec);

        WriteKey(pack, "array2");
        vec.clear();
        for (int i = 0; i < m_count[2]; ++i)
        {
            vec.push_back(m_card[2][i]);
        }
        WriteKey(pack, vec);

        WriteKey(pack, "array3");
        vec.clear();
        for (int i = 0; i < m_count[3]; ++i)
        {
            vec.push_back(m_card[3][i]);
        }
        WriteKey(pack, vec);


        WriteKey(pack, "m_fan");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_fan[i]);
        }
        WriteKey(pack, veci);

        WriteKey(pack, "m_gen");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_gen[i]);
        }
        WriteKey(pack, veci);

        WriteKey(pack, "m_winList");
        veci.clear();
        for (size_t i = 0; i < 4; ++i)
        {
            veci.push_back(m_winList[i]);
        }
        WriteKey(pack, veci);

        WriteKey(pack, "m_checkTing");
        veci.clear();
        for (size_t i = 0; i < 4; ++i)
        {
            veci.push_back(m_checkTing[i]);
        }
        WriteKey(pack, veci);

        WriteKey(pack, "m_rewardCardNum");
        veci.clear();
        for (size_t i = 0; i < 4; ++i)
        {
            veci.push_back(m_rewardCardNum[i]);
        }
        WriteKey(pack, veci);

        WriteKey(pack, "m_rewardHu");
        veci.clear();
        for (size_t i = 0; i < 4; ++i)
        {
            veci.push_back(m_rewardHu[i]);
        }
        WriteKey(pack, veci);

        std::vector<CardValue> vecs;
        WriteKey(pack, "m_hunCard");
        for (int i = 0; i < 3; ++i)
        {
            vecs.push_back(m_hunCard[i]);
        }
        WriteKey(pack,vecs);

        WriteKeyValue(pack, NAME_TO_STR(m_maxFan), m_maxFan);

        WriteKey(pack, "m_changeScore");
        veci.clear();
        for (size_t i = 0; i < 4; ++i)
        {
            veci.push_back(m_changeScore[i]);
        }
        WriteKey(pack, veci);

        WriteKey(pack, "m_laiZiGangNum");
        veci.clear();
        for (size_t i = 0; i < 4; ++i)
        {
            veci.push_back(m_laiZiGangNum[i]);
        }
        WriteKey(pack, veci);

        WriteKey(pack, "m_hongZhongGangNum");
        veci.clear();
        for (int i = 0; i < 4; ++i)
        {
            veci.push_back(m_hongZhongGangNum[i]);
        }
        WriteKey(pack,veci);

        WriteKeyValue(pack, NAME_TO_STR(m_baoPos), m_baoPos);

        std::vector<bool> vecb;
        for (int i = 0; i < 4; ++i)
        {
            vecb.push_back(m_fengDing[i]);
        }
        WriteKeyValue(pack, NAME_TO_STR(m_fengDing), vecb);

        vecb.clear();
        for (int i = 0; i < 4; ++i)
        {
            vecb.push_back(m_yangGuangDing[i]);
        }
        WriteKeyValue(pack, NAME_TO_STR(m_yangGuangDing), vecb);

        vecs.clear();
        WriteKey(pack, "m_xiPai");
        for (int i = 0; i < 3; ++i)
        {
            vecs.push_back(m_xiPai[i]);
        }
        WriteKey(pack, vecs);

        WriteKey(pack, "m_agangcard1");
        vec.clear();
        for (Lint i = 0; i < m_agang[0] && i < 4; ++i)
        {
            vec.push_back(m_agangcard[0][i]);
        }
        WriteKey(pack, vec);

        WriteKey(pack, "m_agangcard2");
        vec.clear();
        for (Lint i = 0; i < m_agang[1] && i < 4; ++i)
        {
            vec.push_back(m_agangcard[1][i]);
        }
        WriteKey(pack, vec);

        WriteKey(pack, "m_agangcard3");
        vec.clear();
        for (Lint i = 0; i < m_agang[2] && i < 4; ++i)
        {
            vec.push_back(m_agangcard[2][i]);
        }
        WriteKey(pack, vec);

        WriteKey(pack, "m_agangcard4");
        vec.clear();
        for (Lint i = 0; i < m_agang[3] && i < 4; ++i)
        {
            vec.push_back(m_agangcard[3][i]);
        }
        WriteKey(pack, vec);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CGameOver>();}
};


//////////////////////////////////////////////////////////////////////////
//客户端请求退出房间
struct LMsgC2SUserOutRoom:public LMsgSC
{
    int m_pos;//准备玩家的位置

    LMsgC2SUserOutRoom():LMsgSC(MSG_C_2_S_USER_OUT_ROOM)
    {
        m_pos = INVAILD_POS;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgC2SUserOutRoom>();}
};

//////////////////////////////////////////////////////////////////////////
//服务返回退出结果
struct LMsgS2CUserOutRoom:public LMsgSC
{
    int		m_errorCode;//0-成功，1-失败
    int		m_pos;

    LMsgS2CUserOutRoom():LMsgSC(MSG_S_2_C_USER_OUT_ROOM)
    {
        m_errorCode = -1;
        m_pos = INVAILD_POS;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
        ReadMapData(obj, NAME_TO_STR(m_errorCode), m_errorCode);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CUserOutRoom>();}
};

struct LMsgC2SUserAIOper:public LMsgSC
{
    LMsgC2SUserAIOper() :LMsgSC(MSG_C_2_S_USER_AIOPER)
    {
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 1);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgC2SUserAIOper>();}
};

//客户端摸牌后的操作
struct LMsgS2CUserAIOper:public LMsgSC
{
    ThinkData m_thinkInfo;

    LMsgS2CUserAIOper() :LMsgSC(MSG_S_2_C_USER_AIOPER)
    {
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_type), m_thinkInfo.m_type);
        WriteKeyValue(pack, NAME_TO_STR(m_card), m_thinkInfo.m_card);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CUserAIOper>();}
};

//////////////////////////////////////////////////////////////////////////
//服务器发送起手胡消息

struct StartHuInfo
{
    int	m_type;
    std::vector<CardValue> m_card;

    StartHuInfo()
    {
        m_type = 0;
    }

    MSGPACK_DEFINE(m_type, m_card);
};

struct LMsgS2CUserStartHu:public LMsgSC
{
    std::vector<StartHuInfo> m_card;

    LMsgS2CUserStartHu():LMsgSC(MSG_S_2_C_START_HU){}

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_card), m_card);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CUserStartHu>();}
};

//////////////////////////////////////////////////////////////////////////
//服务返回退出结果
struct LMsgC2SUserStartHuSelect:public LMsgSC
{
    int		m_type;
    std::vector<CardValue> m_card;

    LMsgC2SUserStartHuSelect():LMsgSC(MSG_C_2_S_START_HU_SELECT)
    {
        m_type = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {

        ReadMapData(obj, NAME_TO_STR(m_type), m_type);
        //ReadMapData(obj, NAME_TO_STR(m_card), m_card);
        msgpack::object array;
        ReadMapData(obj, NAME_TO_STR(m_card), array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            //int m_count = array.via.array.size;
            for (size_t i = 0; i < array.via.array.size; ++i)
            {
                CardValue v;
                msgpack::object& obj =  *(array.via.array.ptr+i);
                obj.convert(v);
                m_card.push_back(v);
            }
        }
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgC2SUserStartHuSelect>();}
};

//////////////////////////////////////////////////////////////////////////
//服务广播玩家操作
struct LMsgS2CUserStartHuSelect:public LMsgSC
{
    int		m_pos;
    int		m_type;
    std::vector<CardValue> m_card;

    LMsgS2CUserStartHuSelect():LMsgSC(MSG_S_2_C_START_HU_SELECT)
    {
        m_pos = INVAILD_POS;
        m_type = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 4);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_card), m_card);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CUserStartHuSelect>();}
};

//////////////////////////////////////////////////////////////////////////
//玩家杠牌
struct LMsgS2CUserGangCard:public LMsgSC
{
    int		m_pos;
    int		m_flag;//0-没有自摸，1-自摸
    std::vector<CardValue> m_card;

    LMsgS2CUserGangCard():LMsgSC(MSG_S_2_C_GANG_RESULT)
    {
        m_pos = INVAILD_POS;
        m_flag = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 4);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_flag), m_flag);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_card), m_card);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CUserGangCard>();}
};

//////////////////////////////////////////////////////////////////////////
//服务器广播玩家选择海底牌
struct LMsgS2CUserEndCardSelect:public LMsgSC
{
    int		m_pos;
    int		m_time;

    LMsgS2CUserEndCardSelect():LMsgSC(MSG_S_2_C_END_CARD_SELECT)
    {
        m_pos = INVAILD_POS;
        m_time = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_time), m_time);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CUserEndCardSelect>();}
};

//////////////////////////////////////////////////////////////////////////
//客户的选择海底
struct LMsgC2SUserEndCardSelect:public LMsgSC
{
    int		m_flag;//0-不要，1-要

    LMsgC2SUserEndCardSelect():LMsgSC(MSG_C_2_S_END_SELECT)
    {
        m_flag = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_flag), m_flag);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgC2SUserEndCardSelect>();}
};


//////////////////////////////////////////////////////////////////////////
//服务器广播玩家摸海底
struct LMsgS2CUserEndCardGet:public LMsgSC
{
    int		m_pos;	//位置
    int		m_flag;//是否自摸 0-没有，1-自摸
    CardValue	m_card;

    LMsgS2CUserEndCardGet():LMsgSC(MSG_S_2_C_END_GET)
    {
        m_pos = INVAILD_POS;
        m_flag = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 5);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_flag), m_flag);
        m_card.Write(pack);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CUserEndCardGet>();}
};

//玩家杠牌
struct LMsgS2CRemoveQiangGangCard:public LMsgSC
{
    int		m_pos;
    int		m_flag;		//0-手牌， 1未用
    CardValue	m_card;

    LMsgS2CRemoveQiangGangCard():LMsgSC(MSG_S_2_C_REMOVE_QIANGGANG_CARD)
    {
        m_pos = INVAILD_POS;
        m_flag = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 5);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_flag), m_flag);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        m_card.Write(pack);
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgS2CRemoveQiangGangCard>();}
};

//////////////////////////////////////////////////////////////////////////
//客户端请求说话
struct LMsgC2SUserSpeak:public LMsgSC
{
    int		m_type;//说话类型，0-表情，1-语音,2-文字
    int		m_id;	//表情id
    std::string		m_musicUrl; //语音地址链接
    std::string		m_msg;//聊天文字

    LMsgC2SUserSpeak() :LMsgSC(MSG_C_2_S_USER_SPEAK)
    {
        m_type = -1;
        m_id = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_type), m_type);
        ReadMapData(obj, NAME_TO_STR(m_id), m_id);
        ReadMapData(obj, NAME_TO_STR(m_musicUrl), m_musicUrl);
        ReadMapData(obj, NAME_TO_STR(m_msg), m_msg);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 5);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);
        WriteKeyValue(pack, NAME_TO_STR(m_id), m_id);
        WriteKeyValue(pack, NAME_TO_STR(m_musicUrl), m_musicUrl);
        WriteKeyValue(pack, NAME_TO_STR(m_msg), m_msg);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SUserSpeak>(); }
};

//////////////////////////////////////////////////////////////////////////
//服务广播操作结果
struct LMsgS2CUserSpeak:public LMsgSC
{
    int		m_userId;//说话人id
    int		m_pos;//说话人的位置
    int		m_type;//说话类型
    int		m_id;//类型id
    std::string		m_musicUrl;//语音url链接
    std::string		m_msg;//聊天文字

    LMsgS2CUserSpeak() :LMsgSC(MSG_S_2_C_USER_SPEAK)
    {
        m_userId = 0;
        m_pos = INVAILD_POS;
        m_type = -1;
        m_id = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 7);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);
        WriteKeyValue(pack, NAME_TO_STR(m_id), m_id);
        WriteKeyValue(pack, NAME_TO_STR(m_musicUrl), m_musicUrl);
        WriteKeyValue(pack, NAME_TO_STR(m_msg), m_msg);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CUserSpeak>(); }
};

//////////////////////////////////////////////////////////////////////////
//胡牌通知消息
struct LMsgS2CHuCard :public LMsgSC
{
    int					m_ting;	//是否听牌
    std::vector<CardValue>	m_card;	//胡牌列表

    LMsgS2CHuCard() :LMsgSC(MSG_S_2_C_HU_CARD)
    {
        m_ting = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_ting), m_ting);
        WriteKeyValue(pack, NAME_TO_STR(m_card), m_card);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CHuCard>(); }
};

//////////////////////////////////////////////////////////////////////////
//客户端请求创建桌子
struct LMsgC2SCreateDesk :public LMsgSC
{
    int		m_flag;					//房间类型，1-2圈，2-四圈，3-8圈
    int		m_payment;				//付费方式，1 - 房主支付，2 - 平均支付， 3 - 大赢家支付
    int		m_nIPAntiCheat;	//IP防作弊
    int		m_nGPSEarlyWarning;		//GPS预警
    std::string		m_secret;				//房间密码，如果为空，服务器会随机一个密码
    int		m_gold;					//房间抵住，单位元，最低为1元，最多不能超过100
    int		m_state;				//玩法规则: 0 转转  1  长沙  101-血战到底  102-血流成河
    int		m_robotNum;				// 0,不加机器人，1，2，3加机器人数量
    int     m_credits;              //最大信用值
    std::string		m_GPSLat;				//GPS经度
    std::string		m_GPSLng;				//GPS纬度
    CardValue	m_cardValue[CARD_CONFIG];
    std::vector<int>	m_playType;		//玩法: 1-自摸加底 2-自摸加番
    LMsgC2SCreateDesk() :LMsgSC(MSG_C_2_S_CREATE_ROOM)
    {
        m_flag = 0;
        m_gold = 0;
        m_state = 0;
        m_credits = 0;
        m_robotNum = 0;
        m_payment = 0;
        m_nGPSEarlyWarning = 0;
        m_nIPAntiCheat = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_flag), m_flag);
        ReadMapData(obj, NAME_TO_STR(m_secret), m_secret);
        ReadMapData(obj, NAME_TO_STR(m_gold), m_gold);
        ReadMapData(obj, NAME_TO_STR(m_state), m_state);
        ReadMapData(obj, NAME_TO_STR(m_robotNum), m_robotNum);
        ReadMapData(obj, NAME_TO_STR(m_credits), m_credits);
        msgpack::object array;
        ReadMapData(obj, NAME_TO_STR(m_cardValue), array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            for (size_t i = 0; i < array.via.array.size && i < CARD_CONFIG; ++i)
            {
                msgpack::object& pv = *(array.via.array.ptr + i);
                pv.convert(m_cardValue[i]);
            }
        }
        ReadMapData(obj, NAME_TO_STR(m_playType), array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            //int m_count = array.via.array.size;
            for (size_t i = 0; i < array.via.array.size; ++i)
            {
                int v;
                msgpack::object& obj =  *(array.via.array.ptr+i);
                obj.convert(v);
                m_playType.push_back(v);
            }
        }
        ReadMapData(obj, NAME_TO_STR(m_payment), m_payment);
        ReadMapData(obj, NAME_TO_STR(m_nIPAntiCheat), m_nIPAntiCheat);
        ReadMapData(obj, NAME_TO_STR(m_nGPSEarlyWarning), m_nGPSEarlyWarning);
        ReadMapData(obj, NAME_TO_STR(m_GPSLat), m_GPSLat);
        ReadMapData(obj, NAME_TO_STR(m_GPSLng), m_GPSLng);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 13);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_flag), m_flag);
        WriteKeyValue(pack, NAME_TO_STR(m_secret), m_secret);
        WriteKeyValue(pack, NAME_TO_STR(m_gold), m_gold);
        WriteKeyValue(pack, NAME_TO_STR(m_state), m_state);
        WriteKeyValue(pack, NAME_TO_STR(m_robotNum), m_robotNum);
        WriteKeyValue(pack, NAME_TO_STR(m_credits), m_credits);
        WriteKeyValue(pack, NAME_TO_STR(m_playType), m_playType);
        WriteKeyValue(pack, NAME_TO_STR(m_payment), m_payment);
        WriteKeyValue(pack, NAME_TO_STR(m_nIPAntiCheat), m_nIPAntiCheat);
        WriteKeyValue(pack, NAME_TO_STR(m_nGPSEarlyWarning), m_nGPSEarlyWarning);
        WriteKeyValue(pack, NAME_TO_STR(m_GPSLat), m_GPSLat);
        WriteKeyValue(pack, NAME_TO_STR(m_GPSLng), m_GPSLng);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SCreateDesk>(); }
};

//////////////////////////////////////////////////////////////////////////
//玩家请求创建桌子结果
struct LMsgS2CCreateDeskRet :public LMsgSC
{
    enum ErrorCode
    {
        Err_CreditNotEnough = 6,
    };
    int		m_errorCode;//0-成功，1-房卡不够，2-玩家已有存在的桌子，3-桌子号已被使用,4-创建桌子失败,5-创建参数有问题,6-人品不够,信用有误，7-超过代开房间上线数量
    int		m_deskId;//桌子id
    DeskListItem m_deskListItem;//创建的房间信息

    LMsgS2CCreateDeskRet() :LMsgSC(MSG_S_2_C_CREATE_ROOM)
    {
        m_errorCode = 0;
        m_deskId = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_errorCode), m_errorCode);
        ReadMapData(obj, NAME_TO_STR(m_deskId), m_deskId);
        m_deskListItem.Read(obj);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 10);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
        WriteKeyValue(pack, NAME_TO_STR(m_deskId), m_deskId);
        m_deskListItem.Write(pack);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CCreateDeskRet>(); }
};


//////////////////////////////////////////////////////////////////////////
//客户端加入桌子
struct LMsgC2SAddDesk :public LMsgSC
{
    int		m_deskId;//桌子id
    std::string		m_GPSLat;			//GPS经度
    std::string		m_GPSLng;			//GPS纬度
    int		m_userId;//玩家id //进入房间时判断是否为房主

    LMsgC2SAddDesk() :LMsgSC(MSG_C_2_S_ADD_ROOM)
    {
        m_deskId = 0;
        m_userId = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_deskId), m_deskId);
        ReadMapData(obj, NAME_TO_STR(m_GPSLat), m_GPSLat);
        ReadMapData(obj, NAME_TO_STR(m_GPSLng), m_GPSLng);
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 5);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_deskId), m_deskId);
        WriteKeyValue(pack, NAME_TO_STR(m_GPSLat), m_GPSLat);
        WriteKeyValue(pack, NAME_TO_STR(m_GPSLng), m_GPSLng);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SAddDesk>(); }
};

//////////////////////////////////////////////////////////////////////////
//玩家请求加入桌子结果
struct LMsgS2CAddDeskRet :public LMsgSC
{
    enum ErrorCode
    {
        Err_CreditNotEnough = 6,
        Err_DiamondsNotEnough = 10	//钻石不足
    };

    int		m_errorCode;//0-成功，1-人已满,2-房间不存在,3-未知错误 6-信用不够 8-相同IP(IP检测) 9-实际距离过近(GPS警告) 10-钻石不足
    int		m_deskId;//桌子号码

    LMsgS2CAddDeskRet() :LMsgSC(MSG_S_2_C_ADD_ROOM)
    {
        m_errorCode = -1;
        m_deskId = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_errorCode), m_errorCode);
        ReadMapData(obj, NAME_TO_STR(m_deskId), m_deskId);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
        WriteKeyValue(pack, NAME_TO_STR(m_deskId), m_deskId);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CAddDeskRet>(); }
};

struct LMsgC2SEnterCoinsDesk :public LMsgSC
{
    int		m_state;				//玩法规则: 0 转转  1  长沙  101-血战到底  102-血流成河
    int		m_robotNum;				// 0,不加机器人，1，2，3加机器人数量
    CardValue	m_cardValue[CARD_CONFIG];
    std::vector<int>	m_playType;		//玩法: 1-自摸加底 2-自摸加番
    int		m_coins;				// 玩家的金币
    LMsgC2SEnterCoinsDesk() :LMsgSC(MSG_C_2_S_ENTER_COINS_ROOM)
    {
        m_state = 0;
        m_robotNum = 0;
        m_coins = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_state), m_state);
        ReadMapData(obj, NAME_TO_STR(m_robotNum), m_robotNum);
        msgpack::object array;
        ReadMapData(obj, NAME_TO_STR(m_cardValue), array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            for (size_t i = 0; i < array.via.array.size && i < CARD_CONFIG; ++i)
            {
                msgpack::object& pv = *(array.via.array.ptr + i);
                pv.convert(m_cardValue[i]);
            }
        }
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
        ReadMapData(obj, NAME_TO_STR(m_coins), m_coins);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SEnterCoinsDesk>(); }
};

struct LMsgS2CEnterCoinsDeskRet :public LMsgSC
{
    int		m_errorCode;//0-成功，1-金币不够，2-玩家已在金币场，3-未知错误 4-创建桌子失败,5-创建参数有问题 6-人数太多
    int		m_remainCount; //m_errorCode=1时有效 表示玩家可以领取的金币的次数

    LMsgS2CEnterCoinsDeskRet() :LMsgSC(MSG_S_2_C_ENTER_COINS_ROOM)
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

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CEnterCoinsDeskRet>(); }
};

//////////////////////////////////////////////////////////////////////////

//vip比赛结束
struct LMsgS2CVipEnd :public LMsgSC
{
    int	m_id[4];//玩家id
    std::string	m_nike[4];//游戏玩家昵称
    int	m_gold[4];//输赢积分总数
    int	m_zimo[4];//自摸次数
    int	m_bomb[4];//放炮次数
    int	m_win[4];//收炮次数
    int	m_mgang[4];//明杠次数
    int	m_agang[4];//暗杠的次数

    int	m_dzimo[4];//大胡自摸次数
    int	m_dbomb[4];//大胡放炮次数
    int	m_dwin[4];//大胡收炮次数
    int	m_CheckTing[4];	// 被查叫次数
    int	m_nDeskID;		//房间号码
    int	m_nDiFen;			//底分
    int	m_nCurCircle;	//局数
    int	m_nPlayType;	//玩法
    int m_pos;				//玩家位置

    CardValue m_cardLaiZi[4];	//癞子牌
    std::vector<CardValue>	m_max_handCard[4];//玩家最大牌的手牌
    std::vector<CardValue>	m_max_pengCard[4];//玩家最大牌的碰牌
    std::vector<CardValue>	m_max_mgangCard[4];//玩家最大牌的明杠牌
    std::vector<CardValue>	m_max_agangCard[4];//玩家最大牌的暗杠牌
    std::vector<CardValue>	m_max_eatCard[4];//玩家最大牌的吃牌
    CardValue m_max_huCard[4];//玩家最大牌的胡牌
    int	m_owerId;
    std::string m_owerNick;

    LMsgS2CVipEnd() :LMsgSC(MSG_S_2_C_VIP_END)
    {
        for (int i = 0; i < 4; ++i)
        {
            m_id[i] = 0;
            m_gold[i] = 0;
            m_zimo[i] = 0;
            m_bomb[i] = 0;
            m_win[i] = 0;
            m_mgang[i] = 0;
            m_agang[i] = 0;
            m_dzimo[i] = 0;
            m_dbomb[i] = 0;
            m_dwin[i] = 0;
            m_CheckTing[i] = 0;
        }
        m_nDeskID = 0;
        m_nDiFen = 0;
        m_nCurCircle = 0;
        m_nPlayType = 0;
        m_pos = -1;
        m_owerId = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 48);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);

        WriteKey(pack,"m_nike");
        std::vector<std::string>vecs;
        for(int i = 0; i < 4; ++i)
        {
            vecs.push_back(m_nike[i]);
        }
        WriteKey(pack,vecs);

        WriteKey(pack,"m_id");
        std::vector<int>veci;
        for(int i = 0; i < 4; ++i)
        {
            veci.push_back(m_id[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack,"m_gold");
        veci.clear();
        for(int i = 0; i < 4; ++i)
        {
            veci.push_back(m_gold[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack,"m_zimo");
        veci.clear();
        for(int i = 0; i < 4; ++i)
        {
            veci.push_back(m_zimo[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack,"m_bomb");
        veci.clear();
        for(int i = 0; i < 4; ++i)
        {
            veci.push_back(m_bomb[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack,"m_win");
        veci.clear();
        for(int i = 0; i < 4; ++i)
        {
            veci.push_back(m_win[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack,"m_mgang");
        veci.clear();
        for(int i = 0; i < 4; ++i)
        {
            veci.push_back(m_mgang[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack,"m_agang");
        veci.clear();
        for(int i = 0; i < 4; ++i)
        {
            veci.push_back(m_agang[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack,"m_dzimo");
        veci.clear();
        for(int i = 0; i < 4; ++i)
        {
            veci.push_back(m_dzimo[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack,"m_dbomb");
        veci.clear();
        for(int i = 0; i < 4; ++i)
        {
            veci.push_back(m_dbomb[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack,"m_dwin");
        veci.clear();
        for(int i = 0; i < 4; ++i)
        {
            veci.push_back(m_dwin[i]);
        }
        WriteKey(pack,veci);

        WriteKey(pack,"m_ting");
        veci.clear();
        for(int i = 0; i < 4; ++i)
        {
            veci.push_back(m_CheckTing[i]);
        }
        WriteKey(pack,veci);

        for (int i = 0; i < 4; ++i)
        {
            char name[32];
            sprintf(name, "m_max_handCard%d", i);
            WriteKey(pack, name);
            WriteKey(pack, m_max_handCard[i]);
        }

        for (int i = 0; i < 4; ++i)
        {
            char name[32];
            sprintf(name, "m_max_pengCard%d", i);
            WriteKey(pack, name);
            WriteKey(pack, m_max_pengCard[i]);
        }

        for (int i = 0; i < 4; ++i)
        {
            char name[32];
            sprintf(name, "m_max_mgangCard%d", i);
            WriteKey(pack, name);
            WriteKey(pack, m_max_mgangCard[i]);
        }

        for (int i = 0; i < 4; ++i)
        {
            char name[32];
            sprintf(name, "m_max_agangCard%d", i);
            WriteKey(pack, name);
            WriteKey(pack, m_max_agangCard[i]);
        }

        for (int i = 0; i < 4; ++i)
        {
            char name[32];
            sprintf(name, "m_max_eatCard%d", i);
            WriteKey(pack, name);
            WriteKey(pack, m_max_eatCard[i]);
        }

        for (int i = 0; i < 4; ++i)
        {
            char name[32];
            sprintf(name, "m_max_huCard%d", i);
            WriteKey(pack, name);
            WriteKey(pack, m_max_huCard[i]);
        }

        for (Lint i = 0; i < 4; ++i)
        {
            char name[32];
            sprintf(name, "m_cardLaiZi%d", i);
            WriteKey(pack, name);
            WriteKey(pack, m_cardLaiZi[i]);
        }

        WriteKeyValue(pack, NAME_TO_STR(m_nDeskID), m_nDeskID);
        WriteKeyValue(pack, NAME_TO_STR(m_nDiFen), m_nDiFen);
        WriteKeyValue(pack, NAME_TO_STR(m_nCurCircle), m_nCurCircle);
        WriteKeyValue(pack, NAME_TO_STR(m_nPlayType), m_nPlayType);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_owerId), m_owerId);
        WriteKeyValue(pack, NAME_TO_STR(m_owerNick), m_owerNick);
    }
    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CVipEnd>(); }
};

//////////////////////////////////////////////////////////////////////////
//客户端请求对账单
struct LMsgC2SVipLog :public LMsgSC
{
    int		m_time;//id
    int		m_userId;//请求某个玩家的战绩

    LMsgC2SVipLog() :LMsgSC(MSG_C_2_S_VIP_LOG)
    {
        m_time = 0;
        m_userId = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_time), m_time);
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_time), m_time);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SVipLog>(); }
};

//////////////////////////////////////////////////////////////////////////
//客户端请求对账单2
//Client 2 LogicManager 2 LogicDB
struct LMsgC2SRoomLog :public LMsgSC
{
    std::string		m_strUUID; //客户端没有传递，由服务器填写
    int		   m_userId; //请求某个玩家的战绩
    int        m_pos;    //pos1
    int        m_time;   //time
    int        m_deskId; //桌子id

    LMsgC2SRoomLog() :LMsgSC(MSG_C_2_S_ROOM_LOG)
    {
        m_strUUID = "";
        m_userId = 0;
        m_pos = 0;
        m_deskId = 0;
        m_time = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_strUUID), m_strUUID);
        ReadMapData(obj, NAME_TO_STR(m_time), m_time);
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
        ReadMapData(obj, NAME_TO_STR(m_deskId), m_deskId);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 6);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_strUUID),m_strUUID);
        WriteKeyValue(pack, NAME_TO_STR(m_userId),m_userId);
        WriteKeyValue(pack, NAME_TO_STR(m_time), m_time);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_deskId), m_deskId);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SRoomLog>(); }
};

//////////////////////////////////////////////////////////////////////////
//vip对账结果

struct VipLog
{
    int	m_time;
    std::string	m_videoId;
    int	m_score[4];

    VipLog()
    {
        m_time = 0;
        memset( m_score, 0, sizeof(m_score) );
    }
};

struct VipLogItemInfo
{
    std::string m_id;
    int m_time;
    int m_flag;		//房间类型
    int m_userid[4];
    std::string m_nike[4];
    int m_score[4];
    int m_reset;//是否解散
    int m_deskId;//是否解散
    int m_size;
    VipLog m_item[32];
    int m_maxCircle;	//最大圈数
    std::vector<Lint> m_playType;	//玩法（包含底分信息）
    std::string m_strHouseOwnerNike;

    VipLogItemInfo()
    {
        m_time = 0;
        m_flag = 0;
        memset( m_userid, 0, sizeof(m_userid) );
        memset( m_score, 0, sizeof(m_score) );
        m_reset = 0;
        m_deskId = 0;
        m_size = 0;
        m_maxCircle = 0;
    }
};

struct LMsgS2CVipLog:public LMsgSC
{
    int				m_size;//长度
    VipLogItemInfo		m_info[20];//log信息

    LMsgS2CVipLog() :LMsgSC(MSG_S_2_C_VIP_LOG)
    {
        m_size = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);

        WriteKey(pack,"m_data");
        WriteArray(pack,m_size);
        for(int i = 0 ; i < m_size; ++i)
        {
            WriteMap(pack,9);
            WriteKeyValue(pack,"m_id",m_info[i].m_id);
            WriteKeyValue(pack,"m_time",m_info[i].m_time);
            WriteKeyValue(pack,"m_flag",m_info[i].m_flag);
            WriteKeyValue(pack,"m_reset",m_info[i].m_reset);
            WriteKeyValue(pack,"m_deskId",m_info[i].m_deskId);

            std::vector<std::string> vecs;
            std::vector<int>veci,vec;
            for(int j = 0 ; j < 4; ++j)
            {
                vecs.push_back(m_info[i].m_nike[j]);
                veci.push_back(m_info[i].m_score[j]);
                vec.push_back(m_info[i].m_userid[j]);
            }
            WriteKey(pack,"m_userid");
            WriteKey(pack,vec);

            WriteKey(pack,"m_nike");
            WriteKey(pack,vecs);

            WriteKey(pack,"m_score");
            WriteKey(pack,veci);

            WriteKey(pack,"m_match");
            WriteArray(pack,m_info[i].m_size);

            WriteKeyValue(pack, "m_maxCircle", m_info[i].m_maxCircle);
            WriteKeyValue(pack, "m_playType", m_info[i].m_playType);
            WriteKeyValue(pack, NAME_TO_STR(m_strHouseOwnerNike), m_info[i].m_strHouseOwnerNike);
        }
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CVipLog>(); }
};

//LogicManager 2 Client
struct LMsgS2CRoomLog:public LMsgSC
{
    int m_size;
    int m_flag;		//房间类型
    VipLog m_item[32];

    LMsgS2CRoomLog() :LMsgSC(MSG_S_2_C_ROOM_LOG)
    {
        m_flag = 0;
        m_size = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack,"m_flag",m_flag);
        WriteKey(pack,"m_data");
        WriteArray(pack,m_size);
        for(int k = 0 ; k < m_size && k < 32; ++k)
        {
            WriteMap(pack,3);
            WriteKeyValue(pack,"m_time",m_item[k].m_time);
            WriteKeyValue(pack,"m_videoId",m_item[k].m_videoId);

            std::vector<int> vec;
            for(int n = 0 ; n < 4; ++n)
            {
                vec.push_back(m_item[k].m_score[n]);
            }
            WriteKey(pack,"m_score");
            WriteKey(pack,vec);
        }
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CRoomLog>(); }
};

//inviter-----------------------------------------------
struct UserBasicInfo
{
    int		m_id;
    std::string		m_unionId;
    std::string		m_nickName;
    std::string		m_headImgUrl;
    UserBasicInfo()
    {
        m_id = 0;
        m_unionId = "";
        m_nickName = "";
        m_headImgUrl = "";
    }

    void Write(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 4);
        WriteKeyValue(pack, NAME_TO_STR(m_id), m_id);
        WriteKeyValue(pack, NAME_TO_STR(m_unionId), m_unionId);
        WriteKeyValue(pack, NAME_TO_STR(m_nickName), m_nickName);
        WriteKeyValue(pack, NAME_TO_STR(m_headImgUrl), m_headImgUrl);
    }
};

struct InviteeInfo
{
    UserBasicInfo	m_info;
    int				m_taskFinished;		// 被邀请者任务是否完成，0：没有完成，非0：完成
    InviteeInfo():m_info()
    {
        m_taskFinished = 0;
    }

    void Write(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 5);
        WriteKeyValue(pack, NAME_TO_STR(m_id), m_info.m_id);
        WriteKeyValue(pack, NAME_TO_STR(m_unionId), m_info.m_unionId);
        WriteKeyValue(pack, NAME_TO_STR(m_nickName), m_info.m_nickName);
        WriteKeyValue(pack, NAME_TO_STR(m_headImgUrl), m_info.m_headImgUrl);
        WriteKeyValue(pack, NAME_TO_STR(m_taskFinished), m_taskFinished);
    }
};

struct LMsgS2CInvitingInfo : public LMsgSC
{
    struct Reward
    {
        int	m_type;
        int	m_count;
        Reward():m_type(0),m_count(0){}
    };

    Reward						m_reward;
    UserBasicInfo				m_inviter;
    std::vector<InviteeInfo>	m_invitees;

    LMsgS2CInvitingInfo() : LMsgSC(MSG_S_2_C_INVITING_INFO) ,m_inviter()
    {
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 4);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);

        WriteKey(pack, NAME_TO_STR(m_reward));
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_type), m_reward.m_type);
        WriteKeyValue(pack, NAME_TO_STR(m_count), m_reward.m_count);

        WriteKey(pack, NAME_TO_STR(m_inviter));
        m_inviter.Write(pack);

        WriteKey(pack, NAME_TO_STR(m_invitees));
        WriteArray(pack, static_cast<uint32_t >(m_invitees.size()));

        for (size_t i = 0; i < m_invitees.size(); i++)
            m_invitees[i].Write(pack);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgS2CInvitingInfo>(); }
};

struct LMsgC2SBindInviter : public LMsgSC
{
    int	m_inviterId;

    LMsgC2SBindInviter() : LMsgSC(MSG_C_2_S_BIND_INVITER)
    {
        m_inviterId = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_inviterId), m_inviterId);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_inviterId), m_inviterId);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgC2SBindInviter>(); }
};

struct LMsgS2CBindInviter : public LMsgSC
{
    ErrorCode			m_error;
    UserBasicInfo		m_inviter;

    LMsgS2CBindInviter() : LMsgSC(MSG_S_2_C_BIND_INVITER),m_inviter()
    {
        m_error = Unknown;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_error), (int)m_error);
        WriteKey(pack, NAME_TO_STR(m_inviter));
        m_inviter.Write(pack);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgS2CBindInviter>(); }
};
////-end

struct LMsgC2SGetUserInfo : public LMsgSC
{
    int		m_userId;

    LMsgC2SGetUserInfo() : LMsgSC(MSG_C_2_S_GET_USER_INFO), m_userId(-1) {}

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgC2SGetUserInfo>(); }
};

struct LMsgS2CUserInfo : public LMsgSC
{
    ErrorCode			m_error;
    UserBasicInfo		m_user;

    LMsgS2CUserInfo() : LMsgSC(MSG_S_2_C_USER_INFO)
    {
        m_error = Unknown;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_error), (int)m_error);
        WriteKey(pack, NAME_TO_STR(m_user));
        m_user.Write(pack);
    }

    virtual LMsgPtr Clone() {return std::make_shared<LMsgS2CUserInfo>();}
};

struct LMsgC2SGetInvitingInfo : public LMsgSC
{
    std::string  m_strUUID;		//由服务器填写
    int     m_userId;
    LMsgC2SGetInvitingInfo() : LMsgSC(MSG_C_2_S_GET_INVITING_INFO)
    {
        m_userId = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
        ReadMapData(obj, NAME_TO_STR(m_strUUID), m_strUUID);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
        WriteKeyValue(pack, NAME_TO_STR(m_strUUID), m_strUUID);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgC2SGetInvitingInfo>(); }
};

//------------------------------------------------------------------


//////////////////////////////////////////////////////////////////////////
//客户端请求录像
struct LMsgC2SVideo :public LMsgSC
{
    std::string		m_videoId;//id

    LMsgC2SVideo() :LMsgSC(MSG_C_2_S_VIDEO){}

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_videoId), m_videoId);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_videoId), m_videoId);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SVideo>(); }
};

//////////////////////////////////////////////////////////////////////////
//vip对账结果

struct LMsgS2CVideo:public LMsgSC
{
    VideoLog	m_video;//log信息
    int			m_sex[4];
    std::string	m_nike[4];
    std::string	m_imageUrl[4];
    int			m_zhuang;
    int			m_deskId;
    int			m_curCircle;
    int			m_maxCircle;
    int			m_flag;
    int			m_score[4];

    LMsgS2CVideo() :LMsgSC(MSG_S_2_C_VIDEO)
    {
        memset( m_sex, 0, sizeof(m_sex) );
        m_zhuang = 0;
        m_deskId = 0;
        m_curCircle = 0;
        m_maxCircle = 0;
        m_flag = 0;
        memset( m_score, 0, sizeof(m_score) );
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 19);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);

        WriteKeyValue(pack, NAME_TO_STR(m_zhuang), m_zhuang);
        WriteKeyValue(pack, NAME_TO_STR(m_deskId), m_deskId);
        WriteKeyValue(pack, NAME_TO_STR(m_curCircle), m_curCircle);
        WriteKeyValue(pack, NAME_TO_STR(m_maxCircle), m_maxCircle);
        WriteKeyValue(pack, NAME_TO_STR(m_state), m_flag);

        std::vector<int>vecid;
        std::vector<int>vecsex;
        std::vector<int>vecscore;
        std::vector<std::string>vecnike;
        std::vector<std::string>vecurl;

        for(int i = 0 ; i < 4; ++i)
        {
            if ( m_video.m_userId[i] == 0 )
            {
                continue;
            }
            vecid.push_back(m_video.m_userId[i]);
            vecnike.push_back(m_nike[i]);
            vecsex.push_back(m_sex[i]);
            vecurl.push_back(m_imageUrl[i]);
            vecscore.push_back(m_score[i]);
        }

        WriteKey(pack,"m_score");
        WriteKey(pack,vecscore);

        WriteKey(pack,"m_userid");
        WriteKey(pack,vecid);

        WriteKey(pack,"m_nike");
        WriteKey(pack, vecnike);

        WriteKey(pack,"m_sex");
        WriteKey(pack, vecsex);

        WriteKey(pack,"m_imageUrl");
        WriteKey(pack, vecurl);

        WriteKey(pack,"m_time");
        WriteKey(pack,m_video.m_time);

        WriteKey(pack,"m_oper");
        WriteKey(pack,m_video.m_oper);

        WriteKey(pack,"m_playtype");
        WriteKey(pack,m_video.m_playType);

        std::vector<CardValue> vec;
        vec.push_back(m_video.m_fanPai);
//		vec.push_back(m_video.m_hunPai);
        WriteKeyValue(pack, NAME_TO_STR(m_fanPai), vec);

        for(int i = 0 ; i < 4; ++i)
        {
            std::stringstream ss ;
            ss << "m_card" << i;
            WriteKey(pack,ss.str());
            WriteKey(pack,m_video.m_handCard[i]);
        }
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CVideo>(); }
};

//////////////////////////////////////////////////////////////////////////
//同步vip房间信息
struct LMsgS2CVipInfo :public LMsgSC
{
    int			m_curCircle;//当前局数
    int			m_curMaxCircle;//最大局数
    std::vector<int>	m_playtype;

    LMsgS2CVipInfo() :LMsgSC(MSG_S_2_C_VIP_INFO)
    {
        m_curCircle = 0;
        m_curMaxCircle = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 4);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_curCircle), m_curCircle);
        WriteKeyValue(pack, NAME_TO_STR(m_curMaxCircle), m_curMaxCircle);
        WriteKeyValue(pack, NAME_TO_STR(m_playtype), m_playtype);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CVipInfo>(); }
};


// 服务器通知玩家开始定缺
struct LMsgS2CUserStartDingQue:public LMsgSC
{
    int		m_state[4];		//玩家定飘的状态，0：没订过缺	1：订过缺

    LMsgS2CUserStartDingQue() :LMsgSC(MSG_S_2_C_USER_DINGQUE_START)
    {
        memset( m_state, 0, sizeof(m_state) );
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);

        WriteKey(pack, "m_state");
        std::vector<int> vec;
        for (int i = 0; i < 4; ++i)
        {
            vec.push_back(m_state[i]);
        }
        WriteKey(pack,vec);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CUserStartDingQue>(); }
};

//服务器通知玩家定缺完毕
struct LMsgS2CUserCompleteDingQue:public LMsgSC
{
    int		m_color[4];		//玩家定缺的颜色 定飘的值

    LMsgS2CUserCompleteDingQue() :LMsgSC(MSG_S_2_C_USER_DINGQUE_COMPLETE)
    {
        memset( m_color, 0, sizeof(m_color) );
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);

        WriteKey(pack, "m_color");
        std::vector<int> vec;
        for (int i = 0; i < 4; ++i)
        {
            vec.push_back(m_color[i]);
        }
        WriteKey(pack,vec);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CUserCompleteDingQue>(); }
};

//////////////////////////////////////////////////////////////////////////
//客户端定缺
struct LMsgC2SUserDingQue:public LMsgSC
{
    int		m_color;	// 定飘 的值 0 1 2

    LMsgC2SUserDingQue() :LMsgSC(MSG_C_2_S_USER_DINGQUE)
    {
        m_color = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_color), m_color);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgC2SUserDingQue>();}
};


// 服务器通知玩家换牌
struct LMsgS2CUserChange:public LMsgSC
{
    int		m_time;		// 10:请求玩家 0:表示玩家换牌成功
    int		m_pos;		//换牌玩家位置
    CardValue	m_card[3];	// m_time=0时有效  目前只有金币场填写因为有倒计时

    LMsgS2CUserChange() :LMsgSC(MSG_S_2_C_USER_CHANGE)
    {
        m_time = 0;
        m_pos = INVAILD_POS;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 4);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_time), m_time);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        std::vector<CardValue> vec;
        WriteKey(pack, "m_card");
        for (int i = 0; i < 3; ++i)
        {
            vec.push_back(m_card[i]);
        }
        WriteKey(pack,vec);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CUserChange>(); }
};

//////////////////////////////////////////////////////////////////////////
//客户端换牌
struct LMsgC2SUserChange:public LMsgSC
{
    int		m_pos;		//玩家的位置
    CardValue	m_card[3];	//玩家换的牌

    LMsgC2SUserChange() :LMsgSC(MSG_C_2_S_USER_CHANGE)
    {
        m_pos = INVAILD_POS;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
        //ReadMapData(obj, NAME_TO_STR(m_card), m_card);
        msgpack::object array;
        ReadMapData(obj, NAME_TO_STR(m_card), array);
        if (array.type == msgpack::v1::type::ARRAY && array.via.array.size == 3)
        {
            //int m_count = array.via.array.size;
            for (size_t i = 0; i < array.via.array.size; ++i)
            {
                CardValue v;
                msgpack::object& obj =  *(array.via.array.ptr+i);
                obj.convert(v);
                m_card[i] = v;
            }
        }
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
    }

    virtual LMsgPtr Clone(){return std::make_shared<LMsgC2SUserChange>();}
};

// 服务器通知玩家换牌结果
struct LMsgS2CUserChangeResult:public LMsgSC
{
    int		m_flag;		//换牌规则
    CardValue	m_card[3];	//玩家换的牌

    LMsgS2CUserChangeResult() :LMsgSC(MSG_S_2_C_USER_CHANGERESULT)
    {
        m_flag = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_flag), m_flag);

        std::vector<CardValue> vec;
        WriteKey(pack, "m_card");
        for (int i = 0; i < 3; ++i)
        {
            vec.push_back(m_card[i]);
        }
        WriteKey(pack,vec);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CUserChangeResult>(); }
};

// 单局结算
struct SingleResult
{
    int m_winPos;
    int m_dianPos;
    std::vector<int> m_fanType;
    int m_genCount;
    CardValue m_winCard;

    SingleResult()
    {
        m_winPos = INVAILD_POS;
        m_dianPos = INVAILD_POS;
        m_fanType.clear();
        m_genCount = 0;
        m_winCard.m_color = 0;
        m_winCard.m_number = 0;
    }

    MSGPACK_DEFINE(m_winPos, m_dianPos, m_fanType,m_genCount, m_winCard);

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteKeyValue(pack, NAME_TO_STR(m_winPos), m_winPos);
        WriteKeyValue(pack, NAME_TO_STR(m_dianPos), m_dianPos);
        WriteKeyValue(pack, NAME_TO_STR(m_fanType), m_fanType);
        WriteKeyValue(pack, NAME_TO_STR(m_genCount), m_genCount);
        WriteKeyValue(pack, NAME_TO_STR(m_winCard), m_winCard);
    }
};

// 服务器通知玩家单次胡牌信息
struct LMsgS2CGameResult:public LMsgSC
{
    std::vector<SingleResult>	m_result;

    LMsgS2CGameResult() :LMsgSC(MSG_S_2_C_GAMEREULT){}

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);

        std::vector<SingleResult> vec;
        WriteKey(pack, "m_result");
        for (size_t i = 0; i < m_result.size(); ++i)
        {
            vec.push_back(m_result[i]);
        }
        WriteKey(pack,vec);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CGameResult>(); }
};

struct LMsgC2STangCard : public LMsgSC
{
    CardValue			m_cardValue[14];  //躺牌
    CardValue           m_outCard;

    LMsgC2STangCard() :LMsgSC(MSG_C_2_S_TANGCARD)
    {
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        msgpack::object array;
        ReadMapData(obj, NAME_TO_STR(m_cardValue), array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            for (size_t i = 0; i < array.via.array.size; ++i)
            {
                msgpack::object& pv =  *(array.via.array.ptr+i);
                pv.convert(m_cardValue[i]);
            }
        }

        ReadMapData(obj, NAME_TO_STR(m_outCard), array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            msgpack::object& pv =  *(array.via.array.ptr);
            pv.convert(m_outCard);
        }
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgC2STangCard>();
    }
};

struct LMsgS2CTangCardRet : public LMsgSC
{
    enum
    {
        TSuccessed   = 0,    //躺成功
        TPosError    = 1,    //pos错误
        TStatusError = 2,    //状态错误
        TCardError   = 3,    //躺牌错误,躺牌不在手牌中 ，或为空
        TFailed      = 4,    //躺不下去
    };

    int				m_result;   //0=success other=failed
    int                m_pos;      // 躺牌玩家的pos
    int                m_tangCount;
    CardValue	        m_cardValue[14];	//玩家 躺下的牌
    CardValue			m_outCard;			//玩家躺牌出去的牌
    int                m_tingCount;
    CardValue	        m_tingCards[27];	//玩家听的牌

    LMsgS2CTangCardRet() :LMsgSC(MSG_S_2_C_TANGCARD_RET)
    {
        m_result = -1;
        m_pos = INVAILD_POS;
        m_tangCount = 0;
        m_tingCount = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 6);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_result), m_result);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);

        std::vector<CardValue> vec;
        WriteKey(pack, "m_cardValue");
        for (int i = 0; i < m_tangCount && i < 14; ++i)
        {
            vec.push_back(m_cardValue[i]);
        }
        WriteKey(pack,vec);

        WriteKeyValue(pack, NAME_TO_STR(m_outCard), m_outCard);

        vec.clear();
        WriteKey(pack, "m_tingCards");
        for (int i = 0; i < m_tingCount && i < 14; ++i)
        {
            vec.push_back(m_tingCards[i]);
        }
        WriteKey(pack,vec);
    }

    virtual LMsgPtr Clone()
    {
        return std::make_shared<LMsgS2CTangCardRet>();
    }
};

//////////////////////////////////////////////////////////////////////////
//客户端请求 可点赞记录
struct LMsgC2SCRELog :public LMsgSC
{
    int		m_time;   //暂未用
    int		m_userId; //用户id

    LMsgC2SCRELog() :LMsgSC(MSG_C_2_S_REQ_CREHIS)
    {
        m_time = 0;
        m_userId = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_time), m_time);
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_time), m_time);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SCRELog>(); }
};

//用户基本信息for 点赞客户端显示头像 等信息
struct UserBaseInfo_s
{
    int    m_userId;
    std::string m_strNike;
    std::string m_strHeadUrl;
    int    m_sex;

    UserBaseInfo_s()
    {
        m_sex = 0;
        m_userId = 0;
    }

    MSGPACK_DEFINE(m_userId, m_strNike, m_strHeadUrl, m_sex);

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
        WriteKeyValue(pack, NAME_TO_STR(m_strNike), m_strNike);
        WriteKeyValue(pack, NAME_TO_STR(m_strHeadUrl), m_strHeadUrl);
        WriteKeyValue(pack, NAME_TO_STR(m_sex), m_sex);
    }
};

//服务器 返回 可点赞记录
struct LMsgS2CCRELogRet :public LMsgSC
{
    std::vector<std::string> m_record;   //点赞记录
    std::vector<UserBaseInfo_s> m_info;  //用户基本信息

    LMsgS2CCRELogRet() :LMsgSC(MSG_S_2_C_RET_CREHIS)
    {
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_record), m_record);

        std::vector<UserBaseInfo_s> vec;
        WriteKey(pack, "m_info");
        for (size_t i = 0; i < m_info.size(); ++i)
        {
            vec.push_back(m_info[i]);
        }
        WriteKey(pack,vec);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CCRELogRet>(); }
};

//////////////////////////////////////////////////////////////////////////
//客户端请求 评价点赞
struct LMsgC2SREQ_GTU :public LMsgSC
{
    int     m_index;                   //索引
    int		m_userId;                  //当前请求玩家id
    std::string     m_onelog;                  //可点赞记录
    std::vector<int>        m_userList;   //用户列表

    LMsgC2SREQ_GTU() :LMsgSC(MSG_C_2_S_REQ_GTU)
    {
        m_userId = 0;
        m_index = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_index), m_index);
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
        ReadMapData(obj, NAME_TO_STR(m_onelog), m_onelog);
        msgpack::object array;
        ReadMapData(obj, NAME_TO_STR(m_userList), array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            //int m_count = array.via.array.size;
            for (size_t i = 0; i < array.via.array.size; ++i)
            {
                int v = 0;
                msgpack::object& obj =  *(array.via.array.ptr+i);
                obj.convert(v);
                m_userList.push_back(v);
            }
        }
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SREQ_GTU>(); }
};

//////////////////////////////////////////////////////////////////////////
//服务器 回复 客户端请求点赞成功
struct LMsgS2CREQ_GTU_RET :public LMsgSC
{
    int        m_index;   //索引

    LMsgS2CREQ_GTU_RET() :LMsgSC(MSG_S_2_C_RET_GTU)
    {
        m_index = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_index), m_index);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CREQ_GTU_RET>(); }
};


//////////////////////////////////////////////////////////////////////////
//客户端请求 评价点赞
struct LMsgC2SREQ_Exch :public LMsgSC
{
    int        m_card;                   //索引

    LMsgC2SREQ_Exch() :LMsgSC(MSG_C_2_S_REQ_EXCH)
    {
        m_card = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_card), m_card);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SREQ_Exch>(); }
};

//////////////////////////////////////////////////////////////////////////
//服务器 回复 客户端请求点赞成功
struct LMsgS2CREQ_EXCH_RET :public LMsgSC
{
    int        m_card;   //索引

    LMsgS2CREQ_EXCH_RET() :LMsgSC(MSG_S_2_C_RET_EXCH)
    {
        m_card = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_card), m_card);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CREQ_EXCH_RET>(); }
};



struct LMsgS2CActivityInfo:public LMsgSC
{
    typedef struct Value
    {
        std::string m_key;
        std::string m_value;
        MSGPACK_DEFINE(m_key,m_value);
    }ATTRI;

    struct Info
    {
        int m_id;		   // ActivityID类型
        std::string m_info;	   // 活动内容 根据m_id来填充和解析
        std::vector<struct Value> m_attri;  //根据m_id来填充解析

        Info()
        {
            m_id = 0;
        }

        MSGPACK_DEFINE(m_id, m_info,m_attri);
    };

    std::vector<Info> m_activities;

    LMsgS2CActivityInfo() : LMsgSC(MSG_S_2_C_ACTIVITY_INFO)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        int m_num = (int)m_activities.size();
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_num), m_num);
        WriteKeyValue(pack, NAME_TO_STR(m_activities), m_activities);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CActivityInfo>(); }
};

struct LMsgC2SActivityPhone : public LMsgSC
{
    std::string		m_strUUID;	// 客户端不需要填充 服务器内部转发使用
    int		m_userId;	// 客户端不需要填充 服务器内部转发使用
    int		m_AutoId;		// 服务器的给客户端的ID
    std::string		m_PhoneNum;		// 电话号码

    LMsgC2SActivityPhone() :LMsgSC(MSG_C_2_S_ACTIVITY_WRITE_PHONE)
    {
        m_userId = 0;
        m_AutoId = -1;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_strUUID), m_strUUID);
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
        ReadMapData(obj, NAME_TO_STR(m_AutoId), m_AutoId);
        ReadMapData(obj, NAME_TO_STR(m_PhoneNum), m_PhoneNum);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 5);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_strUUID), m_strUUID);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
        WriteKeyValue(pack, NAME_TO_STR(m_AutoId), m_AutoId);
        WriteKeyValue(pack, NAME_TO_STR(m_PhoneNum), m_PhoneNum);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SActivityPhone>(); }
};

struct LMsgS2CActivityPhone:public LMsgSC
{
    int		m_errorCode;			//结果 0成功 其他失败
    LMsgS2CActivityPhone() :LMsgSC(MSG_S_2_C_ACTIVITY_WRITE_PHONE)
    {
        m_errorCode = -1;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_errorCode), m_errorCode);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CActivityPhone>(); }
};

struct LMsgC2SActivityRequestLog : public LMsgSC
{
    std::string		m_strUUID;	// 客户端不需要填充 服务器内部转发使用
    int		m_userId;	// 客户端不需要填充 服务器内部转发使用
    int		m_activeId;	// 活动ID ActivityID类型 若为0表示查询玩家所有的活动纪录

    LMsgC2SActivityRequestLog() :LMsgSC(MSG_C_2_S_ACTIVITY_REQUEST_LOG)
    {
        m_userId = 0;
        m_activeId = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_strUUID), m_strUUID);
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
        ReadMapData(obj, NAME_TO_STR(m_activeId), m_activeId);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 4);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_strUUID), m_strUUID);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
        WriteKeyValue(pack, NAME_TO_STR(m_activeId), m_activeId);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SActivityRequestLog>(); }
};

struct ActivityLog
{
    ActivityLog()
    {
        AutoId = 0;
        Id = 0;
        SpendType = 0;
        SpendCount = 0;
        GiftType = 0;
        GiftCount = 0;
        Date = 0;
    }

    int AutoId;
    int Id;
    int SpendType;
    int SpendCount;
    std::string GiftName;
    int GiftType;
    int GiftCount;
    int Date;
    std::string Phone;

    // 下面这种写法 客户端看不到变量名 直接安装顺序来取值 所以顺序很重要
    MSGPACK_DEFINE(AutoId, Id, SpendType, SpendCount, GiftName, GiftType, GiftCount, Date, Phone);
};

struct LMsgS2CActivityReplyLog:public LMsgSC
{
    int		m_errorCode;			//结果 0成功 其他失败
    std::vector<ActivityLog> m_logs;

    LMsgS2CActivityReplyLog() :LMsgSC(MSG_S_2_C_ACTIVITY_REPLY_LOG)
    {
        m_errorCode = -1;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_errorCode), m_errorCode);
        ReadMapData(obj, NAME_TO_STR(m_logs), m_logs);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
        WriteKeyValue(pack, NAME_TO_STR(m_logs), m_logs);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CActivityReplyLog>(); }
};

struct LMsgC2SActivityRequestDrawOpen:public LMsgSC
{
    std::string		m_strUUID;	// 客户端不需要填充 服务器内部转发使用
    int		m_userId;	// 客户端不需要填充 服务器内部转发使用

    LMsgC2SActivityRequestDrawOpen() :LMsgSC(MSG_C_2_S_ACTIVITY_REQUEST_DRAW_OPEN)
    {
        m_userId = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_strUUID), m_strUUID);
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_strUUID), m_strUUID);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SActivityRequestDrawOpen>(); }
};

// 装盘
struct ActivityDrawWinUser
{
    std::string m_userId;	// 玩家id
    std::string m_giftName;	// 中奖的名称
    MSGPACK_DEFINE(m_userId,m_giftName);
};

struct LMsgS2CActivityReplyDrawOpen:public LMsgSC
{
    int		m_errorCode;	// 转盘结果 0成功 -1未知错误 1当前没有这个转盘活动
    // m_errorCode=0 时 下面的值有效
    std::vector<ActivityDrawWinUser> m_winUsers;
    int		m_SpendType;	// 消耗的类型 0免费 1房卡（单位个）
    int		m_SpendCount;	// 消耗的数量

    LMsgS2CActivityReplyDrawOpen() :LMsgSC(MSG_S_2_C_ACTIVITY_REPLY_DRAW_OPEN)
    {
        m_errorCode = -1;
        m_SpendType = 0;
        m_SpendCount = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_errorCode), m_errorCode);
        ReadMapData(obj, NAME_TO_STR(m_winUsers), m_winUsers);
        ReadMapData(obj, NAME_TO_STR(m_SpendType), m_SpendType);
        ReadMapData(obj, NAME_TO_STR(m_SpendCount), m_SpendCount);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 5);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
        WriteKeyValue(pack, NAME_TO_STR(m_winUsers), m_winUsers);
        WriteKeyValue(pack, NAME_TO_STR(m_SpendType), m_SpendType);
        WriteKeyValue(pack, NAME_TO_STR(m_SpendCount), m_SpendCount);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CActivityReplyDrawOpen>(); }
};

struct LMsgC2SActivityRequestDraw:public LMsgSC
{
    std::string		m_strUUID;	// 客户端不需要填充 服务器内部转发使用
    int		m_userId;	// 客户端不需要填充 服务器内部转发使用
    int		m_cardNum;	// 玩家的卡 客户端不需要填充 服务器内部转发使用

    LMsgC2SActivityRequestDraw() :LMsgSC(MSG_C_2_S_ACTIVITY_REQUEST_DRAW)
    {
        m_userId = 0;
        m_cardNum = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_strUUID), m_strUUID);
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
        ReadMapData(obj, NAME_TO_STR(m_cardNum), m_cardNum);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 4);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_strUUID), m_strUUID);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
        WriteKeyValue(pack, NAME_TO_STR(m_cardNum), m_cardNum);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SActivityRequestDraw>(); }
};

struct LMsgS2CActivityReplyDraw:public LMsgSC
{
    int		m_errorCode;	// 转盘结果 0成功 -1未知错误 1消耗的物品不足（一般卡不足） 2 抽奖活动已经关闭或者未开启
    // 结果为0时 下面的值有效
    int		m_GiftIndex;	// 奖励索引
    std::string		m_GiftName;		// 奖励名称
    int		m_GiftType;		// 奖励类型 参照ActivityDrawGift 1房卡（单位个） 2话费（单位元） 3 乐视电视（单位个） 4 IPad min4（单位个） 5 iPhone7 plus（单位个）
    int		m_GiftCount;	// 奖励数量
    int		m_NeedPhoneNum;	// 是否需要电话
    int		m_AutoId;		// 填写电话号码时回执使用

    LMsgS2CActivityReplyDraw() :LMsgSC(MSG_S_2_C_ACTIVITY_REPLY_DRAW)
    {
        m_errorCode = -1;
        m_GiftIndex = 0;
        m_GiftType = 0;
        m_GiftCount = 0;
        m_NeedPhoneNum = false;
        m_AutoId = -1;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_errorCode), m_errorCode);
        ReadMapData(obj, NAME_TO_STR(m_GiftIndex), m_GiftIndex);
        ReadMapData(obj, NAME_TO_STR(m_GiftName), m_GiftName);
        ReadMapData(obj, NAME_TO_STR(m_GiftType), m_GiftType);
        ReadMapData(obj, NAME_TO_STR(m_GiftCount), m_GiftCount);
        ReadMapData(obj, NAME_TO_STR(m_NeedPhoneNum), m_NeedPhoneNum);
        ReadMapData(obj, NAME_TO_STR(m_AutoId), m_AutoId);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 8);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
        WriteKeyValue(pack, NAME_TO_STR(m_GiftIndex), m_GiftIndex);
        WriteKeyValue(pack, NAME_TO_STR(m_GiftName), m_GiftName);
        WriteKeyValue(pack, NAME_TO_STR(m_GiftType), m_GiftType);
        WriteKeyValue(pack, NAME_TO_STR(m_GiftCount), m_GiftCount);
        WriteKeyValue(pack, NAME_TO_STR(m_NeedPhoneNum), m_NeedPhoneNum);
        WriteKeyValue(pack, NAME_TO_STR(m_AutoId), m_AutoId);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CActivityReplyDraw>(); }
};

//客户端 分享朋友圈 成功
struct LMsgC2SActivityRequestShare:public LMsgSC
{
    std::string		m_strUUID;	// 客户端不需要填充 服务器内部转发使用
    int		m_userId;	// 客户端不需要填充 服务器内部转发使用

    LMsgC2SActivityRequestShare() :LMsgSC(MSG_C_2_S_ACTIVITY_REQUEST_SHARE)
    {
        m_userId = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_strUUID), m_strUUID);
        ReadMapData(obj, NAME_TO_STR(m_userId), m_userId);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_strUUID), m_strUUID);
        WriteKeyValue(pack, NAME_TO_STR(m_userId), m_userId);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SActivityRequestShare>(); }
};

struct LMsgS2CActivityFinishShare:public LMsgSC
{
    int		m_ErrorCode;	// 转盘结果 0成功 -1未知错误
    int		m_GiftCount;	// 奖励房卡张数0=不奖励
    int		m_GiftType;		// 奖励类型 参照ActivityDrawGift

    LMsgS2CActivityFinishShare() :LMsgSC(MSG_S_2_C_ACTIVITY_FINISH_SHARE)
    {
        m_ErrorCode = -1;
        m_GiftCount = 0;
        m_GiftType = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_ErrorCode), m_ErrorCode);
        ReadMapData(obj, NAME_TO_STR(m_GiftCount), m_GiftCount);
        ReadMapData(obj, NAME_TO_STR(m_GiftType), m_GiftType);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 4);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_ErrorCode), m_ErrorCode);
        WriteKeyValue(pack, NAME_TO_STR(m_GiftType), m_GiftType);
        WriteKeyValue(pack, NAME_TO_STR(m_GiftCount), m_GiftCount);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CActivityFinishShare>(); }
};

//=== modify by hp 2017-07-06 玩家被封号时通知客户端该玩家被踢出桌子的消息
struct LMsgS2CKickUser : public LMsgSC
{
    std::string m_userName;//被踢出的玩家昵称
    std::string m_reason;  //被踢出的原因

    LMsgS2CKickUser() :LMsgSC(MSG_S_2_C_KICK_USER)
    {
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_userName), m_userName);
        ReadMapData(obj, NAME_TO_STR(m_reason), m_reason);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_userName), m_userName);
        WriteKeyValue(pack, NAME_TO_STR(m_reason), m_reason);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CKickUser>(); }
};

struct LMsgS2CDeskListUpdate : public LMsgSC
{
    int			m_type;			// 1桌子人数变化，2桌子解散，3玩家在桌子中掉线重连
    DeskListItem 	m_deskListItem;	// 代开房间信息

    LMsgS2CDeskListUpdate() :LMsgSC(MSG_S_2_C_DESK_LIST_UPDATE)
    {
        m_type = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_type), m_type);
        m_deskListItem.Read(obj);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 9);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);
        m_deskListItem.Write(pack);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CDeskListUpdate>(); }
};

// 通知客户端需要弹出X一色和过选项按键
struct LMsgS2CNoticeYiSe : public LMsgSC
{
    int	m_option; // 使用按位&操作解析该值(见 ENUM_AFFIRM_FLAG 值)

    LMsgS2CNoticeYiSe() :LMsgSC(MSG_S_2_C_NOTICE_YISE)
    {
        m_option = ENUM_AFFIRM_FLAG_NONE;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_option), m_option);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_option), m_option);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CNoticeYiSe>(); }
};

// 用户在客户端上选择X一色的结果
struct LMsgC2SelectYiSeRet : public LMsgSC
{
    int	m_type; // 1风一色,2将一色,3清一色,4过 (对应 ENUM_AFFIRM_TYPE)

    LMsgC2SelectYiSeRet() :LMsgSC(MSG_C_2_S_SELECT_YISE_RET)
    {
        m_type = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_type), m_type);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgC2SelectYiSeRet>(); }
};

// 用户收到客户端的选择X一色的结果通知其他客户端
struct LMsgS2CSelectYiSeRet : public LMsgSC
{
    int	m_type; // 1风一色,2将一色,3清一色,4过 (对应 ENUM_AFFIRM_TYPE)
    int	m_pos;	// 通知者的位置

    LMsgS2CSelectYiSeRet() :LMsgSC(MSG_S_2_C_SELECT_YISE_RET)
    {
        m_type = 0;
        m_pos = -1;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_type), m_type);
        ReadMapData(obj, NAME_TO_STR(m_pos), m_pos);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_type), m_type);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
    }

    virtual LMsgPtr Clone(){ return std::make_shared<LMsgS2CSelectYiSeRet>(); }
};

//服务器告诉可以亮牌的消息
struct LMsgS2CLiangPai : public LMsgSC
{
    int m_liangNum;	//大于0表示可以亮牌

    LMsgS2CLiangPai() :LMsgSC(MSG_S_2_C_LIANGPAI)
    {
        m_liangNum = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_liangNum), m_liangNum);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgS2CLiangPai>(); }
};

//服务器通知所有客户端亮牌的消息
struct LMsgS2CLiangPaiRet : public LMsgSC
{
    int m_pos;         //亮牌玩家的位置
    int m_liangNum;	//玩家是否选择亮牌

    LMsgS2CLiangPaiRet() : LMsgSC(MSG_S_2_C_LIANGPAI_RET)
    {
        m_pos = -1;
        m_liangNum = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 3);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_pos), m_pos);
        WriteKeyValue(pack, NAME_TO_STR(m_liangNum), m_liangNum);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgS2CLiangPaiRet>(); }
};

//客户端返回亮牌选择结果
struct LMsgC2SLiangPai : public LMsgSC
{
    int m_liangResult;	//0 -- 不亮， 1 -- 亮

    LMsgC2SLiangPai() :LMsgSC(MSG_C_2_S_LIANGPAI)
    {
        m_liangResult = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_liangResult), m_liangResult);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgC2SLiangPai>(); }
};

//玩家请求再来一局
struct LMsgC2SRematch :public LMsgSC
{
    int			m_flag;					//房间类型，1-2圈，2-四圈，3-8圈
    int			m_payment;				//付费方式，1 - 房主支付，2 - 平均支付， 3 - 大赢家支付
    int			m_nIPAntiCheat;			//IP防作弊
    int			m_nGPSEarlyWarning;		//GPS预警
    std::string	m_secret;				//房间密码，如果为空，服务器会随机一个密码
    int			m_gold;					//房间抵住，单位元，最低为1元，最多不能超过100
    int			m_state;				//玩法规则: 0 转转  1  长沙  101-血战到底  102-血流成河
    int			m_robotNum;				// 0,不加机器人，1，2，3加机器人数量
    int        	m_credits;              //最大信用值
    std::string	m_GPSLat;				//GPS经度
    std::string	m_GPSLng;				//GPS纬度
    CardValue	m_cardValue[CARD_CONFIG];
    std::vector<int> m_playType;		//玩法: 1-自摸加底 2-自摸加番
    std::string	m_rematchKey;			//再来一局key 上一局房间id+玩家id(东)+玩家id(南)+玩家id(西)+玩家id(北)

    LMsgC2SRematch() :LMsgSC(MSG_C_2_S_REMATCH)
    {
        m_flag = 0;
        m_gold = 0;
        m_state = 0;
        m_credits = 0;
        m_robotNum = 0;
        m_payment = 0;
        m_nGPSEarlyWarning = 0;
        m_nIPAntiCheat = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
        ReadMapData(obj, NAME_TO_STR(m_flag), m_flag);
        ReadMapData(obj, NAME_TO_STR(m_secret), m_secret);
        ReadMapData(obj, NAME_TO_STR(m_gold), m_gold);
        ReadMapData(obj, NAME_TO_STR(m_state), m_state);
        ReadMapData(obj, NAME_TO_STR(m_robotNum), m_robotNum);
        ReadMapData(obj, NAME_TO_STR(m_credits), m_credits);
        msgpack::object array;
        ReadMapData(obj, NAME_TO_STR(m_cardValue), array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            for (Lsize i = 0; i < array.via.array.size && i < CARD_CONFIG; ++i)
            {
                msgpack::object& pv = *(array.via.array.ptr + i);
                pv.convert(m_cardValue[i]);
            }
        }
        ReadMapData(obj, NAME_TO_STR(m_playType), array);
        if (array.type == msgpack::v1::type::ARRAY)
        {
            //int m_count = array.via.array.size;
            for (Lsize i = 0; i < array.via.array.size; ++i)
            {
                int v;
                msgpack::object& obj = *(array.via.array.ptr + i);
                obj.convert(v);
                m_playType.push_back(v);
            }
        }
        ReadMapData(obj, NAME_TO_STR(m_payment), m_payment);
        ReadMapData(obj, NAME_TO_STR(m_nIPAntiCheat), m_nIPAntiCheat);
        ReadMapData(obj, NAME_TO_STR(m_nGPSEarlyWarning), m_nGPSEarlyWarning);
        ReadMapData(obj, NAME_TO_STR(m_GPSLat), m_GPSLat);
        ReadMapData(obj, NAME_TO_STR(m_GPSLng), m_GPSLng);
        ReadMapData(obj, NAME_TO_STR(m_rematchKey), m_rematchKey);
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgC2SRematch>(); }
};

//玩家请求再来一局结果
struct LMsgS2CRematchRet :public LMsgSC
{
    int		m_errorCode;		//0-成功，1-房卡不够，2-玩家已有存在的桌子，3-桌子号已被使用,4-创建桌子失败,5-创建参数有问题,6-人品不够,信用有误，7-超过代开房间上线数量 8-支付方式不对

    LMsgS2CRematchRet() :LMsgSC(MSG_S_2_C_REMATCH)
    {
        m_errorCode = 0;
    }

    virtual void ReadPack(msgpack::object& obj)
    {
    }

    virtual void WritePack(msgpack::packer<msgpack::sbuffer>& pack)
    {
        WriteMap(pack, 2);
        WriteKeyValue(pack, NAME_TO_STR(m_msgId), m_msgId);
        WriteKeyValue(pack, NAME_TO_STR(m_errorCode), m_errorCode);
    }

    virtual LMsgPtr Clone() { return std::make_shared<LMsgS2CRematchRet>(); }
};


#endif //PROJECT_LMSGCS_H
