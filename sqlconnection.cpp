#include "sqlconnection.h"
#include<string.h>
MysqlConnection::MysqlConnection():_mysql(nullptr)
{
	_mysql = new MYSQL();
}

MysqlConnection::~MysqlConnection()
{
	if (_mysql)
	{
		delete _mysql;
	}
	_mysql = nullptr;
}


bool MysqlConnection::Init(const char* szHost, int nPort, const char* szUser, const char* szPwd, const char* szDb)
{
	LOG_INFO("MysqlConnection Init Enter\n");

	if ((_mysql = mysql_init(_mysql)) == nullptr)
	{
		LOG_ERROR("MysqlConnection init Error\n");
		return false;
	}

	if (mysql_real_connect(_mysql, szHost, szUser, szPwd, szDb, nPort, nullptr, 0) == nullptr)
	{
		LOG_ERROR("MysqlConnection init connect fail %s\n", this->GetErrInfo());
		return false;
	}

	char cAuto = 1;
	if (mysql_options(_mysql, MYSQL_OPT_RECONNECT, &cAuto) != 0)
	{
		LOG_ERROR("MysqlConnection init option fail %s\n",this->GetErrInfo());
		return false;
	}
	return true;
}

bool MysqlConnection::Execute(const char* szSql)
{
	if (mysql_real_query(_mysql, szSql, strlen(szSql)) != 0)
	{
		if (mysql_errno(_mysql) == CR_SERVER_GONE_ERROR)
		{
			ReConnect();
		}
		return false;
	}
	return true;
}

bool MysqlConnection::Execute(const char* szSql, SqlRecordSet& recordSet)
{
	if (mysql_real_query(_mysql, szSql, strlen(szSql)) != 0)
	{
		if (mysql_errno(_mysql) == CR_SERVER_GONE_ERROR)
		{
			ReConnect();
		}
		return false;
	}

	MYSQL_RES* Res = mysql_store_result(_mysql);
	if (!Res)
	{
		return false;
	}
	recordSet.SetResult(Res);
	return true;
}

int MysqlConnection::EscapeStr(const char* StrSrc, int SrcLen, char* StrDes)
{
	if (!_mysql)
	{
		return 0;
	}
	return mysql_real_escape_string(_mysql,StrDes,StrSrc,SrcLen);
}


void MysqlConnection::Close()
{

}


const char* MysqlConnection::GetErrInfo()
{
	return mysql_error(_mysql);
}


void MysqlConnection::ReConnect()
{
	mysql_ping(_mysql);
}
