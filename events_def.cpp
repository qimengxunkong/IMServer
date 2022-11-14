#include"events_def.h"

std::ostream& MobileCodeReqEv::dump(std::ostream& out)const
{
	out << "MobileReq Sn:" << GetSn()<< std::endl;
	out << "MobileReq:" << GetMobile() << std::endl;
	return out;
}

std::ostream& MobileCodeRspEv::dump(std::ostream& out)const
{
	out << "MobileRsp code:" << GetCode() << std::endl;
	out << "MobileRsp icode:" << GetICode() << std::endl;
	out << "MobileRsp Destribe:" << GetData() << std::endl;
	return out;
}

std::ostream& LoginReqEv::dump(std::ostream& out)const
{
	out << "LoginReq Mobile code:" << _lr.mobile() << std::endl;
	out << "LoginReq mobile icode:" << _lr.icode() << std::endl;
}

std::ostream& LoginRspEv::dump(std::ostream& out)const
{
	out << "LoginRsp icode:" << _lrsp.code() << std::endl;
	out << "LoginRsp Describe:" << _lrsp.desc() << std::endl;
}