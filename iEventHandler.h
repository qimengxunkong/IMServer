#pragma once
#include"iEvent.h"
#include"EventType.h"
class iEventHandler
{
public:
	iEventHandler() {};
	virtual ~iEventHandler() {};
	virtual iEvent* handle(const iEvent* ev) { return NULL; };
	
	
};

