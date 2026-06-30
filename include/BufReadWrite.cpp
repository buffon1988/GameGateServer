//
// Created by root on 11/7/17.
//

#include "BufReadWrite.h"


/// std::string 的特例版本
void Write(muduo::net::Buffer& buf, const std::string& value)
{
    int len = static_cast<int>(value.size()); // FIXME: IF USE SAME LMsg, int replace with size_t
    Write(buf, len);
    if (len > 0)
    {
        buf.append(value.data(), len);
    }
}

/// Buffer 的特例版本(把value写入到buf)
void Write(muduo::net::Buffer& buf, const muduo::net::Buffer& value)
{
    int len = static_cast<int>(value.readableBytes());
    Write(buf, len);
    if (len > 0)
    {
        buf.append(value.peek(), len);
    }
}



/// std::string 的特例版本
void Read(muduo::net::Buffer& buf, std::string& value)
{
    int len = 0;
    Read(buf, len);
    if (len > 0 && len <= static_cast<int>(buf.readableBytes()))
    {
        value = std::string(buf.peek(), len);
        buf.retrieve(len);
    }
}

/// Buffer 的特例版本(从buf中读出value)
void Read(muduo::net::Buffer& buf, muduo::net::Buffer& value)
{
    int len = 0;
    Read(buf, len);
    if (len > 0 && len <= static_cast<int>(buf.readableBytes()))
    {
        value.append(buf.peek(), len);
        buf.retrieve(len);
    }
}
