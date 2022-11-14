#include "DispathMsgService.h"
#include"EventType.h"
#include"protocol.pb.h"
DispathMsgService* DispathMsgService::_dms = nullptr;

DispathMsgService::DispathMsgService()
{
	_tp = nullptr;
	_svrExit = false;
}

DispathMsgService::~DispathMsgService()
{

}


bool DispathMsgService::open()
{
	_tp = thread_pool_init();
}

void DispathMsgService::close()
{
	_svrExit = true;
	thread_pool_destroy(_tp);
	_subcribes.clear();
}


void DispathMsgService::subscribe(u32 eid, iEventHandler* hdler)
{
	if (hdler == nullptr)
	{
		printf("dms subcribe hdler is nullptr\n");
		return;
	}
	auto iter = _subcribes.find(eid);
	if (iter != _subcribes.end())
	{
		auto iterHld = std::find(iter->second.begin(), iter->second.end(), hdler);
		if (iterHld == iter->second.end())
		{
			iter->second.push_back(hdler);
		}
	}
	else
	{
		_subcribes[eid].push_back(hdler);
	}
	
}

void DispathMsgService::unsubscribe(u32 eid, iEventHandler* hdler)
{
	if (hdler == nullptr)
	{
		printf("dms subcribe hdler is nullptr\n");
		return;
	}
	auto iter = _subcribes.find(eid);
	if (iter != _subcribes.end())
	{
		auto iterHld = std::find(iter->second.begin(), iter->second.end(), hdler);
		if (iterHld != iter->second.end())
		{
			iter->second.erase(iterHld);
		}
	}
}


iEvent* DispathMsgService::process(const iEvent* ev)
{
	if (ev == nullptr)
	{
		printf("dms process ev is nullptr\n");
		return nullptr;
	}

	u32 eid = ev->GetEId();
	auto iter = _subcribes.find(eid);
	iEvent* rsp = nullptr;
	if (iter == _subcribes.end())
	{
		printf("dms process ev is not found\n");
		return nullptr;
	}
	else
	{
		for (auto iterhld = iter->second.begin(); iterhld != iter->second.end(); iterhld++)
		{
			rsp = (*iterhld)->handle(ev);
		}
	}
	rsp->dump(std::cout);
	printf("process suceess\n");
	printf("message_len:%d",rsp->ByteSize());
	return rsp;
}

i32 DispathMsgService::enqueue(iEvent* ev)
{
	if (ev == nullptr)
	{
		printf("dms enqueue ev is nullptr\n");
		return -1;
	}
	thread_task_t *task = thread_task_alloc(0);
	task->handler = DispathMsgService::sev;
	task->ctx = ev;
	thread_task_post(_tp, task);
	return 0;
}


void DispathMsgService::sev(void* argv)
{
	if (argv == nullptr)
	{
		printf("dms sev argv is nullptr\n");
	}
	DispathMsgService* dms = DispathMsgService::GetInstance();
	if (!dms->_svrExit)
	{
		iEvent* ev = static_cast<iEvent*>(argv);
		iEvent *rsp = dms->process(ev);
		if (rsp)
		{
			rsp->SetArg(ev->GetArg());
			
		}
		else
		{
			rsp = new ExitRspEv();
			rsp->SetArg(ev->GetArg());
		}

		DispathMsgService::GetInstance()->_mtx.lock();
		DispathMsgService::GetInstance()->_rspEvent.push(rsp);
		DispathMsgService::GetInstance()->_mtx.unlock();
	}
}


DispathMsgService* DispathMsgService::GetInstance()
{
	if (DispathMsgService::_dms == nullptr)
	{
		_dms = new DispathMsgService();
	}
	return _dms;
}

iEvent* DispathMsgService::parseEvent(const char* message, u32 len, u32 eid)
{
	if (message == nullptr)
	{
		LOG_ERROR("DispathMsgService parseEvent message is nullptr\n");
		return nullptr;
	}

	if (eid == EventID_Mobile_Req)
	{
		tutorial::mobile_request mr;
		if (mr.ParseFromArray(message, len))
		{
			MobileCodeReqEv *ev = new MobileCodeReqEv(mr.mobile());
			LOG_DEBUG("DispathMsgService parseEvet success\n");
			return ev;
		}
		else
		{
			LOG_ERROR("DispathMsgService parseEvent Parse EventID_Mobile_Req Message error\n");
		}
	}
	else if (eid == EventID_Login_Req)
	{
		tutorial::login_request lr;
		if (lr.ParseFromArray(message, len))
		{
			LoginReqEv* ev = new LoginReqEv(lr.mobile(),lr.icode());
			LOG_DEBUG("DispathMsgService parseEvet success\n");
			return ev;
		}
		else
		{
			LOG_ERROR("DispathMsgService parseEvent Parse EventID_Mobile_Req Message error\n");
		}
	}
	else
	{

	}
	return nullptr;
}

void DispathMsgService::handleAllRspEvent(NetworkInterface* interface)
{
	bool done = false;

	while (!done)
	{
		iEvent* ev = nullptr;

		_mtx.lock();
		if (!_rspEvent.empty())
		{
			ev = _rspEvent.front();
			_rspEvent.pop();
		}
		else
		{
			done = true;
		}
		_mtx.unlock();

		if (!done)
		{
			if (ev->GetEId() == EventID_Mobile_Rsp)
			{
				packageAndSend(ev, interface);
			}
			else if (ev->GetEId() == EventID_ExitRsp)
			{
				ConnectSession* cs = static_cast<ConnectSession*>(ev->GetArg());
				cs->ev_response = ev;
				interface->sendResponseMessage(cs);
			}
			else if(ev->GetEId() == EventID_Login_Rsp)
			{
				packageAndSend(ev, interface);
			}
			else
			{

			}

		}
		else
		{

		}
	}
}

void DispathMsgService::packageAndSend(iEvent* ev, NetworkInterface* interface)
{
	ConnectSession* cs = static_cast<ConnectSession*>(ev->GetArg());

	if (ev->GetEId() == EventID_Mobile_Rsp)
	{
		ev = dynamic_cast<MobileCodeRspEv*>(ev);
	}
	else if (ev->GetEId() == EventID_Login_Rsp)
	{
		ev = dynamic_cast<LoginRspEv*>(ev);
	}
	else
	{
		ev = dynamic_cast<ExitRspEv*>(ev);
	}

	cs->ev_response = ev;
	cs->message_len = ev->ByteSize();
	cs->writebuff = new char[cs->message_len + MESSAGE_LEN];

	memcpy(cs->writebuff, MESSAGE_HEADER_ID, 4);
	*(u16*)(cs->writebuff + 4) = ev->GetEId();
	*(i32*)(cs->writebuff + 6) = ev->ByteSize();
	printf("message len:%d ,message:%s\n",cs->message_len,cs->writebuff);
	ev->SerializeToArray(cs->writebuff + MESSAGE_LEN, cs->message_len);
	interface->sendResponseMessage(cs);
}