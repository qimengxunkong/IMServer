#include<iostream>
#include"protocol.pb.h"
#include"iEvent.h"
#include"events_def.h"
#include"UserEventHandler.h"
#include<unistd.h>
#include"iniconfig.h"
#include"logger.h"
#include"BussinessProcessor.h"
using namespace std;
int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "MyIMServer conf fail" << std::endl;
		return -1;
	}
	
	if (!Logger::instance()->init(std::string(argv[1])))
	{
		printf("init error\n");
		return -3;
	}
	
	IniConfig *config = IniConfig::GetInstance();
	if (!config->loadfile(std::string(std::string(argv[2]))))
	{
		printf("load %s failed.", argv[1]);
		return -2;
	}

	st_env_config conf_args = config->getconfig();

	

	MobileCodeReqEv* pmcr = new MobileCodeReqEv("18371405274");

	std::shared_ptr<MysqlConnection> sqlCon(new MysqlConnection());

	if (!sqlCon->Init(conf_args.db_ip.data(), conf_args.db_port, conf_args.db_user.data(), conf_args.db_pwd.data(), conf_args.db_name.data()))
	{
		LOG_ERROR("sql connect failed\n");
		return -4;
	}
	BussinessProcessor busproc(sqlCon);

	DispathMsgService* dms = DispathMsgService::GetInstance();
	dms->open();



	NetworkInterface* NWIF = new NetworkInterface();
	if (!NWIF->start(conf_args.svr_port))
	{
		LOG_ERROR("NetworkInface start fail\n");
		return 0;
	}
	while (true)
	{
		NWIF->NewworkEventDispath();
		sleep(1);
	}
	return 0;
}