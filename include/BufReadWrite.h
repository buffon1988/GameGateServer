//
// Created by root on 11/7/17.
//

#ifndef PROJECT_BUFREADWRITE_H
#define PROJECT_BUFREADWRITE_H

#include <muduo/net/Buffer.h>


/// Write 接口模板
template<class T> void Write(muduo::net::Buffer& buf, const T& value)
{
    buf.append(&value, sizeof(value));
}

/// std::string 的特例版本
void Write(muduo::net::Buffer& buf, const std::string& value);

/// Buffer 的特例版本(把value写入到buf)
void Write(muduo::net::Buffer& buf, const muduo::net::Buffer& value);


///////////////////////////////////////////////////////////////////////////
/// Read 接口模板
template<class T> void Read(muduo::net::Buffer& buf, T& value)
{
    int len = sizeof(value);
    if (len <= static_cast<int>(buf.readableBytes()))
    {
        const void* data = static_cast<const void*>(buf.peek());
        memcpy(&value, data, len);
        buf.retrieve(len);
    }
}

/// std::string 的特例版本
void Read(muduo::net::Buffer& buf, std::string& value);

/// Buffer 的特例版本(从buf中读出value)
void Read(muduo::net::Buffer& buf, muduo::net::Buffer& value);



#endif //PROJECT_BUFREADWRITE_H
