//
// Created by root on 3/22/18.
//

#include "IniConfig.h"

#define MAX_LINE_CHAR 256


IniConfig::IniConfig()
{

}

IniConfig::IniConfig(Lstring file)
{
    LoadFile(file);
}

void IniConfig::LoadFile(Lstring file)
{
    m_ini_map.clear();
    char line_content[MAX_LINE_CHAR];
    std::ifstream ifs(file.c_str());
    while(ifs)
    {
        ifs.getline(line_content,MAX_LINE_CHAR);
        std::string s = line_content;
        std::string::iterator it = s.begin();
        for(;it != s.end();)
        {
            if((*it) == ' ' || (*it) == '\t' || (*it) == '\r' )
            {
                it = s.erase(it);
            }
            else if((*it) == ';' || (*it) == '#')
            {
                it = s.erase(it,s.end());
            }
            else
            {
                ++it;
            }
        }
        Lsize pos = s.find('=');
        if(pos != std::string::npos)
        {
            m_ini_map[s.substr(0,pos)]=s.substr(pos+1,s.size());
        }
    }
    ifs.close();
}


IniConfig::~IniConfig()
{

}

Lstring	IniConfig::GetString(std::string key)
{
    if (m_ini_map.count(key))
    {
        return m_ini_map[key];
    }
    else
    {
        return "";
    }
}

Lstring	IniConfig::GetString(std::string key,std::string def)
{
    if (m_ini_map.count(key))
    {
        return m_ini_map[key];
    }
    else
    {
        return def;
    }
}

Lint IniConfig::GetInt(Lstring key)
{
    std::string map_value=GetString(key);
    if (map_value.size())
    {
        return atoi(map_value.c_str());
    }
    else
    {
        return 0;
    }
}

Lint IniConfig::GetInt(std::string key,Lint def)
{
    std::string map_value=GetString(key);
    if (map_value.size())
    {
        return atoi(map_value.c_str());
    }
    else
    {
        return def;
    }
}

Lshort IniConfig::GetShort(std::string key)
{
    std::string map_value=GetString(key);
    if (map_value.size())
    {
        return (Lshort)atoi(map_value.c_str());
    }
    else
    {
        return 0;
    }
}

Lshort IniConfig::GetShort(std::string key,Lshort def)
{
    std::string map_value=GetString(key);
    if (map_value.size())
    {
        return (Lshort)atoi(map_value.c_str());
    }
    else
    {
        return def;
    }
}

Lchar IniConfig::GetChar(std::string key)
{
    std::string map_value=GetString(key);
    if (map_value.size())
    {
        return (Lchar)atoi(map_value.c_str());
    }
    else
    {
        return 0;
    }
}

Lchar IniConfig::GetChar(std::string key,Lchar def)
{
    std::string map_value=GetString(key);
    if (map_value.size())
    {
        return (Lchar)atoi(map_value.c_str());
    }
    else
    {
        return def;
    }
}