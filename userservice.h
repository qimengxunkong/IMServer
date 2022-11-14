#pragma once
#include"sqltables.h"
#include<memory>
class userservice
{
public:
	userservice(std::shared_ptr<MysqlConnection> sqlcon) :_sqlcon(sqlcon)
	{

	}

	bool exist(const std::string &mobile);
	bool insert(const std::string &mobile);
private:std::shared_ptr<MysqlConnection> _sqlcon;
};

