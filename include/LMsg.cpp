//
// Created by root on 8/18/17.
//

#include "LMsg.h"
#include "LMsgCS.h"
#include "LMsgSS.h"


void LMsgFactory::InitMsg()
{
    RegistMsg(std::make_shared<LMsgC2SMsg>());
    RegistMsg(std::make_shared<LMsgS2CMsg>());

    RegistMsg(std::make_shared<LMsgC2SHeart>());
    RegistMsg(std::make_shared<LMsgS2CHeart>());

    RegistMsg(std::make_shared<LMsgC2SLoginGate>());
    RegistMsg(std::make_shared<LMsgS2CLoginGate>());
    RegistMsg(std::make_shared<LMsgS2CShutDown>());
    RegistMsg(std::make_shared<LMsgC2SNotice>());
    RegistMsg(std::make_shared<LMsgS2CNotice>());

    RegistMsg(std::make_shared<LMsgC2SLogin>());
    RegistMsg(std::make_shared<LMsgS2CLogin>());
    RegistMsg(std::make_shared<LMsgL2CeUserServerLogin>());
    RegistMsg(std::make_shared<LMsgL2CeUserServerLogout>());

    RegistMsg(std::make_shared<LMsgS2CItemInfo>());
    RegistMsg(std::make_shared<LMsgS2CHorseInfo>());

    RegistMsg(std::make_shared<LMsgS2CIntoDesk>());
    RegistMsg(std::make_shared<LMsgS2CDeskAddUser>());
    RegistMsg(std::make_shared<LMsgS2CDeskDelUser>());
    RegistMsg(std::make_shared<LMsgC2SSelectResetDesk>());
    RegistMsg(std::make_shared<LMsgS2CResetDesk>());
    RegistMsg(std::make_shared<LMsgC2SResetDesk>());

    RegistMsg(std::make_shared<LMsgS2CDeskState>());

    RegistMsg(std::make_shared<LMsgC2SUserReady>());
    RegistMsg(std::make_shared<LMsgS2CUserReady>());

    RegistMsg(std::make_shared<LMsgS2CPlayStart>());
    RegistMsg(std::make_shared<LMsgS2COutCard>());
    RegistMsg(std::make_shared<LMsgC2SUserPlay>());
    RegistMsg(std::make_shared<LMsgS2CUserPlay>());
    RegistMsg(std::make_shared<LMsgS2CThink>());

    RegistMsg(std::make_shared<LMsgC2SUserOper>());
    RegistMsg(std::make_shared<LMsgS2CUserOper>());
    RegistMsg(std::make_shared<LMsgS2CGameOver>());

    RegistMsg(std::make_shared<LMsgC2SUserAIOper>());
    RegistMsg(std::make_shared<LMsgS2CUserAIOper>());

    RegistMsg(std::make_shared<LMsgS2CUserStartHu>());
    RegistMsg(std::make_shared<LMsgC2SUserStartHuSelect>());
    RegistMsg(std::make_shared<LMsgS2CUserStartHuSelect>());

    RegistMsg(std::make_shared<LMsgS2CUserGangCard>());
    RegistMsg(std::make_shared<LMsgS2CUserEndCardSelect>());
    RegistMsg(std::make_shared<LMsgC2SUserEndCardSelect>());
    RegistMsg(std::make_shared<LMsgS2CUserEndCardGet>());

    RegistMsg(std::make_shared<LMsgC2SUserOutRoom>());
    RegistMsg(std::make_shared<LMsgS2CUserOutRoom>());

    RegistMsg(std::make_shared<LMsgC2SLeaveDesk>());
    RegistMsg(std::make_shared<LMsgS2CLeaveDesk>());

    RegistMsg(std::make_shared<LMsgC2SUserSpeak>());
    RegistMsg(std::make_shared<LMsgS2CUserSpeak>());

    RegistMsg(std::make_shared<LMsgC2SCreateDesk>());
    RegistMsg(std::make_shared<LMsgS2CCreateDeskRet>());

    RegistMsg(std::make_shared<LMsgC2SAddDesk>());
    RegistMsg(std::make_shared<LMsgS2CAddDeskRet>());

    RegistMsg(std::make_shared<LMsgC2SEnterCoinsDesk>());
    RegistMsg(std::make_shared<LMsgS2CEnterCoinsDeskRet>());


    RegistMsg(std::make_shared<LMsgS2CVipEnd>());
    RegistMsg(std::make_shared<LMsgC2SVipLog>());
    RegistMsg(std::make_shared<LMsgS2CVipLog>());
    RegistMsg(std::make_shared<LMsgC2SVideo>());
    RegistMsg(std::make_shared<LMsgS2CVideo>());
    RegistMsg(std::make_shared<LMsgS2CRemoveQiangGangCard>());

    RegistMsg(std::make_shared<LMsgC2SCRELog>());
    RegistMsg(std::make_shared<LMsgS2CCRELogRet>());
    RegistMsg(std::make_shared<LMsgC2SREQ_GTU>());
    RegistMsg(std::make_shared<LMsgS2CREQ_GTU_RET>());

    RegistMsg(std::make_shared<LMsgC2SREQ_Exch>());
    RegistMsg(std::make_shared<LMsgS2CREQ_EXCH_RET>());

    RegistMsg(std::make_shared<LMsgS2CActivityInfo>());
    RegistMsg(std::make_shared<LMsgC2SActivityPhone>());
    RegistMsg(std::make_shared<LMsgS2CActivityPhone>());
    RegistMsg(std::make_shared<LMsgC2SActivityRequestLog>());
    RegistMsg(std::make_shared<LMsgS2CActivityReplyLog>());
    RegistMsg(std::make_shared<LMsgC2SActivityRequestDrawOpen>());
    RegistMsg(std::make_shared<LMsgS2CActivityReplyDrawOpen>());
    RegistMsg(std::make_shared<LMsgC2SActivityRequestDraw>());
    RegistMsg(std::make_shared<LMsgS2CActivityReplyDraw>());

    RegistMsg(std::make_shared<LMsgC2SActivityRequestShare>());
    RegistMsg(std::make_shared<LMsgS2CActivityFinishShare>());

    RegistMsg(std::make_shared<LMsgC2SRoomLog>());
    RegistMsg(std::make_shared<LMsgLBD2LReqRoomLog>());
    RegistMsg(std::make_shared<LMsgS2CRoomLog>());

    RegistMsg(std::make_shared<LMsgS2CInvitingInfo>());
    RegistMsg(std::make_shared<LMsgC2SBindInviter>());
    RegistMsg(std::make_shared<LMsgS2CBindInviter>());
    RegistMsg(std::make_shared<LMsgC2SGetUserInfo>());
    RegistMsg(std::make_shared<LMsgC2SGetInvitingInfo>());


    RegistMsg(std::make_shared<LMsgL2CeLogin>());
    RegistMsg(std::make_shared<LMsgL2CeGateInfo>());
    RegistMsg(std::make_shared<LMsgCe2LUserLogin>());
    RegistMsg(std::make_shared<LMsgCe2LUserIdInfo>());

    RegistMsg(std::make_shared<LMsgG2LLogin>());
    RegistMsg(std::make_shared<LMsgG2LUserMsg>());
    RegistMsg(std::make_shared<LMsgL2GUserMsg>());
    RegistMsg(std::make_shared<LMsgL2CeModifyUserNew>());
    RegistMsg(std::make_shared<LMsgL2CeSaveLogItem>());
    RegistMsg(std::make_shared<LMsgCe2LGMCharge>());
    RegistMsg(std::make_shared<LMsgCe2LGMHorse>());
    RegistMsg(std::make_shared<LMsgCe2LGMBuyInfo>());
    RegistMsg(std::make_shared<LMsgCe2LGMCoins>());
    RegistMsg(std::make_shared<LMsgCe2LGMHide>());

    RegistMsg(std::make_shared<LMsgG2LUserOutMsg>());
    RegistMsg(std::make_shared<LMsgL2GUserOutMsg>());

    RegistMsg(std::make_shared<LMsgC2SVerifyHead>());

    RegistMsg(std::make_shared<LMsgL2LdbLogin>());
    RegistMsg(std::make_shared<LMsgL2LDBSaveLogItem>());
    RegistMsg(std::make_shared<LMsgL2LDBSaveVideo>());
    RegistMsg(std::make_shared<LMsgL2LBDReqVipLog>());
    RegistMsg(std::make_shared<LMsgLBD2LReqVipLog>());
    RegistMsg(std::make_shared<LMsgL2LDBReqVideo>());
    RegistMsg(std::make_shared<LMsgLDB2LReqVideo>());

    RegistMsg(std::make_shared<LMsgL2LBDReqCRELog>());
    RegistMsg(std::make_shared<LMsgL2LDBSaveCRELog>());
    RegistMsg(std::make_shared<LMsgLDB2LM_RecordCRELog>());
    RegistMsg(std::make_shared<LMsgL2LDBDEL_GTU>());

    RegistMsg(std::make_shared<LMsgLM2CEN_ADD_CRE>());

    RegistMsg(std::make_shared<LMsgLDB2LLogInfo>());
    RegistMsg(std::make_shared<LMsgS2CUserStartDingQue>());
    RegistMsg(std::make_shared<LMsgC2SUserDingQue>());
    RegistMsg(std::make_shared<LMsgS2CUserCompleteDingQue>());

    RegistMsg(std::make_shared<LMsgS2CUserChange>());
    RegistMsg(std::make_shared<LMsgC2SUserChange>());
    RegistMsg(std::make_shared<LMsgS2CUserChangeResult>());
    RegistMsg(std::make_shared<LMsgS2CGameResult>());
    RegistMsg(std::make_shared<LMsgC2STangCard>());
    RegistMsg(std::make_shared<LMsgS2CTangCardRet>());


    RegistMsg(std::make_shared<LMsgLMG2GateLogicInfo>());
    RegistMsg(std::make_shared<LMsgL2LMGLogin>());
    RegistMsg(std::make_shared<LMsgLMG2LRLogInfo>());
    RegistMsg(std::make_shared<LMsgLMG2GHorseInfo>());
    RegistMsg(std::make_shared<LMsgLMG2LLogin>());
    RegistMsg(std::make_shared<LMsgL2LMGModifyUserState>());
    RegistMsg(std::make_shared<LMsgL2GModifyUserState>());
    RegistMsg(std::make_shared<LMsgLMG2LCreateDesk>());
    RegistMsg(std::make_shared<LMsgLMG2LAddToDesk>());
    RegistMsg(std::make_shared<LMsgL2LMGModifyCard>());
    RegistMsg(std::make_shared<LMsgL2LMGAddUserPlayCount>());
    RegistMsg(std::make_shared<LMsgLMG2LPXActive>());
    RegistMsg(std::make_shared<LMsgG2LMGLogin>());
    RegistMsg(std::make_shared<LMsgL2LMGRecyleDeskID>());
    RegistMsg(std::make_shared<LMsgLMG2LdbUserLogin>());
    RegistMsg(std::make_shared<LMsgLDB2LMGUserMsg>());
    RegistMsg(std::make_shared<LMsgLMG2LdbLogin>());
    RegistMsg(std::make_shared<LMsgCE2LSetGameFree>());
    RegistMsg(std::make_shared<LMsgCE2LSetPXActive>());
    RegistMsg(std::make_shared<LMsgCE2LSetOUGCActive>());
    RegistMsg(std::make_shared<LMsgCE2LSetExchActive>());
    RegistMsg(std::make_shared<LMsgCE2LSetActivity>());

    RegistMsg(std::make_shared<LMsgLM_2_LDBBindInviter>());
    RegistMsg(std::make_shared<LMsgLDB2LMGBindingRelationships>());
    RegistMsg(std::make_shared<LMsgLM_2_LDB_ReqInfo>());
    RegistMsg(std::make_shared<LMsgLDB_2_LM_RetInfo>());

    RegistMsg(std::make_shared<LMsgFromLoginServer>());
    RegistMsg(std::make_shared<LMsgToLoginServer>());

    RegistMsg(std::make_shared<LMsgConvertClientMsg>());
    RegistMsg(std::make_shared<LMsgHeartBeatRequestMsg>());
    RegistMsg(std::make_shared<LMsgHeartBeatReplyMsg>());

    RegistMsg(std::make_shared<LMsgNewUserVerifyRequest>());

    RegistMsg(std::make_shared<LMsgCN2LMGLogin>());
    RegistMsg(std::make_shared<LMsgLMG2CNLogin>());
    RegistMsg(std::make_shared<LMsgCN2LMGFreeDeskReq>());
    RegistMsg(std::make_shared<LMsgLMG2CNFreeDeskReply>());
    RegistMsg(std::make_shared<LMsgLMG2CNUserLogin>());
    RegistMsg(std::make_shared<LMsgLMG2CNEnterCoinDesk>());
    RegistMsg(std::make_shared<LMsgCN2LMGModifyUserCoins>());
    RegistMsg(std::make_shared<LMsgL2LMGExchCard>());
    RegistMsg(std::make_shared<LMsgLMG2CNGetCoins>());
    RegistMsg(std::make_shared<LMsgLMG2GLCoinsServerInfo>());
    RegistMsg(std::make_shared<LMsgG2CNLogin>());
    RegistMsg(std::make_shared<LMsgL2CNLogin>());
    RegistMsg(std::make_shared<LMsgCN2LCreateCoinDesk>());
    RegistMsg(std::make_shared<LMsgL2CNCreateCoinDeskRet>());
    RegistMsg(std::make_shared<LMsgL2CNGameResult>());
    RegistMsg(std::make_shared<LMsgL2CNEndCoinDesk>());
    RegistMsg(std::make_shared<LMsgC2SGoOnCoinsDesk>());
    RegistMsg(std::make_shared<LMsgS2CGoOnCoinsDeskRet>());
    RegistMsg(std::make_shared<LMsgS2CKickCoinsDesk>());
    RegistMsg(std::make_shared<LMsgS2CGiveCoins>());
    RegistMsg(std::make_shared<LMsgC2SGetCoins>());
    RegistMsg(std::make_shared<LMsgS2CGetCoins>());
    RegistMsg(std::make_shared<LMsgS2COtherCoins>());

    RegistMsg(std::make_shared<LMsgCe2LGMFengHao>());
    RegistMsg(std::make_shared<LMsgLMG2LKickDesk>());
    RegistMsg(std::make_shared<LMsgS2CKickUser>());

    RegistMsg(std::make_shared<LMsgS2CDeskListUpdate>());
    RegistMsg(std::make_shared<LMsgL2LMGDeskListUpdate>());

    RegistMsg(std::make_shared<LMsgS2CNoticeYiSe>());
    RegistMsg(std::make_shared<LMsgC2SelectYiSeRet>());
    RegistMsg(std::make_shared<LMsgS2CSelectYiSeRet>());

    RegistMsg(std::make_shared<LMsgC2SUserBusy>());
    RegistMsg(std::make_shared<LMsgS2CUserBusy>());

    RegistMsg(std::make_shared<LMsgCe2LGMDissolveRoom>());
    RegistMsg(std::make_shared<LMsgLMG2LGMDissolveRoom>());

    RegistMsg(std::make_shared<LMsgS2CLiangPai>());
    RegistMsg(std::make_shared<LMsgS2CLiangPaiRet>());
    RegistMsg(std::make_shared<LMsgC2SLiangPai>());

    RegistMsg(std::make_shared<LMsgC2SRematch>());
    RegistMsg(std::make_shared<LMsgS2CRematchRet>());
    RegistMsg(std::make_shared<LMsgLMG2LRematch>());
    RegistMsg(std::make_shared<LMsgL2LMGRematchRet>());

}

void LMsgFactory::ClearMsg()
{
    msgMap_.clear();
}

void LMsgFactory::RegistMsg(const LMsgPtr& pMsg)
{
    auto it = msgMap_.find(pMsg->m_msgId);
    if(it != msgMap_.end())
    {
        LOG_ERROR << "LMsgFactory::RegistMsg msg exiset msgId:" <<pMsg->m_msgId;
    }
    else
    {
        msgMap_[pMsg->m_msgId] = pMsg;
    }
}

std::shared_ptr<LMsg> LMsgFactory::CreateMsg(int msgId)
{
    auto it = msgMap_.find(msgId);
    if(it == msgMap_.end())
    {
        LOG_ERROR << "LMsgFactory::CreateMsg msg not exiset msgId:" << msgId;
        return NULL;
    }
    else
    {
        return it->second->Clone();
    }
}

//////////////////////////////////////////////////////////////////////////
LMsg::LMsg(int msgId)
    : m_msgId(msgId),
      bPacked_(false),
      connPtr_(NULL),
      bufPtr_(NULL)
{
    //LOG_INFO << "LMsg(): m_msgId = " << m_msgId;
}

LMsg::~LMsg()
{
    //LOG_INFO << "~LMsg(): m_msgId = " << m_msgId;
}

/// 打包一个消息数据，格式如下
/// |-2 bytes(bodylen)-|-2 bytes(checksum)-|-4 bytes(time)-|-4 bytes(messageid)-|-1 byte(isPacked)-|-N bytes(data)-|
/// |---------------------------Head(12 bytes)----------------------------------|----------------Body--------------|
BufferPtr LMsg::GetSendBufferPtr()
{
    if (bufPtr_.get() == NULL)
    {
        bufPtr_.reset(new Buffer);
        Buffer& buf(*bufPtr_.get());

        /// fill 2 bytes(checksum) + 4 bytes(time) + 4 bytes(messageid)
        Write(buf, static_cast<uint16_t>(0));
        Write(buf, static_cast<uint32_t>(0));
        Write(buf, m_msgId);

        if (bPacked_)
        {
            /// fill 1 bytes(isPacked) + N bytes data
            Write(buf, static_cast<uint8_t>(1));
            msgpack::sbuffer sbuf;
            msgpack::packer<msgpack::sbuffer> pack(&sbuf);
            WritePack(pack);
            buf.append(sbuf.data(), sbuf.size());
        }
        else
        {
            /// fill 1 bytes(isPacked) + N bytes data
            Write(buf, static_cast<uint8_t>(0));
            Write(buf, m_msgId);
            WriteMsg(buf);
        }

        /// fill 2 bytes(body len)
        size_t len = bufPtr_->readableBytes() - 10;
        bufPtr_->prepend(&len, 2);
    }

    return bufPtr_;
}

Buffer* LMsg::GetSendBuffer()
{
    return GetSendBufferPtr().get();
}
