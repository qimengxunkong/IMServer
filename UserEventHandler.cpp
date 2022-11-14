#include "UserEventHandler.h"
#include<time.h>
#include"sqltables.h"
#include"iniconfig.h"
#include"userservice.h"
UserEventHandler::UserEventHandler()
{
	DispathMsgService* dms = DispathMsgService::GetInstance();
	dms->subscribe(EventID_Mobile_Req,this);
	dms->subscribe(EventID_Login_Req, this);
}

UserEventHandler::~UserEventHandler()
{
	DispathMsgService* dms = DispathMsgService::GetInstance();
	dms->unsubscribe(EventID_Mobile_Req, this);
	dms->unsubscribe(EventID_Login_Req, this);
}

iEvent* UserEventHandler::handle(const iEvent* ev)
{
	if (ev->GetEId() == EventID_Mobile_Req)
	{
		return handleMobileCodeReq(dynamic_cast<MobileCodeReqEv*>(const_cast<iEvent*>(ev)));
	}
	else if (ev->GetEId() == EventID_Login_Req)
	{
		return handleLoginReqEv(dynamic_cast<LoginReqEv*>(const_cast<iEvent*>(ev)));
	}
	else if (ev->GetEId() == EventID_NotifyOne_Req)
	{

	}
	else if (ev->GetEId() == EventID_SendTxtMsg_Req)
	{

	}
	else if (ev->GetEId() == EventID_AddFriends_Req)
	{

	}
	else if (ev->GetEId() == EventID_AddFriendsResult_Req)
	{

	}
	else if (ev->GetEId() == EventID_DeleteFriends_Req)
	{

	}
	else if (ev->GetEId() == EventID_SelectFriends_Req)
	{

	}
	else
	{

	}

	return nullptr;
}

MobileCodeRspEv* UserEventHandler::handleMobileCodeReq(MobileCodeReqEv* ev)
{
	std::string mobile = ev->GetMobile();
	i32 icode = CodeGen();
	_mtx.lock();
	_m2c[mobile] = icode;
	_mtx.unlock();
	return new MobileCodeRspEv(200, icode);
}

i32 UserEventHandler::CodeGen()
{
	srand(static_cast<unsigned int>(time(NULL)));
	i32 code = static_cast<unsigned int>(random() % (999999 - 100000) + 100000);
	return code;
}

LoginRspEv* UserEventHandler::handleLoginReqEv(LoginReqEv* ev)
{
	std::string mobile = ev->GetCode();
	i32 icode = ev->GetICode();
	LoginRspEv* lr = nullptr;
	_mtx.lock();
	auto iter = _m2c.find(mobile);
	if ((iter == _m2c.end())|| ((iter !=_m2c.end()) && (iter->second != icode)))
	{
		lr = new LoginRspEv(EErrc_InvalidData);
	}
	_mtx.unlock();
	if (lr)
	{
		return lr;
	}

	std::shared_ptr<MysqlConnection> sqlCon(new MysqlConnection());
	st_env_config conf_args = IniConfig::GetInstance()->getconfig();

	if (!sqlCon->Init(conf_args.db_ip.data(), conf_args.db_port, conf_args.db_user.data(), conf_args.db_pwd.data(), conf_args.db_name.data()))
	{
		LOG_ERROR("UserEventHandler handleLoginReqEv sqlinit failed\n");
		return new LoginRspEv(EErrc_ProccessFailed);
	}

	userservice us(sqlCon);
	bool result = false;
	if (!us.exist(mobile))
	{
		result = us.insert(mobile);
		if (!result)
		{
			LOG_ERROR("UserEventHandler handleLoginReqEv sql insert failed\n");
			return new LoginRspEv(EErrc_ProccessFailed);
		}
	}
	return new LoginRspEv(EErrc_Success);
}