#pragma once
#include<string>
struct st_env_config
{
    std::string db_ip;
    unsigned short  db_port;
    std::string db_user;
    std::string db_pwd;
    std::string db_name;

    short svr_port;

    st_env_config()
    {

    }

    st_env_config(std::string db_ip,unsigned short  db_port,std::string db_user,std::string db_pwd,std::string db_name,short svr_port)
    {
        this->db_ip = db_ip;
        this->db_port = db_port;
        this->db_user = db_user;
        this->db_pwd = db_pwd;
        this->db_name = db_name;

        this->svr_port = svr_port;
    }

    const st_env_config& operator=(const st_env_config& st)
    {
        if(this != &st)
        {
            this->db_ip = st.db_ip;
            this->db_name = st.db_name;
            this->db_port = st.db_port;
            this->db_pwd = st.db_pwd;
            this->db_user = st.db_user;
            this->svr_port = st.svr_port;
        }
        return *this;
    }
};