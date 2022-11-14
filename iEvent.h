#pragma once
#include"glob_def.h"
#include"EventType.h"
#include<iostream>
#include"logger.h"
class iEvent
{
public:iEvent(u32 EId,u32 Sn);
	   virtual ~iEvent();
	   virtual std::ostream& dump(std::ostream& out)const;
	   virtual i32 ByteSize() { return 0; }
	   virtual bool SerializeToArray(char* buf, int len) { return true; }
	   u32 GenerateSeqNo();
	   u32 GetEId()const { return _eid; };
	   u32 GetSn()const { return _sn; };
	   void SetArg(void* arg) { _arg = arg; };
	   void* GetArg()const { return _arg; };
	   void SetEId(u32 eid) { _eid = eid; }
private:u32 _eid;
	    u32 _sn;
		void* _arg;
};