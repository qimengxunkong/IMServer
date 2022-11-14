#include"iEvent.h"

iEvent::iEvent(u32 EId, u32 Sn):_eid(EId),_sn(Sn)
{

}
iEvent::~iEvent()
{

}

std::ostream& iEvent::dump(std::ostream& out)const
{

}


u32 iEvent::GenerateSeqNo()
{
	static u32 sn = 0;
	return sn++;
}
