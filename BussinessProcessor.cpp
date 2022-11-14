#include "BussinessProcessor.h"
void BussinessProcessor::init()
{
	sqltables st(_sqlcon);
	st.CreateFriendRelationshipTab();
	st.CreatUserInfoTab();
}