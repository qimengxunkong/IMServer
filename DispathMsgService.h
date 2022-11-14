#pragma once
#include"threadpool/thread_pool.h"
#include"iEventHandler.h"
#include"EventType.h"
#include "events_def.h"
#include"NetworkInterface.h"
#include <vector>
#include<map>
#include<queue>
#include<mutex>

class NetworkInterface;
class DispathMsgService
{
protected:DispathMsgService();
public:
	
	~DispathMsgService();

	virtual bool open();
	virtual void close();

	virtual void subscribe(u32 eid,iEventHandler * hdler);
	virtual void unsubscribe(u32 eid, iEventHandler* hdler);

	virtual iEvent* process(const iEvent* ev);
	virtual i32 enqueue(iEvent *ev);

	static void sev(void *argv);

	static DispathMsgService* GetInstance();

	iEvent* parseEvent(const char* message,u32 len,u32 eid);

	void handleAllRspEvent(NetworkInterface *interface);
private:
	void packageAndSend(iEvent* ev, NetworkInterface* interface);

	thread_pool_t* _tp;
	static DispathMsgService* _dms;

	typedef std::vector<iEventHandler*> T_EventHandler;
	typedef std::map<u32, T_EventHandler> T_EventHanlderMap;
	T_EventHanlderMap _subcribes;

	bool _svrExit;
	std::queue<iEvent*> _rspEvent;
	std::mutex _mtx;
};

