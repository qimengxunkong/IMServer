#pragma once
#include<unordered_map>
enum EventID
{
	EventID_Mobile_Req = 0x01,
	EventID_Mobile_Rsp = 0x02,

	EventID_Login_Req = 0x03,
	EventID_Login_Rsp = 0x04,

	EventID_NotifyOne_Req = 0x05,
	EventID_NotifyOne_Rsp = 0x06,

	EventID_SendTxtMsg_Req = 0x07,
	EventID_SendTxtMsg_Rsp = 0x08,

	EventID_AddFriends_Req = 0x09,
	EventID_AddFriends_Rsp = 0x10,

	EventID_AddFriendsResult_Req = 0x11,
	EventID_AddFriendsResult_Rsp = 0x12,

	EventID_DeleteFriends_Req = 0x13,
	EventID_DeleteFriends_Rsp = 0x14,

	EventID_SelectFriends_Req = 0x15,
	EventID_SelectFriends_Rsp = 0x16,



	EventID_ExitRsp = 0x55,
	EventID_UnKnow = 0xff,
};


enum EErrorCode
{
	EErrc_Success = 200,
	EErrc_InvalidMsg = 400,
	EErrc_InvalidData = 404,
	EErrc_MethodNotAllowed = 405,
	EErrc_ProccessFailed = 406,
	EErrc_BikeIsTook = 407,
	EErrc_BikeIsRunning = 408,
	EErrc_BikeIsDamaged = 409,
};

static std::unordered_map<EErrorCode, std::string> EErrorMap = {{EErrc_Success,"success"},
																{EErrc_InvalidMsg,"InvalidMsg"},
																{EErrc_InvalidData,"InvalidData"},
																{EErrc_MethodNotAllowed,"MethodNotAllowed" },
																{EErrc_ProccessFailed,"ProccessFailed" },
																{EErrc_BikeIsTook,"BikeIsTook" },
																{EErrc_BikeIsRunning,"BikeIsRunning" },
																{EErrc_BikeIsDamaged,"BikeIsDamaged" },
										
};