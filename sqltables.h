#pragma once
#include <memory>
#include "sqlconnection.h"
#include "iEvent.h"
class sqltables
{
public:sqltables(std::shared_ptr<MysqlConnection> sqlCon) : _sqlconn(sqlCon){};
	  ~sqltables() {};
	  bool CreatUserInfoTab()
	  {
		  const char* pUserInfoTabel = " \
									 CREATE TABLE IF NOT EXISTS userinfo( \
									 id            int(16)          NOT NULL PRIMARY KEY AUTO_INCREMENT COMMENT'用户id', \
                                     mobile        varchar(16)      NOT NULL DEFAULT '13000000000' COMMENT'手机号', \
                                     username      varchar(128)     NOT NULL DEFAULT '' COMMENT'用户名', \
                                     verify        int(4)           NOT NULL DEFAULT 0 COMMENT'验证',  \
                                     registertm    timestamp        NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT'注册时间', \
                                     INDEX         mobile_index(mobile)   \
					                 )";
		  if (!_sqlconn->Execute(pUserInfoTabel))
		  {
			  LOG_ERROR("create table userinfo table failed. error msg: %s",_sqlconn->GetErrInfo());
			  return false;
		  }
		  return true;
	  }

	  bool CreateFriendRelationshipTab()
	  {
		  const char* pBikeInfoTabel = " \
			                         CREATE TABLE IF NOT EXISTS friend( \
	                                 person1id     int              NOT NULL PRIMARY KEY AUTO_INCREMENT COMMENT'用户1id', \
			                         person2id     int              NOT NULL COMMENT'用户2id', \
                                     PRIMARY KEY(person1id,person2id) '主键', \
                                     FOREIGN KEY(person1id) REFERENCE userinfo(id)  '外键1', \
                                     FOREIGN KEY(person2id) REFERENCE userinfo(id) '外键2', \
                                     )";
		  if (!_sqlconn->Execute(pBikeInfoTabel))
		  {
			  LOG_ERROR("create table bikeinfo table failed. error msg: %s", _sqlconn->GetErrInfo());
			  return false;
		  }
		  return true;
	  }

private:
	std::shared_ptr<MysqlConnection> _sqlconn;
};

