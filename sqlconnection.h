#pragma once
#include<mysql/mysql.h>
#include<string>
#include<mysql/errmsg.h>
#include<assert.h>
#include"iEvent.h"

class SqlRecordSet
{
public:
	SqlRecordSet() :_pRes(nullptr)
	{

	}
	explicit SqlRecordSet(MYSQL_RES* Res)
	{
		_pRes = Res;
	}
	~SqlRecordSet() 
	{
		if (_pRes)
		{
			mysql_free_result(_pRes);
		}
	}

	inline void SetResult(MYSQL_RES* Res)
	{
		assert(_pRes == nullptr);
		if (Res == nullptr)
		{
			LOG_WARN("SqlRecordSet SetResult Res is nullptr\n");
		}
		_pRes = Res;
	}

	i32 GetRowCount()const { return _pRes->row_count; };
	MYSQL_RES* GetResult()const { return _pRes; };
	void FetchRow(MYSQL_ROW &row)
	{
		row = mysql_fetch_row(_pRes);
	}
	inline void SetResult();
private:MYSQL_RES* _pRes;
};
class MysqlConnection
{
public:MysqlConnection();
	  ~MysqlConnection();
	  MYSQL* GetMysql()const { return _mysql; };

	  bool Init(const char *szHost,int nPort,const char* szUser,const char* szPwd,const char* szDb);
	  bool Execute(const char* szSql);
	  bool Execute(const char* szSql,SqlRecordSet& recordSet);
	  int EscapeStr(const char*StrSrc,int SrcLen,char *StrDes);
	  void Close();

	  const char* GetErrInfo();

	  void ReConnect();
private:MYSQL* _mysql;
};

