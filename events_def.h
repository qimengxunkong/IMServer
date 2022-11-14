#pragma once
#include"iEvent.h"
#include"protocol.pb.h"
class MobileCodeReqEv :public iEvent
{
public:MobileCodeReqEv(std::string msg) :iEvent(EventID_Mobile_Req, iEvent::GenerateSeqNo())
	  {
			_msg.set_mobile(msg);
	  }

	  const std::string GetMobile()const
	  {
		  return _msg.mobile();
	  }

	  virtual std::ostream& dump(std::ostream& out)const;
	  virtual i32 ByteSize() { return _msg.ByteSizeLong(); }
	  virtual bool SerializeToArray(char* buf, int len) { return _msg.SerializeToArray(buf, len); }
private:tutorial::mobile_request _msg;
};

class MobileCodeRspEv :public iEvent
{
public:MobileCodeRspEv(i32 code,i32 icode) :iEvent(EventID_Mobile_Rsp, iEvent::GenerateSeqNo())
	  {
			_msg.set_code(code);
			_msg.set_icode(icode);
			if (EErrorMap.find(static_cast<EErrorCode>(code)) == EErrorMap.end())
			{
				_msg.set_data("ErrorCpde not Found");
			}
			else
			{
				_msg.set_data(EErrorMap.find(static_cast<EErrorCode>(code))->second);
			}
			
	  }
	  virtual i32 ByteSize() { return _msg.ByteSizeLong(); }
	  virtual bool SerializeToArray(char* buf, int len) { return _msg.SerializeToArray(buf, len); }
	  virtual std::ostream& dump(std::ostream& out)const;

	  const i32 GetCode()const { return _msg.code(); };
	  const i32 GetICode()const { return _msg.icode(); };
	  const std::string& GetData()const { return _msg.data(); };
private:tutorial::mobile_response _msg;
};


class LoginReqEv :public iEvent
{
public:
	LoginReqEv(std::string code, i32 icode) :iEvent(EventID_Login_Req,iEvent::GenerateSeqNo())
	{
		_lr.set_mobile(code);
		_lr.set_icode(icode);
	}
	const std::string GetCode()const { return _lr.mobile(); };
	const i32 GetICode()const { return _lr.icode(); };
	virtual i32 ByteSize() { return _lr.ByteSizeLong(); }
	virtual bool SerializeToArray(char* buf, int len) { return _lr.SerializeToArray(buf, len); }
	virtual std::ostream& dump(std::ostream& out)const;

private:tutorial::login_request _lr;
};

class LoginRspEv :public iEvent
{
public:
	LoginRspEv(i32 code) :iEvent(EventID_Login_Rsp, iEvent::GenerateSeqNo())
	{
		_lrsp.set_code(code);
		if (EErrorMap.find(static_cast<EErrorCode>(code)) == EErrorMap.end())
		{
			_lrsp.set_desc("ErrorCpde not Found");
		}
		else
		{
			_lrsp.set_desc(EErrorMap.find(static_cast<EErrorCode>(code))->second);
		}
	}

	const i32 GetCode()const { return _lrsp.code(); };
	const std::string& GetData()const { return _lrsp.desc(); };
	virtual i32 ByteSize() { return _lrsp.ByteSizeLong(); }
	virtual bool SerializeToArray(char* buf, int len) { return _lrsp.SerializeToArray(buf, len); }
	virtual std::ostream& dump(std::ostream& out)const;
private:tutorial::login_response _lrsp;
};

class ExitRspEv :public iEvent
{
public:	
	ExitRspEv() :iEvent(EventID_ExitRsp, iEvent::GenerateSeqNo())
	{

	}
};