#pragma once
#include<string>
#include"iniconfigdef.h"
class IniConfig
{
protected:IniConfig();
public: 
        ~IniConfig();
        bool loadfile(const std::string &path);
        const st_env_config& getconfig();
        static IniConfig* GetInstance();
private:st_env_config _config;
        bool _isloaded;
        static IniConfig* _iniconfig;
};