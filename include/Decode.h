//
// Created by root on 8/19/17.
//

#ifndef GAMESERVER_DECODE_H
#define GAMESERVER_DECODE_H

#include "BaseFunc.h"
#include "LMsg.h"
#include "LMsgCS.h"


typedef std::function<void (const LMsgPtr& msg)> MsgCallback;

class Decode : muduo::noncopyable
{
public:
    enum
    {
        RECV_CLIENT,
        RECV_SERVER,
        RECV_HTTP
    };

    explicit Decode(MsgCallback cb)
        : msgCallback_(cb)
    {
    }

    void OnRecv(const TcpConnectionPtr& conn, Buffer* buf, int type = RECV_SERVER)
    {
        switch (type)
        {
            case RECV_HTTP:
                parseRecvHttp(conn, buf);
                break;
            case RECV_CLIENT:
            case RECV_SERVER:
                parseRecvData(conn, buf, type);
                break;
            default:
                LOG_ERROR << "Error recv type.";
                conn->shutdown();
                break;
        }
    }


private:

/// 解析网络层接收到的 HTTP 请求数据，格式如下
/// |--N bytes(type)--N bytes(params)--HTTP/1.1\r\n---N bytes(other)---\r\n\r\n|
/// |------------------------------N byte(head)--------------------------------|---N bytes(body)---|
    void parseRecvHttp(const TcpConnectionPtr& conn, Buffer* buf)
    {
        bool bHttpReq = false;
        std::string body;
        std::string head;
        size_t len = buf->readableBytes();
        do
        {
            if (len <= 0)
                break;

            std::string httpStr(buf->peek(), len);
            //LOG_INFO << "httpStr:{" << httpStr << "}, len = " << len;
            std::string::size_type posHeadEnd = httpStr.find("\r\n\r\n");
            if(posHeadEnd == std::string::npos)
            {
                if (len > 65535)
                {
                    LOG_ERROR << "Invalid request length : " << len;
                    buf->retrieve(len);
                }
                break;
            }

            head = httpStr.substr(0, posHeadEnd+4);
            //LOG_INFO << "head:[" << head << "], headlen = " << head.length();

            std::string::size_type posContentLenBeg = httpStr.find("Content-Length:");
            if (posContentLenBeg != std::string::npos)
            {
                std::string::size_type posContentLenEnd = httpStr.find("\r\n", posContentLenBeg);
                if (posContentLenEnd == std::string::npos)
                    break;
                std::string strContentLen = httpStr.substr(posContentLenBeg+15, posContentLenEnd-posContentLenBeg-15);
                size_t bodyLen = atoi(strContentLen.c_str());
                if (httpStr.length() - head.length() < bodyLen)
                    break;
                //LOG_INFO << "bodyLen = " << bodyLen;
                if (bodyLen > 0)
                {
                    body = httpStr.substr(posHeadEnd+4, bodyLen);
                    //LOG_INFO << "body:[" << body << "]";
                }
            }
            buf->retrieve(httpStr.length());

            std::string::size_type pos = head.find("HTTP/1.1");
            if (pos == std::string::npos)
            {
                LOG_ERROR << "Not a Http 1.1 request";
                break;
            }
            bHttpReq = true;
        }while ((len = buf->readableBytes()) > 0);

        if (bHttpReq)
        {
            std::shared_ptr<LMsgHttp> lMsgHttp(new LMsgHttp);
            lMsgHttp->connPtr_ = conn;
            lMsgHttp->head_ = head;
            lMsgHttp->body_ = body;
            msgCallback_(lMsgHttp);
            //LOG_INFO << "lMsgHttp in tid = " << muduo::CurrentThread::tid();
        }
        else
        {
            LOG_ERROR << "Not a http req.";
            conn->shutdown();
        }
    }


/// 解析网络层接收到的自定义消息包数据，格式如下
/// |-2 bytes(bodylen)-|-2 bytes(checksum)-|-4 bytes(time)-|-4 bytes(messageid)-|
/// |-----------------------------12 bytes (head)-------------------------------|---N bytes(body)---|
    void parseRecvData(const TcpConnectionPtr& conn, Buffer* buf, int type)
    {
        while (buf->readableBytes() >= kMsgHeaderLen)
        {
            int bodylen = 0;
            const void* data = static_cast<const void*>(buf->peek());
            memcpy(&bodylen, data, 2);
            //LOG_INFO << "parseRecvData Msg body len = " << bodylen;
            buf->retrieve(kMsgHeaderLen);

            if (bodylen < 0)
            {
                LOG_ERROR << "Invalid length : " << bodylen;
                conn->stopRead();
                break;
            }
            else if (buf->readableBytes() >= static_cast<size_t >(bodylen))
            {
                bufPtr_.reset(new Buffer);
                bufPtr_->append(buf->peek(), bodylen);
                buf->retrieve(bodylen);
                if (type == RECV_SERVER)
                    parseServerMsg(conn, bufPtr_);
                else if (type == RECV_CLIENT)
                    parseClientMsg(conn, bufPtr_);
            }
            else
            {
                break;
            }
        }
    }


/// 解析服务器之间的消息包中的body的数据，格式如下
/// |--1 byte(是否经过封包)--|------------------N bytes(消息实体)---------------|
///                        |--4 bytes(消息id)--|------ n bytes(其他数据)-------|
/// |----------------------------------body----------------------------------|
    void parseServerMsg(const TcpConnectionPtr& conn, BufferPtr buf)
    {
        if (buf->readableBytes() <= 0)
        {
            LOG_ERROR << "The Msg Pack len <= 0";
            return;
        }

        LMsgPtr lMsgPtr = NULL;
        int msgId = -1;
        uint8_t isPacked = 0;
        const void* data = static_cast<const void*>(buf->peek());
        memcpy(&isPacked, data, sizeof(uint8_t));
        buf->retrieve(sizeof(uint8_t));

        if (isPacked)
        {
            size_t len = buf->readableBytes();
            lMsgPtr = unpackMsg(buf->peek(), len);
            if (lMsgPtr)
            {
                lMsgPtr->connPtr_ = conn;
            }
            buf->retrieve(len);
        }
        else
        {
            const void* vid = static_cast<const void*>(buf->peek());
            memcpy(&msgId, vid, sizeof(int));
            buf->retrieve(sizeof(int));
            //LOG_INFO << "ParseRecvMsg msgId = " << msgId;
            lMsgPtr = gLMsgFactory.CreateMsg(msgId);
            if (lMsgPtr)
            {
                lMsgPtr->ReadMsg(*buf.get());
                lMsgPtr->connPtr_ = conn;
            }
            else
            {
                LOG_ERROR << "ParseRecvMsg error, msgId = " << msgId;
            }
        }

        if (lMsgPtr)
        {
            //LOG_INFO << "lMsgPtr in tid = " << muduo::CurrentThread::tid();
            msgCallback_(lMsgPtr);
        }
        else
        {
            LOG_ERROR << "Error LMsgPtr.";
        }
    }


/// 解析客户端发送的消息包的中的body数据，格式如下
/// |-2 bytes(验证头长度)-|-N bytes(验证头)-|-2 bytes(消息实体长度)-|-1 byte(封包标记)-|---------N bytes(消息实体)----------|
///                                                                             |-4 bytes(消息id)-|-n bytes(其他数据)-|
/// |--------------------------------------------------body---------------------------------------------------------|
    void parseClientMsg(const TcpConnectionPtr& conn, BufferPtr buf)
    {
        size_t nBufLeftLen = buf->readableBytes();
        if (nBufLeftLen <= 0)
        {
            LOG_ERROR << "The Msg Pack len <= 0";
            return;
        }

        bool bConvert = false;
        std::shared_ptr<LMsgC2SVerifyHead> msgVerifyHead;
        LMsgPtr msgEntity;
        BufferPtr msgData;
        do
        {
            // 解析验证头
            unsigned short uVerifyHeadLen = 0;
            if (nBufLeftLen < 2)
            {
                LOG_ERROR << "verifyhead head length not enough";
                break;
            }

            const void* data = static_cast<const void*>(buf->peek());
            memcpy(&uVerifyHeadLen, data, 2);
            buf->retrieve(2);
            nBufLeftLen -= 2;
            if (nBufLeftLen < uVerifyHeadLen)
            {
                LOG_ERROR << "buf length less than verifyhead length";
                break;
            }

            LMsgPtr msgHeadPtr = unpackMsg(buf->peek(), uVerifyHeadLen);
            if (msgHeadPtr == NULL)
            {
                LOG_ERROR << "parse verifyhead msg error";
                break;
            }
            if (msgHeadPtr->GetMsgId() != MSG_C_2_S_VERIFY_HEAD)
            {
                LOG_ERROR << "the msg is not verifyhead msg";
                break;
            }
            msgVerifyHead = std::static_pointer_cast<LMsgC2SVerifyHead>(msgHeadPtr);
            buf->retrieve(uVerifyHeadLen);
            nBufLeftLen -= uVerifyHeadLen;

            // 解析消息实体
            unsigned short nEntityLen = 0;
            if (nBufLeftLen < 2)
            {
                LOG_ERROR << "msg entity head length not enough";
                break;
            }
            data = static_cast<const void*>(buf->peek());
            memcpy(&nEntityLen, data, 2);
            buf->retrieve(2);
            nBufLeftLen -= 2;
            if (nBufLeftLen < nEntityLen)
            {
                LOG_ERROR << "buf length less than msg entity length";
                break;
            }

            buf->retrieve(1); // 跳过封包字节，来自客户端肯定是被封包过的
            nBufLeftLen -= 1;
            msgData.reset(new Buffer);
            msgData->append(buf->peek(), nBufLeftLen);
            buf->retrieve(nBufLeftLen);
            if(msgVerifyHead->m_iMd5Begin <= 0 || msgVerifyHead->m_iMd5End <= 0 ||
                    msgVerifyHead->m_iMd5End < msgVerifyHead->m_iMd5Begin ||
                    msgVerifyHead->m_iMd5End > static_cast<int>(msgData->readableBytes()))
            {
                LOG_ERROR << "begin or end is wrong";
                break;
            }
            // 根据md5值判断包的合法性
            std::string strMd5Data;
            for(int i = msgVerifyHead->m_iMd5Begin - 1; i < msgVerifyHead->m_iMd5End; ++i)
            {
                unsigned char tmp = *((msgData->peek() + i));
                char convert[10] = {0};
                sprintf(convert, "%02X", tmp);
                strMd5Data += convert;
            }

            strMd5Data += MD5_SIGN; //客户端服务器通讯签名
            std::string strMd5 = calcMd5ToHex(strMd5Data.c_str(), strMd5Data.size());
            if(strMd5.compare(msgVerifyHead->m_strMd5) != 0)
            {
                LOG_ERROR << "Md5 is error! s=" << strMd5<< "--c=" << msgVerifyHead->m_strMd5;
                break;
            }
            //LOG_INFO << "Md5 check OK s=" << strMd5<< "--c=" << msgVerifyHead->m_strMd5;

            msgEntity = unpackMsg(msgData->peek(), msgData->readableBytes());
            if (msgEntity == NULL)
            {
                LOG_ERROR << "parseClientMsg parse entity msg failed";
                break;
            }
            bConvert = true;
            break;
        }while(true);

        if (bConvert)
        {
            //LOG_INFO << "recv a client msg, covert to LMsgConvertClientMsg";
            std::shared_ptr<LMsgConvertClientMsg> pConvertMsg(new LMsgConvertClientMsg);
            pConvertMsg->verifyHeadPtr_ = msgVerifyHead;
            pConvertMsg->msgEntityPtr_ = msgEntity;
            pConvertMsg->msgBufPtr_ = msgData;
            pConvertMsg->verifyHeadPtr_->connPtr_ = conn;
            pConvertMsg->msgEntityPtr_->connPtr_ = conn;
            pConvertMsg->connPtr_ = conn;
            msgCallback_(pConvertMsg);
        }
        else
        {
            LOG_ERROR << conn->peerAddress().toIpPort() << " send error msg.";
            conn->shutdown();
        }
    }

/// 反序列化 msgpack 包
    LMsgPtr unpackMsg(const char* data, std::size_t len)
    {
        LMsgPtr lMsgPtr = NULL;
        try
        {
            msgpack::unpacked unpack;
            msgpack::unpack(unpack, data, len);
            msgpack::object obj = unpack.get();
            if(obj.is_nil())
            {
                LOG_ERROR << "unpackMsg obj is nil.";
                return NULL;
            }
            int msgId = -1;
            ReadMapValue(obj, "m_msgId", msgId);
            //LOG_INFO << "unpackMsg unpack msgId = " << msgId;
            lMsgPtr = gLMsgFactory.CreateMsg(msgId);
            if (lMsgPtr)
            {
                lMsgPtr->ReadPack(obj);
            }
            else
            {
                LOG_ERROR << "unpackMsg unpack error, msgId = " << msgId;
            }
        }
        catch (const std::exception& ex)
        {
            fprintf(stderr, "unpackMsg unpack exception, reason: %s\n", ex.what());
            LOG_ERROR << "unpackMsg unpack exception, reason:" << ex.what();
        }
        catch (...)
        {
            fprintf(stderr, "unpackMsg unpack exception");
            LOG_ERROR << "unpackMsg unpack exception";
        }

        return lMsgPtr;
    }


private:
    MsgCallback msgCallback_;
    BufferPtr bufPtr_;
    const static int kMsgHeaderLen = 12; // 消息头长度
};


#endif //GAMESERVER_DECODE_H
