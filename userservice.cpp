#include "userservice.h"
bool userservice::exist(const std::string& mobile)
{
	char sqlcontent[1024] = "";
	sprintf(sqlcontent,"select * from userinfo where mobile = %s",mobile.data());
	SqlRecordSet rs;
	if (!_sqlcon->Execute(sqlcontent, rs))
	{
		return false;
	}
	return rs.GetRowCount() != 0;
}

bool userservice::insert(const std::string& mobile)
{
	char sqlcontent[1024] = "";
	sprintf(sqlcontent, "insert into userinfo(mobile) values(%s)", mobile.data());
	
	return !_sqlcon->Execute(sqlcontent);
}
