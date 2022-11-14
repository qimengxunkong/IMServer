#include"iniconfig.h"
#include<iniparser/iniparser.h>
IniConfig* IniConfig::_iniconfig = nullptr;
IniConfig::IniConfig()
{
    _isloaded = false;
}

IniConfig::~IniConfig()
{

}

bool IniConfig::loadfile(const std::string &path)
{
    dictionary  *ini;
    
    ini = iniparser_load(path.c_str());
    if (ini==NULL) 
    {
        fprintf(stderr, "cannot parse file: %s\n", path.c_str());
        return false ;
    }
    const char *ip = iniparser_getstring(ini,"database:ip","127.0.0.1");
    int port =  iniparser_getint(ini,"database:port",3306);
    const char *user = iniparser_getstring(ini,"database:user","root");
    const char *pwd = iniparser_getstring(ini,"database:pwd","123456");
    const char *db = iniparser_getstring(ini,"database:db","MyIMServer");

    int svrport = iniparser_getint(ini,"server:port",9090);
    _config = st_env_config(std::string(ip),port,std::string(user),std::string(pwd),std::string(db),svrport);

    iniparser_freedict(ini);
    _isloaded = true;
    return _isloaded;
}

const st_env_config& IniConfig::getconfig()
{
    return _config;
}

IniConfig* IniConfig::GetInstance()
{
    if (!_iniconfig)
    {
        _iniconfig = new IniConfig();
    }
    return _iniconfig;
}