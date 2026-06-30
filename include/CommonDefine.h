//
// Created by root on 9/7/17.
//

#ifndef PROJECT_COMMONDEFINE_H
#define PROJECT_COMMONDEFINE_H


static const int RECV_FROM_CLIENT = 101; //接收客户端数据
static const int RECV_FROM_SERVER = 102; //接收服务端数据
static const int RECV_FROM_HTTP   = 103; //接收http数据

#define MD5_SIGN        "e3770a44b79aa536e3519a561da2f891";
#define PLATE_WECHAT    "wechat"
#define PLATE_LOCAL     "local"

/////////////////////////////////////////////////////////////////////////////////
//房卡类型
enum CARD_TYPE
{
    CARD_TYPE_4 = 1,        //4局卡
    CARD_TYPE_8 = 2,        //8局卡
    CARD_TYPE_16 = 3,       //16局卡
    CARD_TYPE_ZS = 100,     //数量（钻石直接使用数量）
};

//分别对应于上面需要的房卡数
enum CARD_NEED_TYPE
{
    CARD_NEED_4 = 2,
    CARD_NEED_8 = 4,
    CARD_NEED_16 = 8
};

//付费方式
enum PAY_TYPE
{
    PAY_TYPE_HOMEOWNER = 1,	//房主付费
    PAY_TYPE_AVERAGE = 2,	//平均付费
    PAY_TYPE_WINNER = 3		//大赢家付费
};

//房卡操作类型
enum CARDS_OPER_TYPE
{
    CARDS_OPER_TYPE_INIT = 0,           //新手赠送
    CARDS_OPER_TYPE_CHARGE = 1,         //充值活动
    CARDS_OPER_TYPE_FREE_GIVE = 2,      //免费赠送
    CARDS_OPER_TYPE_CREATE_ROOM = 3,    //创建房间扣除
    CARDS_OPER_TYPE_ACTIVE = 4,		    //活动相关房卡
    CARDS_OPER_TYPE_INVITED = 5,        //邀请好友
    CARDS_OPER_TYPE_EXCHANGE = 6,       //换金币
    CARDS_OPER_TYPE_BIND_PHONE = 7,     //增加绑定手机送钻石类型
    CARDS_OPER_TYPE_BIND_AGENTID = 8,   //增加绑定邀请码送钻石类型
    CARDS_OPER_TYPE_RETURN = 9,         //代开房功能，返回钻石类型
    CARDS_OPER_TYPE_WECHAT_SHARED = 10, //微信分享送钻石
};

enum COINS_OPER_TYPE
{
    COINS_OPER_TYPE_DEBUG = -1,//新手赠送
    COINS_OPER_TYPE_CHARGE = 0,//充值
    COINS_OPER_TYPE_FREE_GIVE = 1,//免费赠送
    COINS_OPER_TYPE_RESULT = 2,//房间结算
    COINS_OPER_TYPE_SELF_GIVE = 3,	//玩家自己领取
    COINS_OPER_TYPE_CHANGE_CARD = 4,   //换卡
};

/////////////////////////////////////////////////////////////////////////////////
/// 公共心跳包
enum HEARBEAT_WHO
{
    HEARBEAT_WHO_UNKNOWN = 0,
    HEARBEAT_WHO_CENTER,
    HEARBEAT_WHO_LOGICMANAGER,
    HEARBEAT_WHO_LOGICSERVER,
    HEARBEAT_WHO_LOGINSERVER,
    HEARBEAT_WHO_GATESERVER,
    HEARBEAT_WHO_LOGICDB,
    HEARBEAT_WHO_COINSSERVER,
    HEARBEAT_WHO_NUM
};
const char* const HearBeat_Name[HEARBEAT_WHO_NUM] =
        { "Unknown", "Center", "Manager", "Logic", "Login", "Gate", "DB", "Coins"};


/// 玩家状态信息定义
enum LGUserMsgState
{
    LGU_STATE_CENTER,		//玩家在大厅，发到LogicManager
    LGU_STATE_DESK,			//玩家在桌子，发到Logic
    LGU_STATE_COIN,			//玩家在金币场，发到CoinsServer
    LGU_STATE_COINDESK,		//玩家在金币场桌子，发到Logic
};
/////////////////////////////////////////////////////////////////////////////////
enum MSG_ID
{
    MSG_ERROR_MSG                       = 0, //错误消息

    /**** 客户端与服务端之间的消息号 ****/
    MSG_C_2_S_MSG                       = 1, //客户端发给服务器
    MSG_S_2_C_MSG                       = 2, //服务器返回结果

    MSG_C_2_S_LOGIN                     = 11,//客户端请求登录
    MSG_S_2_C_LOGIN                     = 12,//服务器返回客户登录结果
    MSG_S_2_C_ITEM_INFO                 = 13,//返回玩家物品信息
    MSG_S_2_C_HORSE_INFO                = 14,//跑马灯信息
    MSG_C_2_S_HEART                     = 15,//心跳包
    MSG_S_2_C_HEART                     = 16,//心跳包
    MSG_C_2_S_NOTICE                    = 17,//请求服务器通知
    MSG_S_2_C_NOTICE                    = 18,//服务器返回通知

    MSG_C_2_S_CREATE_ROOM 				= 20,//玩家请求创建房间
    MSG_S_2_C_CREATE_ROOM 				= 21,//玩家请求创建房间结果
    MSG_C_2_S_ADD_ROOM 					= 22,//玩家请求加入房间
    MSG_S_2_C_ADD_ROOM 					= 23,//玩家请求加入房间结果
    MSG_C_2_S_LEAVE_ROOM 				= 24,//玩家请求退出房间
    MSG_S_2_C_LEAVE_ROOM 				= 25,//玩家请求退出房间结果
    MSG_C_2_S_RESET_ROOM 				= 26,//申请解算房间
    MSG_S_2_C_RESET_ROOM 				= 27,// 服务器广播申请解算房间结果
    MSG_C_2_S_RESET_ROOM_SELECT 		= 28,//玩家执行申请操作

    MSG_S_2_C_INTO_DESK 				= 30,//服务器通知玩家进入桌子
    MSG_S_2_C_ADD_USER 					= 31,//桌子添加玩家
    MSG_S_2_C_DEL_USER 					= 32,//桌子删除玩家

    MSG_S_2_C_DESK_STATE 				= 35,//发送桌子状态
    MSG_C_2_S_READY 					= 36,//客户端玩家准备
    MSG_S_2_C_READY 					= 37,//服务器广播某个玩家准备
    MSG_C_2_S_BUSY                      = 38,//客户端忙碌
    MSG_S_2_C_BUSY                      = 39,//服务器广播玩家忙碌状态

    MSG_S_2_C_USER_ONLINE_INFO 			= 40,//服务器广播玩家在线状态
    MSG_S_2_C_VIP_INFO 					= 41,//同步桌子信息
    MSG_C_2_S_COINS_ROOM_LIST  			= 42,//预留
    MSG_S_2_C_COINS_ROOM_LIST  			= 43,//预留
    MSG_C_2_S_ENTER_COINS_ROOM 			= 44,//玩家请求进入金币房间
    MSG_S_2_C_ENTER_COINS_ROOM 			= 45,//玩家请求创建房间结果

    MSG_S_2_C_START 					= 50,//服务器发送游戏开始消息
    MSG_S_2_C_OUT_CARD                  = 51,//服务器发送玩家思考选项，选择后准备出牌的消息
    MSG_C_2_S_PLAY_CARD 				= 52,//客户端请求出牌
    MSG_S_2_C_PLAY_CARD 				= 53,//服务器广播玩家出牌
    MSG_S_2_C_USER_THINK 				= 54,//服务器通知玩家思考
    MSG_C_2_S_USER_OPER 				= 55,//玩家操作结果
    MSG_S_2_C_USER_OPER 				= 56,//服务器广播玩家操作结果
    MSG_C_2_S_USER_SPEAK 				= 57,//玩家请求说话
    MSG_S_2_C_USER_SPEAK 				= 58,//服务器广播玩家说话
    MSG_S_2_C_HU_CARD	 				= 59,//胡牌通知消息
    MSG_S_2_C_GAME_OVER 				= 60,//服务器通知本场游戏结束
    MSG_C_2_S_USER_OUT_ROOM 			= 61,//玩家请求退出房间
    MSG_S_2_C_USER_OUT_ROOM 			= 62,//玩家退出房间结果
    MSG_C_2_S_USER_AIOPER 				= 63,//客户端请求自动决策
    MSG_S_2_C_USER_AIOPER 				= 64,//服务器自动帮决策/出牌
    MSG_S_2_C_START_HU 					= 65,//玩家起手胡
    MSG_C_2_S_START_HU_SELECT 			= 66,//玩家选择起手胡
    MSG_S_2_C_START_HU_SELECT 			= 67,//玩家选择起手胡结果
    MSG_S_2_C_GANG_RESULT 				= 68,//玩家杠牌结果
    MSG_S_2_C_END_CARD_SELECT 			= 69,//服务器广播海底牌选择
    MSG_C_2_S_END_SELECT 				= 70,//玩家选择结果
    MSG_S_2_C_END_GET 					= 71,//服务器广播玩家摸海底牌
    MSG_S_2_C_REMOVE_QIANGGANG_CARD 	= 72,//移除抢杠胡的手牌
    MSG_S_2_C_FANPIGU_BUPAI 			= 73,//服务器发送翻屁股补牌消息
    MSG_C_2_S_FANPIGU_BUPAI_RESULT 		= 74,//玩家翻屁股补牌结果消息
    MSG_S_2_C_FANPIGU_BROADCAST 		= 75,//翻屁股新牌广播
    MSG_S_2_C_GET_CARD                  = 76,//摸牌
    MSG_S_2_C_VIP_END 					= 80,//游戏结束

    MSG_C_2_S_VIP_LOG 					= 90,//玩家请求vip房间对账单
    MSG_S_2_C_VIP_LOG 					= 91,//玩家请求vip房间对账单结果
    MSG_C_2_S_VIDEO 					= 92,//玩家请求录像
    MSG_S_2_C_VIDEO 					= 93,//玩家请求录像
    MSG_S_2_C_ACTIVE_INFO = 94,				//通知客户端活动状态  已作废
    MSG_C_2_S_REQUEST_DRAW = 95,			//客户端请求抽卡   已作废
    MSG_S_2_C_FINISH_DRAW = 96,				//通知客户端抽卡结果  已作废
    MSG_C_2_S_WRITE_PHONE = 97,				//客户端请求写入电话  已作废
    MSG_S_2_C_WRITE_PHONE = 98,				//通知客户端写入电话结果  已作废
    MSG_C_2_S_ACTIVE_LOG = 99,				//客户端请求活动日志  已作废
    MSG_S_2_C_ACTIVE_LOG = 100,				//返馈客户端参与活动日志  已作废
    MSG_C_2_S_ACTIVE_INFO = 101,			//客户端请求活动信息  已作废
    MSG_S_2_C_ACTIVE_OPEN = 102,			//通知客户端当前是否有活动  已作废

    MSG_S_2_C_USER_DINGQUE_START = 110,			//通知玩家定缺
    MSG_C_2_S_USER_DINGQUE = 111,			//玩家回复定缺

    MSG_S_2_C_USER_CHANGE = 112,			//通知玩家换牌
    MSG_C_2_S_USER_CHANGE = 113,			//玩家回复换牌
    MSG_S_2_C_USER_CHANGERESULT = 114,		//玩家换牌结果
    MSG_S_2_C_GAMEREULT = 115,				//单局结算信息

    MSG_S_2_C_USER_DINGQUE_COMPLETE = 116,	//所有玩家定缺完毕

    MSG_C_2_S_ROOM_LOG = 117,//玩家请求vip房间对账单
    MSG_S_2_C_ROOM_LOG = 118,//玩家请求vip房间对账单结果

    MSG_C_2_S_REMATCH = 119,				//玩家请求再来一局
    MSG_S_2_C_REMATCH = 120,				//玩家请求再来一局结果

    MSG_C_2_S_GET_USER_INFO = 130,		// 请求获取指定玩家的信息
    MSG_S_2_C_USER_INFO = 131,			// 服务器返回玩家信息

    MSG_C_2_S_GET_INVITING_INFO = 132,	      // 请求获取邀请信息
    MSG_S_2_C_INVITING_INFO = 133,		      // 服务器返回邀请信息

    MSG_C_2_S_BIND_INVITER = 134,		      // 请求绑定邀请人
    MSG_S_2_C_BIND_INVITER = 135,		      // 服务器返回绑定邀请人结果
    MSG_C_2_S_USER_SHARED = 136,		// 玩家进行了分享
    MSG_C_2_S_REQ_LUCKY_DRAW_NUM = 137,	// 请求玩家抽奖次数
    MSG_S_2_C_LUCKY_DRAW_NUM = 138,		// 服务器推送玩家抽奖次数

    MSG_C_2_S_TANGCARD = 139,			// 玩家发起廊起请求消息
    MSG_S_2_C_TANGCARD_RET = 140,		// 服务器发回廊起请求验证结果

    MSG_C_2_S_REQ_CREHIS = 141,         //玩家请求点赞记录
    MSG_S_2_C_RET_CREHIS = 142,         //服务器返回客户端 点赞记录

    MSG_C_2_S_REQ_GTU = 143,           //客户端 点赞
    MSG_S_2_C_RET_GTU = 144,           //服务器返回点赞结果

    MSG_C_2_S_REQ_EXCH = 145,          //换金币
    MSG_S_2_C_RET_EXCH = 146,          //结果

    MSG_C_2_S_XIAZUI = 147,			// 玩家发起下嘴请求消息(徐州)
    MSG_S_2_C_XIAZUI_RET = 148,		// 服务器发回下嘴请求验证结果(徐州)

    ///client 跟 server 之间消息交互
    MSG_C_2_S_LOGIN_CENTER = 150,//客户端请求登录中心服务器
    MSG_S_2_C_LOGIN_CENTER = 150,//客户端请求登录中心服务器

    MSG_S_2_C_NOTICE_YISE = 151,		//通知客户端进行一色提示
    MSG_C_2_S_SELECT_YISE_RET = 152,	//客户端选择一色结果返回服务器
    MSG_S_2_C_SELECT_YISE_RET = 153,	//服务器广播一色的选择结果

    ///client and gate
    MSG_C_2_S_LOGIN_GATE = 170,	//客户端登录Gate
    MSG_S_2_C_LOGIN_GATE = 171,	//Gate回客户端登录消息

    ///gate 到 client 切断连接消息
    MSG_S_2_C_SHUTDOWN_GATE = 180,//切断客户端连接消息

    /// 活动 200 - 300
    MSG_S_2_C_ACTIVITY_INFO = 200,			//通用的活动内容 推动给客户端
    MSG_C_2_S_ACTIVITY_WRITE_PHONE = 201,	//填写活动相关的电话号码
    MSG_S_2_C_ACTIVITY_WRITE_PHONE = 202,
    MSG_C_2_S_ACTIVITY_REQUEST_LOG = 203,	//请求中奖纪录
    MSG_S_2_C_ACTIVITY_REPLY_LOG = 204,		//返回中奖纪录

    /// 转盘
    MSG_C_2_S_ACTIVITY_REQUEST_DRAW_OPEN = 211,	//请求打开转盘
    MSG_S_2_C_ACTIVITY_REPLY_DRAW_OPEN = 212,	//返回请求打开转盘
    MSG_C_2_S_ACTIVITY_REQUEST_DRAW = 213,	//客户端请求抽卡
    MSG_S_2_C_ACTIVITY_REPLY_DRAW = 214,	//通知客户端抽卡结果

    /// 分享
    MSG_C_2_S_ACTIVITY_REQUEST_SHARE = 220,	//客户端请求分享
    MSG_S_2_C_ACTIVITY_FINISH_SHARE = 221,	//通知客户端分享结果
    MSG_S_2_C_KICK_USER = 222, //玩家被封号时通知客户端该玩家被踢出桌子
    MSG_S_2_C_DESK_LIST_UPDATE = 223, //桌子信息改变了的消息

    MSG_S_2_C_LIANGPAI = 230,       //服务器告诉可以亮牌的消息
    MSG_S_2_C_LIANGPAI_RET = 231,	//服务器通知所有客户端亮牌结果
    MSG_C_2_S_LIANGPAI = 232,	//客户端返回亮牌结果

    MSG_C_2_S_VERIFY_HEAD               = 500,      //客户端包头验证头
    MSG_S_2_C_MAX 						= 4096, 	//服务器客户端之间最大消息号


    /**** 服务端与服务端之间的消息号 ****/
    ///logic/logicM 跟 center 之间的交互
    MSG_L_2_CE_LOGIN = 6000, //logicmanager登录center
    MSG_L_2_CE_GATE_INFO = 6002, //logicmanager同步gate信息到center
    MSG_CE_2_L_USER_ID_INFO = 6003, //center同步玩家id信息
    MSG_CE_2_L_USER_LOGIN = 6004, //center同步玩家请求登录的消息
    MSG_L_2_CE_MODIFY_USER_NEW = 6005, //logic同步玩家更新的消息
    MSG_L_2_CE_ROOM_SAVE = 6006, //logic同步更新room的消息s
    MSG_L_2_CE_USER_LOGIN = 6011,			//logicm同步Center玩家当前登陆某服务器
    MSG_L_2_CE_USER_LOGOUT = 6012,			//logicm同步Center玩家当前登出某服务器
    MSG_CE_2_L_REQ_LOG_INFO = 6013, //center发送玩家请求战绩信息			---已废
    MSG_L_2_CE_REQ_VIDEO = 6014, //logic发送玩家请求录像					---已废
    MSG_CE_2_L_REQ_VIDEO = 6015, //center发送玩家请求录像					---已废
    MSG_CE_2_L_GM_CHARGE = 6020, //center发送gm充值请求
    MSG_CE_2_L_GM_HORSE = 6021, //center发送gm跑马灯
    MSG_CE_2_L_GM_BUYINFO = 6022, //center发送gm够买提示
    MSG_CE_2_L_GM_HIDE = 6023, //center发送gmyincang
    MSG_CE_2_L_GM_COINS = 6024, //center发送gm充值金币
    MSG_CE_2_L_GM_FENGHAO = 6025, //center发送gm封号请求
    MSG_CE_2_L_GM_DISSOLVE_ROOM = 6026, //强制解散房间
    MSG_LMG_2_CE_SAVE_CARD = 6400, //logicmanager发给center来保存房卡		---暂时未增加该功能
    MSG_CE_2_L_SET_GAME_FREE = 6401,	//设置游戏免费
    MSG_CE_2_L_SET_DRAWACTIVE = 6402,	//设置抽奖活动配置 已作废
    MSG_CE_2_L_SET_PXACTIVE = 6403,		//设置牌型活动配置
    MSG_CE_2_L_SET_OUGCACTIVE = 6404,	//设置老玩家送卡活动配置
    MSG_CE_2_L_SET_EXCHACTIVE = 6405,	//设置老玩家送卡活动配置
    MSG_CE_2_L_SET_ACTIVITY = 6406,		//设置活动

    ///gete 跟 logic 之间的交互
    MSG_G_2_L_LOGIN = 7000, //gate登录logic		由LogicManager转发
    MSG_G_2_L_USER_MSG = 8000, //gate转发玩家消息到logic
    MSG_L_2_G_USER_MSG = 8001, //logic转发玩家消息到gate
    MSG_G_2_L_USER_OUT_MSG = 8002, //gate转发玩家退出消息到logic
    MSG_L_2_G_USER_OUT_MSG = 8003, //logic转发玩家退出消息到gate
    MSG_L_2_G_MODIFY_USER_STATE = 8006,	//修改玩家状态 gate to logic

    ///LogicManager 与 Logic
    MSG_L_2_LMG_LOGIN = 11001,				//逻辑管理登陆
    MSG_LMG_2_L_RLOG_INFO = 11002,			//远程日志信息
    MSG_LMG_2_L_CREATE_DESK = 11011,		//创建房间	只有玩家不在Logic上才创建
    MSG_LMG_2_L_ADDTO_DESK = 11013,			//加入房间	只有玩家不在Logic上才加入
    MSG_LMG_2_L_USER_LOGIN = 11016,			//玩家重新登陆
    MSG_L_2_LMG_MODIFY_USER_STATE = 11017,			//修改玩家状态  logicmanager to logic
    MSG_L_2_LMG_MODIFY_USER_CARD = 11018,			//修改玩家卡数
    MSG_L_2_LMG_ADD_USER_PLAYCOUNT = 11019,			//修改玩家把数
    MSG_L_2_LMG_RECYLE_DESKID = 11020,				//创建失败，或桌子解散时
    MSG_LMG_2_L_PAIXING_ACTIVE = 11021,				//LMtoL牌型活动信息
    MSG_LMG_2_L_EXCH_CARD = 11022,
    MSG_LMG_2_L_KICK_DESK = 11023,    //被封号时踢出桌子消息类型
    MSG_L_2_LMG_DESK_LIST_UPDATE = 11024, //桌子信息改变了的消息
    MSG_LMG_2_L_GM_DISSOLVE_ROOM = 11025, //强制解散房间
    MSG_LMG_2_L_REMATCH = 11026,				//玩家请求再来一局
    MSG_L_2_LMG_REMATCH = 11027,				//玩家请求再来一局结果

    /// LogicManager 与 Gate
    MSG_G_2_LMG_LOGIN = 12001,			//逻辑管理登陆
    MSG_LMG_2_G_SYNC_LOGIC = 12010,		//同步逻辑信息
    MSG_LMG_2_G_HORSE_INFO = 12011,		//跑马灯

    /// LogicManager 与 Gate 与 Logic
    MSG_LMG_2_GL_COINS_SERVER_INFO = 13001,     //金币服务器的信息

    /// logicmanager 和 logicdb 交互消息
    MSG_LMG_2_LDB_USER_LOGIN = 14901,	//用户登陆
    MSG_LDB_2_LMG_USER_MSG   = 14902,	//发送给用户的消息
    MSG_LMG_2_LDB_LOGIN = 14999,	//LogicManager 登入LogicDB

    /// logic 和 logicdb 直接的消息交互
    MSG_L_2_LDB_LOGIN = 15000, //logic 登录 logicdb
    MSG_L_2_LDB_VIP_LOG = 15001,//logic 请求 vip战绩
    MSG_LDB_2_L_VIP_LOG = 15002,//logicdb 返回 vip战绩
    MSG_L_2_LDB_VIDEO  = 15003,//logic 请求 video
    MSG_LDB_2_L_VIDEO  = 15004,//logicdb 返回 video
    MSG_L_2_LDB_ROOM_SAVE = 15005, //logic同步更新room的消息
    MSG_L_2_LDB_VIDEO_SAVE = 15006, //logic同步更新video的消息
    MSG_L_2_LDB_REQ_ACTIVE = 15007,		//Logic请求活动  已作废
    MSG_LDB_2_L_REQ_ACTIVE = 15008,		//Center返回请求活动  已作废
    MSG_L_2_LDB_SYC_DRAW = 15009,			//Center返回请求活动  已作废
    MSG_L_2_LDB_SYC_PHONE = 15010,		//Center返回请求活动  已作废
    MSG_LDB_2_L_LOG_INFO = 15011, //center同步未完成的桌子信息
    MSG_LDB_2_L_ROOM_LOG = 15015,//logicdb 返回room vip战绩
    MSG_LM_2_LDB_BIND_INVITER = 15016,		      // 请求绑定邀请人
    MSG_LDB_2_LMG_REQ_BINDING_RELATIONSHIPS = 15017,
    MSG_LM_2_LDB_REQ_INFO = 15020,         //
    MSG_LDB_2_LM_RET_INFO = 15021,
    MSG_L_2_LDB_CRE_LOG = 15022,//logic 请求 vip战绩
    MSG_L_2_LDB_ENDCRE_LOG = 15024,      //logicdb保存 点赞日志
    MSG_LDB_2_LM_RET_CRELOGHIS = 15025,  //logicdb返回点赞记录
    MSG_L_2_LDB_REQ_DELGTU = 15026,      //logicdb删除点赞记录

    /// LogicManager 与 CoinsServer
    MSG_CN_2_LMG_LOGIN = 16001,			//金币服务登陆Manager
    MSG_LMG_2_CN_LOGIN = 16002,
    MSG_CN_2_LMG_FREE_DESK_REQ = 16003,		//请求桌子号
    MSG_LMG_2_CN_FREE_DESK_REPLY = 16004,	//请求桌子回复
    MSG_CN_2_LMG_RECYCLE_DESK  = 16005,		// 释放一些桌子
    MSG_LMG_2_CN_USER_LOGIN = 16007,	//玩家在桌内时的登陆
    MSG_LMG_2_CN_ENTER_COIN_DESK = 16011,	//
    MSG_CN_2_LMG_MODIFY_USER_COINS = 16012,	//修改玩家金币
    MSG_LMG_2_CN_GET_COINS = 16013,		 //玩家领取金币 Manager转发的消息

    /// CoinsServer 与 Gate
    MSG_G_2_CN_LOGIN = 16101,			//逻辑管理金币服务器

    /// CoinsServer 与 Logic
    MSG_L_2_CN_LOGIN = 16201,			//逻辑管理金币服务器

    MSG_CN_2_L_CREATE_COIN_DESK = 16211,  //创建金币桌子
    MSG_L_2_CN_CREATE_COIN_DESK_RET = 16212,  //创建桌子返回
    MSG_L_2_CN_GAME_RESULT = 16213,         //单局结算
    MSG_L_2_CN_END_COIN_DESK = 16214,     //结算

    /// CoinServer 与 Client
    MSG_C_2_S_GOON_COINS_ROOM = 16401,  //玩家金币场继续游戏
    MSG_S_2_C_GOON_COINS_ROOM = 16402,  //玩家金币场继续游戏 回复
    MSG_S_2_C_KICK_COINS_ROOM = 16403,	 //玩家被踢出房间
    MSG_S_2_C_GIVE_COINS = 16404,		 //给玩家赠送金币 服务器主动
    MSG_C_2_S_GET_COINS = 16405,		 //玩家领取金币
    MSG_S_2_C_GET_COINS = 16406,		 //返回玩家领取金币
    MSG_S_2_C_OTHER_COINS = 16407,		 //返回玩家领取金币

    ///LM 2 CENTER
    MSG_LM_2_CEN_ADD_CRE = 16800,    //点赞
    MSG_C_2_C_ADD_CRE = 16801,       //点赞

    /// LoginServer <=> CenterServer
    MSG_FROM_LOGINSERVER                = 20000,    //来自login server 的消息
    MSG_TO_LOGINSERVER                  = 20001,	//回给login server 的消息

    /// 公用消息号
    MSG_HEARDBEAT_REQUEST               = 50100,	//心跳请求
    MSG_HEARDBEAT_REPLY	                = 50101,	//心跳应答
    MSG_NEWUSER_VERIFY_REQUEST          = 50200,	//新用户校验请求
    MSG_NEWUSER_VERIFY_REPLY            = 50201,	//新用户校验返回


    /**** 程序内部使用消息号 ****/
    MSG_CONVERT_CLIENTMSG               = 65000,    //客户端数据包转换
    MSG_HTTP_REQ                        = 65500,
    MSG_SOCKET_IN                       = 65534,
    MSG_SOCKET_OUT                      = 65535,
};


#endif //PROJECT_COMMONDEFINE_H
