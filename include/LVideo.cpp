//
// Created by root on 11/2/17.
//

#include "LVideo.h"
#include "BufReadWrite.h"

VideoLog::VideoLog() : m_Id("")
{
    memset(m_userId, 0, sizeof(m_userId));
    m_time = 0;
    m_curCircle = 0;
    m_maxCircle = 0;
    m_deskId = 0;
    memset(m_score,0,sizeof(m_score));
    m_zhuang = 0;
    m_flag = 0;
}

VideoLog::~VideoLog()
{

}

void VideoLog::AddOper(Lint oper, Lint pos, std::vector<CardValue>& cards, Lint* score)
{
    VideoOper o;
    o.m_oper = oper;
    o.m_pos = pos;
    if (score != NULL)
    {
        for (Lint i = 0; i < 4; i++)
        {
            o.m_score.push_back(score[i]);
        }
    }
    for (Lsize i = 0; i < cards.size(); i ++)
    {
        o.m_cards.push_back(cards[i]);
    }
    m_oper.push_back(o);
}


void VideoLog::DealCard(Lint* userId, std::vector<CardValue>* vec,Lint time,Lint zhuang,Lint* score,Lint deskId,Lint cur,Lint max,Lint flag, const std::vector<Lint>& playType, const CardValue* fanPai)
{
    memcpy(m_userId, userId, sizeof(m_userId));
    memcpy(m_score,score,sizeof(m_score));
    for (Lint i = 0; i < 4; ++i)
    {
        m_handCard[i] = vec[i];
    }
    m_time = time;
    m_deskId = deskId;
    m_maxCircle = max;
    m_curCircle = cur;
    m_zhuang = zhuang;
    m_flag = flag;
    m_playType = playType;

    if (fanPai != NULL)
    {
        m_fanPai = *fanPai;
    }
}

Lstring	VideoLog::ToString()
{
    std::stringstream ss;

    for (Lint i = 0; i < 4; ++i)
    {
        for (Lsize j = 0; j < m_handCard[i].size(); ++j)
        {
            ss << m_handCard[i][j].m_color << "," << m_handCard[i][j].m_number;
            if (j + 1 != m_handCard[i].size())
            {
                ss << ",";
            }
        }
        ss << ";";
    }

    for (Lsize i = 0; i < m_oper.size(); ++i)
    {
        ss << m_oper[i].ToString();
    }
    return ss.str();
}

void VideoLog::FromString(Lstring str)
{
    std::vector<Lstring> des,subDes;
    L_ParseString(str, des, ";");
    if (des.size() >= 4)
    {
        for (Lint i = 0; i < 4; ++i)
        {
            subDes.clear();
            L_ParseString(des[i], subDes, ",");
            for (Lsize j = 0; j+1 < subDes.size(); j+=2)
            {
                CardValue v;
                v.m_color = atoi(subDes[j].c_str());
                v.m_number = atoi(subDes[j+1].c_str());
                m_handCard[i].push_back(v);
            }
        }

        for (Lsize i = 4; i < des.size(); ++i)
        {
            VideoOper o;
            o.FromString(des[i]);
            if (o.m_oper != 0)
            {
                m_oper.push_back(o);
            }
        }
    }
}

void VideoLog::WriteData(muduo::net::Buffer& buf)
{
    Write(buf, m_Id);
    Write(buf, m_time);
    Write(buf, m_endTime);
    Write(buf, m_userId[0]);
    Write(buf, m_userId[1]);
    Write(buf, m_userId[2]);
    Write(buf, m_userId[3]);
    Write(buf, m_zhuang);
    Write(buf, m_deskId);
    Write(buf, m_curCircle);
    Write(buf, m_maxCircle);
    Write(buf, m_score[0]);
    Write(buf, m_score[1]);
    Write(buf, m_score[2]);
    Write(buf, m_score[3]);
    Write(buf, m_flag);
    Write(buf, m_str);
    Write(buf, m_str_playtype);
    Write(buf, m_str_fanPai);
}

void VideoLog::ReadData(muduo::net::Buffer& buf)
{
    Read(buf, m_Id);
    Read(buf, m_time);
    Read(buf, m_endTime);
    Read(buf, m_userId[0]);
    Read(buf, m_userId[1]);
    Read(buf, m_userId[2]);
    Read(buf, m_userId[3]);
    Read(buf, m_zhuang);
    Read(buf, m_deskId);
    Read(buf, m_curCircle);
    Read(buf, m_maxCircle);
    Read(buf, m_score[0]);
    Read(buf, m_score[1]);
    Read(buf, m_score[2]);
    Read(buf, m_score[3]);
    Read(buf, m_flag);
    m_str.clear();
    Read(buf, m_str);
    m_str_playtype.clear();
    Read(buf, m_str_playtype);
    Read(buf, m_str_fanPai);
}

void VideoLog::Clear()
{
    m_time = 0;
    m_endTime = 0;
    m_Id = "";
    memset(m_userId, 0, sizeof(m_userId));
    for (Lint i = 0; i < 4; ++i)
    {
        m_handCard[i].clear();
    }
    m_oper.clear();
    m_str.clear();
    m_str_playtype.clear();
    m_str_fanPai.clear();
    m_fanPai.m_color = 0;
    m_fanPai.m_number = 0;
}

Lstring VideoLog::GetInsertSql()
{
    std::stringstream ss;
    ss << "INSERT INTO";
    if ( m_flag > 1000 )	// > 1000 存放到金币表中
    {
        ss << " videocoins ";
    }
    else
    {
        ss << " video ";
    }
    ss << "(Id,Time,EndTime,UserId1,UserId2,UserId3,UserId4,Zhuang,DeskId,MaxCircle,CurCircle,Score1,Score2,Score3,Score4,Flag,Data,PlayType,FanPai) VALUES (";
    ss << "'" << m_Id << "',";
    ss << "'" << m_time << "',";
    ss << "'" << m_endTime << "',";
    ss << "'" << m_userId[0] << "',";
    ss << "'" << m_userId[1] << "',";
    ss << "'" << m_userId[2] << "',";
    ss << "'" << m_userId[3] << "',";
    ss << "'" << m_zhuang << "',";
    ss << "'" << m_deskId << "',";
    ss << "'" << m_maxCircle << "',";
    ss << "'" << m_curCircle << "',";
    ss << "'" << m_score[0] << "',";
    ss << "'" << m_score[1] << "',";
    ss << "'" << m_score[2] << "',";
    ss << "'" << m_score[3] << "',";
    ss << "'" << m_flag << "',";
    ss << "'" << ToString() << "',";
    ss << "'" << PlayTypeToString() << "',";
    ss << "'" << FanPaiToString() << "')";
    return ss.str();
}

Lstring VideoLog::PlayTypeToString()
{
    std::stringstream ss;

    for (Lsize i = 0; i < m_playType.size(); ++i)
    {
        ss << m_playType[i] <<";";
    }
    return ss.str();
}

void VideoLog::PlayTypeFromString(const Lstring& str)
{
    std::vector<Lstring> des;
    L_ParseString(str, des, ";");
    if (des.size() > 0)
    {
        for(Lsize j = 0 ; j < des.size(); ++j)
        {
            m_playType.push_back( atoi(des[j].c_str()));
        }
    }
}

void VideoLog::ParseFanPaiFromString()
{
    if (!m_str_fanPai.empty())
    {
        std::vector<Lstring> des;
        L_ParseString(m_str_fanPai, des, ",");
        if (des.size() == 2)
        {
            m_fanPai.m_color = atoi(des[0].c_str());
            m_fanPai.m_number = atoi(des[1].c_str());
        }
    }
}

Lstring VideoLog::FanPaiToString()
{
    std::stringstream ss;

    if (m_fanPai.m_color != 0 && m_fanPai.m_number != 0)
    {
        ss << m_fanPai.m_color << "," << m_fanPai.m_number;
    }

    return ss.str();
}

