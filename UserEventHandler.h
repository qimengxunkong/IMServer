#pragma once
#include "iEventHandler.h"
#include "DispathMsgService.h"
#include"events_def.h"
#include<mutex>
class UserEventHandler :
    public iEventHandler
{
public:UserEventHandler();
       ~UserEventHandler();
       virtual iEvent* handle(const iEvent* ev) ;

private:
    MobileCodeRspEv* handleMobileCodeReq(MobileCodeReqEv* ev);
    LoginRspEv* handleLoginReqEv(LoginReqEv* ev);
    i32 CodeGen();
private:
    std::string _mobile;
    std::map<std::string, i32> _m2c;
    std::mutex _mtx;
};

