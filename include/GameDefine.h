//
// Created by root on 9/20/17.
//

#ifndef PROJECT_GAMEDEFINE_H_H
#define PROJECT_GAMEDEFINE_H_H

#if defined(DEBUG) || defined(_DEBUG)
// 定义这个宏表示 不 启用心跳检查
#define UNCHECKHEARTBEAT
#endif

#define CARD_COUNT (108+16+12)	// 万筒条 东南西北 中发白
#define CARD_CONFIG 136		    // 客户端配牌的大小

enum CARD_COLOR
{
    CARD_COLOR_WAN = 1, // 万
    CARD_COLOR_TUO = 2,	// 饼
    CARD_COLOR_SUO = 3, // 条
    CARD_COLOR_ZI  = 4, // 东南西北中发白
};

//玩家思考类型
enum THINK_OPERATOR
{
    THINK_OPERATOR_DINGQUE = -3,	//定缺
    THINK_OPERATOR_CHANGE = -2,
    THINK_OPERATOR_REOUT = -1, // 断线续完再次告诉玩家打出去的牌
    THINK_OPERATOR_NULL  = 0,	//空操作
    THINK_OPERATOR_OUT   = 1,	//出牌
    THINK_OPERATOR_BOMB  = 2, // 胡
    THINK_OPERATOR_AGANG = 3, // 暗杠	目前没用到
    THINK_OPERATOR_MGANG = 4, // 明杠
    THINK_OPERATOR_PENG  = 5, // 碰
    THINK_OPERATOR_CHI   = 6, // 吃     目前没用到
    THINK_OPERATOR_ABU   = 7, // 暗杠
    THINK_OPERATOR_MBU   = 8, // 补杠
    THINK_OPERATOR_DIANGANGPAO = 9,// 玩家点杠后自摸胡算点炮胡操作
    THINK_OPERATOR_QIANGANGH = 10, //抢杠胡
    THINK_OPERATOR_TINGCARD = 11,  //听牌
    THINK_OPERATOR_FLY      = 12,   //飞
    THINK_OPERATOR_RETURN   = 13,   //提
    THINK_OPERATOR_MUSTHU   = 14,   //必须胡
};
#define DESK_USER_COUNT 4	//一桌玩家数量

#define INVAILD_POS		4  //无效的位置

#define	PLAY_CARD_TIME		10  //出牌时间
#define WAIT_READY_TIME		15  //准备时间

//桌子当前状态
enum DESK_STATE
{
    DESK_FREE =		0,	//桌子空闲状态
    DESK_WAIT,			//桌子等待状态
    DESK_PLAY,			//正在游戏中
    //	DESK_COUNT_RESULT,	//正在结算中
    //	DESK_RESET_ROOM		//正在申请解散房间中
};

//牌局当前状态
enum DESK_PLAY_STATE
{
    DESK_PLAY_START_HU,		//起手胡状态
    DESK_PLAY_GET_CARD,		//摸牌
    DESK_PLAY_THINK_CARD,	//思考
    DESK_PLAY_END_CARD,		//海底牌
    DESK_PLAY_DINGQUE,		//定缺
    DESK_PLAY_CHANGE,		//换牌
    DESK_PLAY_FANPIGU,		//翻屁股补牌
    DESK_PLAY_DINGPIAO      //PIAO
};

//家胡牌情况，0-没胡，1-自摸，2-收炮，3-点炮
enum WIN_TYPE
{
    WIN_INVALID = -1,
    WIN_ZIMO = 0,	//自摸
    WIN_BOMB,		//点炮
    WIN_NONE,		//慌庄
    WIN_DISS,       //解散房间
    WIN_BREAK,      //
};

//胡牌方式
enum WIN_TYPE_SUB
{
    WIN_SUB_NONE = 0,
    WIN_SUB_ZIMO = 1,		//自摸
    WIN_SUB_BOMB = 2,		//收炮
    WIN_SUB_ABOMB = 3,		//点炮

    WIN_SUB_DZIMO = 4,		//大胡自摸
    WIN_SUB_DBOMB = 5,		//大胡收炮
    WIN_SUB_DABOMB = 6,		//大胡点炮
    WIN_SUB_TINGCARD = 7,   //有叫 3/4血战，2房
};

//起手胡牌类型
enum START_HU_INFO
{
    START_HU_NULL = 0,
    START_HU_QUEYISE=1,	//缺一色
    START_HU_BANBANHU,		//板板胡
    START_HU_DASIXI,		//大四喜
    START_HU_LIULIUSHUN,	//六六顺
};
//麻将类型
enum GameType
{
    HuBeiMaJiangXiShui		        = 306,	//浠水麻将
    HuBeiMaJiangXinFaCai	        = 307,	//新发财
    HuBeiMaJiangXiShuiHuangHuang    = 316,  //黄石晃晃
};

//胡牌类型
enum HU_PAI_LEIXING
{
    HU_NULL = 0,					//没胡
    HU_XIAOHU = 1,					//小胡

    HU_XIAOQIDUI_SPECIAL = 2,			//豪华小七对 1
    HU_XIAOQIDUI = 3,					//七小对 0
    HU_QINGYISE = 4,					//清一色
    HU_JIANGJIANGHU = 5,				//将将胡
    HU_PENGPENGHU = 6,				//对对胡
    HU_QUANQIUREN = 7,				//全球人
    HU_GANG1 = 8,						//杠上开花
    HU_GANG2 = 9,						//杠上炮
    HU_END1 = 10,						//海底捞(扫底胡)
    HU_END2 = 11,						//海底炮
    HU_QIANGGANGHU = 12,				//抢扛胡
    HU_QISHOUFOURLAN = 13,			//起手四个赖子
    HU_XIAOQIDUI_DOUBLESPECIAL = 14, //超豪华七小队 2
    HU_TIANHU = 15,					//天胡
    HU_DIHU = 16,						//地胡
    HU_DANDIAO = 17,				//单钓
    HU_JINGOUDIAO = 18,				//金钩钓

    HU_QINGDUI = 19,				//清对
    HU_QINGQIDUI = 20,				//清七对
    HU_QINGLONGQIDUI = 21,			//清龙七对

    HU_JIANGDUI = 22,               //将对
    HU_JIANGDUI_7 = 23,              //将7对
    HU_ONENINE = 24,                 //全幺九
    HU_MENQING = 25,				//门清
    HU_NOONENINE = 26,				//断幺九 中张

    HU_CA2TIAO = 27,                //4人2房 卡2条

    HU_CA5XIN = 28,                 //夹心5
    HU_ONE2NINE = 29,               //一条龙
    HU_SISTERPAIRS = 30,            //姊妹对
    HU_XIAOQIDUI_TRIPLESPECIAL = 31,//超超豪华七小队 3

    HU_GOLDGOUPAO = 41,             //宜宾 金钩炮
    HU_NOGOLDCARD = 42,             //无听用
    HU_MIDGOLD = 43,             //本金
    HU_TING = 44,				//报听

    HU_QINGSANDA = 80,				//清三搭
    HU_QIANSI = 81,				//前四
    HU_HOUSI = 82,				//后四

    HU_BADCARD = 101,                //烂牌 字不重复 花色牌隔开两张
    HU_QIXIN = 102,				    //七心 东南西北中发白各一个的烂牌
    HU_YAOCARD = 103,				//幺牌 全字和19组成的牌
    HU_JIAXINWU = 104,				//夹心五（只能听五）4番
    HU_HUNYISE = 105,				//混一色
    HU_DASANYUAN = 106,				//大三元
    HU_XIAOSANYUAN = 107,			//小三元
    HU_SHIFENG = 108,				//十风
    HU_SHISANYAO_FEIXIAOJI = 109,	//十三幺-云南飞小鸡
    HU_LONGZHUABEI = 110,			//龙爪背
    HU_SIYAOJI = 111,				//四幺鸡
    HU_GANGSHANGWUMEIHUA = 112,     //杠上五梅花
    HU_WUJI = 113,					//无鸡 无鬼
    HU_XIAOJIGUIWEI = 114,			//小鸡归位
    HU_DOUBLEGANG = 115,			//两杠

    HU_LIGHT4RET = 116,     //明四归
    HU_SHADE4RET = 117,     //暗四归
    HU_LIGHT4RET_HALF = 118, //明四归 半频道
    HU_SHADE4RET_HALF = 119, //暗四归 半频道

    HU_RUANZIMO = 120,		//软自摸
    HU_YINGZIMO = 121,		//硬自摸
    HU_RUANLAIYOU = 122,	//软癞油
    HU_YINGLAIYOU = 123,	//硬癞油

    HU_JIANGYISE = 124,		//将一色
    HU_YINGHU = 125,		//硬胡
    HU_RUANHU = 126,		//软胡
    HU_FENGYISE = 127,		//风一色
    HU_JIANZIHU = 128,		//见字胡
    //新发财
    HU_FOURLAI = 129,		//四赖胡
    HU_ALLTYPE = 149		//总番型		 --保证最大
};

enum GANG_TYPE
{
    GANG_MINGGANG = 0,			//明杠
    GANG_ANGANG = 1,			//暗杠
    GANG_BUGANG = 2,			//补杠
    GANG_ZHUANYI = 3,			//呼叫转移
    GANG_HANBAOG = 4,          //汉堡杠
    GANG_ALLTYPE = 5,
};

//请求解散房间
enum RESET_ROOM_TYPE
{
    RESET_ROOM_TYPE_WAIT,
    RESET_ROOM_TYPE_AGREE,
    RESET_ROOM_TYPE_REFLUSE,
};

enum  PLAYTYPE
{
    PT_ERROR=-1,
    PT_UNDEFINE=0,
    PT_ZIMOHU=1,
    PT_DIANPAOHU=2,
    PT_KEQIANGGANG=3,
    PT_NEEDWIND=4,			//有风
    PT_QIXIAODUI=5,
    PT_ZHUANGXIAN=6,
    PT_ZHANIAO2=7,
    PT_ZHANIAO4=8,
    PT_ZHANIAO6=9,
    PT_NOTNEEDWIND=10,		//无风

    PT_HUANSANZHANG = 20,	//换三张
    PT_ZUIDAFAN = 21,		//最大翻，不选默认为最小翻
    PT_ZIMOADDD = 22,       //自摸加底
    PT_ZIMOADDFAN = 23,     //自摸加番
    PT_TWOFAN	=	24,		//两翻
    PT_THREEFAN	=	25,		//三翻
    PT_FOURFAN	=	26,		//四翻
    PT_JIANGDUI = 27,       //将对 幺九
    PT_MENZHONG = 28,		//门前清 断幺九(中张)
    PT_DIANGANG	= 29,		//点杠花(点炮)
    PT_DIANGANG_THR = 30,   //点杠花(自摸)
    PT_ONETUOONE	= 31,	// 1拖1
    PT_ONETUOTWO	= 32,	// 1拖2
    PT_THRTUOFIVE	= 33,	// 3拖5

    PT_TIANDIHU = 34,       //天地胡

    //以下4人2房使用
    PT_FT_CARD_7   = 35,       //7张
    PT_FT_CARD_10  = 36,       //10张
    PT_FT_CARD_13  = 37,       //13张
    PT_FT_KA2TIAO  = 38,       //卡2条

    //以下 三人两房 使用
    PT_TT_DBOMB_CANHU = 39,    //点炮可平胡
    PT_TT_DDHU_DOUBLE = 40,    //对对胡2翻
    PT_TT_KFIVE_HEART = 41,    //可选加心五

    //丽江卡心五
    PT_PLAY_TWO_MEN = 42,		//打两门
    PT_PLAY_THR_MEN = 43,       //打三门

    //飞小鸡烂牌选项
    PT_BAD_INTERVAL_TWO = 44,	// 间隔2张以上的烂牌（软烂）
    PT_BAD_147_258_369 = 45,	// 147,258,369烂牌定义(正规烂)

    PT_EIGHTFAN	= 46,			//八翻

    // 宜宾
    PT_FIVE_FAN = 101,   //5番
    PT_SIX_FAN  = 102,   //6
    PT_CAN_DPHU  = 103,   //可以点炮胡
    PT_FLY_PIAO = 104,   //飘

    //my
    PT_2PLAYER_NTANG = 150,   //剩余2家不能t
    PT_HAS_TANG_MUSTHU = 151, //有t必胡

    // 万州
    PT_WANZHOU_XUEZHAN = 160, // 血战
    PT_WANZHOU_YAOJIDAI = 161, // 幺鸡带

    // 泸州
    PT_HONGZHONG_4		= 171,	// 带混红中4个
    PT_HONGZHONG_8		= 172,	// 带混红中8个
    PT_HONGZHONG_12		= 173,	// 带混红中12个

    //全半
    PT_CHANNEL_ALL      = 200,      //全频道
    PT_CHANNEL_HALF     = 201,      //半频道
    PT_FAN_EIGHT        = 202,      //8番封顶
    PT_FAN_SIXTEEN      = 203,      //16
    PT_PIAO             = 204,      //飘
    PT_NOTPIAO          = 205,      //不飘
    PT_BUYHORSE_NOT         = 206,  //不买马
    PT_BUYHORSE_ZM          = 207,  //自摸买马
    PT_BUYHORSE_MING        = 208,  //亮倒买马

    //公安晃晃
    PT_BAN_LAI				= 209,	//半癞
    PT_WU_LAI_DAO_DI		= 210,	//无癞到底
    PT_YI_JIAO_LAI_YOU		= 211,	//一脚癞油

    //武汉晃晃
    PT_YI_LAI_DAO_DI		= 212,  //一赖到底
    PT_JIU_FEN_HU			= 213,  //九分胡
    PT_FENG_DING_1			= 214,	//封顶为30
    PT_FENG_DING_2			= 215,	//封顶为40
    PT_FENG_DING_3			= 216,	//封顶为50
    //	PT_CAN_GUO_SHOU_HU		= 217,	//能过手胡

    //红中赖子杠
    PT_KOU_KOU_FAN			= 218,	//口口翻
    PT_FA_CAI_GANG			= 219,	//发财杠
    PT_QI_PI_SI_LAI			= 220,	//七皮四癞
    PT_QI_HU_BEI_SHU_16		= 221,	//16倍起胡
    PT_QI_HU_BEI_SHU_32		= 222,	//32倍起胡
    PT_QI_HU_BEI_SHU_64		= 223,	//64倍起胡

    //黄冈晃晃 房间类型303
    PT_FAN_THREE =  251,   //3番
    PT_FAN_FOUR  =  252,   //4番
    PT_FAN_FIVE  =  253,   //5番
    PT_GANG_ONE =   254,   //抢杠点炮
    PT_GANG_ALL =   255,   //抢杠承包
    PT_CLOSE_DOOR = 256,   //双赖关门

    //浠水麻将
    PT_DI_FEN_1				= 261,	//一分底分
    PT_DI_FEN_2				= 262,	//二分底分
    PT_QI_DUI_ZI			= 263,	//七对可胡
    PT_QUAN_QIU_REN	= 264,	//全求人可胡

    //新发财
    PT_DI_FEN_5				= 271,	//五分底分
    PT_DI_FEN_10			= 272,	//十分底分
};

enum ErrorCode
{
    ErrorNone = 0,
    UserNotExist = 1,
    TaskNotExist = 2,
    ExceedMaximum = 3,
    AlreadyBinded = 4,
    NotQualified = 5,
    TaskNotFinished = 6,
    Expired = 7,
    BindingSelf = 8,
    Unknown = 100
};


//客户端显示确认按钮项
enum ENUM_AFFIRM_FLAG
{
    ENUM_AFFIRM_FLAG_NONE = 0x00000000,			//没有显示按钮
    ENUM_AFFIRM_FLAG_FENGYISE = 0x00000001,		//显示风一色按钮
    ENUM_AFFIRM_FLAG_JIANGYISE = 0x00000002,	//显示将一色按钮
    ENUM_AFFIRM_FLAG_QINGYISE = 0x00000004		//显示清一色按钮
};

//服务器保存确认结果
enum ENUM_AFFIRM_TYPE
{
    ENUM_AFFIRM_TYPE_NONE = 0,			//没有确认值
    ENUM_AFFIRM_TYPE_FENGYISE = 1,		//确认“风一色”
    ENUM_AFFIRM_TYPE_JIANGYISE = 2,		//确认“将一色"
    ENUM_AFFIRM_TYPE_QINGYISE = 3,		//确认”清一色"
    ENUM_AFFIRM_TYPE_PASS = 4			//确认"过"
};


#endif //PROJECT_GAMEDEFINE_H_H
