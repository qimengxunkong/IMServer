#pragma once
#include"sqltables.h"
#include"UserEventHandler.h"
#include<memory>
class BussinessProcessor
{
public:
	BussinessProcessor(std::shared_ptr<MysqlConnection> sqlcon) :_sqlcon(sqlcon), _ueh(new UserEventHandler())
	{

	}
	~BussinessProcessor()
	{
		_sqlcon.reset();
	}
	void init();
private:
	std::shared_ptr<MysqlConnection> _sqlcon;
	std::shared_ptr<UserEventHandler> _ueh;

};

