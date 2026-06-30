//
// Created by root on 3/22/18.
//

#ifndef PROJECT_INICONFIG_H
#define PROJECT_INICONFIG_H

#include "LBase.h"

class IniConfig
{
public:
    IniConfig();
    IniConfig(Lstring file);
    ~IniConfig();

    void	LoadFile(Lstring file);

    Lstring	GetString(Lstring key);
    Lstring GetString(Lstring key,Lstring def);

    Lint	GetInt(Lstring key);
    Lint	GetInt(Lstring key,Lint def);

    Lshort	GetShort(Lstring key);
    Lshort  GetShort(Lstring key,Lshort def);

    Lchar	GetChar(Lstring key);
    Lchar	GetChar(Lstring key,Lchar def);

private:
    std::map<Lstring,Lstring> m_ini_map;

};


#endif //PROJECT_INICONFIG_H
