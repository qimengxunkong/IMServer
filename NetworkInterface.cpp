#include "NetworkInterface.h"
#include <string.h>
#include <arpa/inet.h>
ConnectSession*  seesion_init(u32 fd, bufferevent* bev)
{
	ConnectSession* temp = NULL;
	temp = new ConnectSession();

	if (!temp) {
		fprintf(stderr, "malloc failed. reason: %m\n");
		return NULL;
	}

	memset(temp, '\0', sizeof(ConnectSession));
	temp->bev = bev;
	temp->fd = fd;
	return temp;
}

void session_free(ConnectSession* cs)
{
	if (cs)
	{
		if (cs->readbuff)
		{
			delete[]cs->readbuff;
			cs->readbuff = nullptr;
		}

		if (cs->writebuff)
		{
			delete[]cs->writebuff;
			cs->writebuff = nullptr;
		}
		delete cs;

	}
}

void session_reset(ConnectSession* cs)
{
	if (cs)
	{
		if (cs->readbuff)
		{
			delete[]cs->readbuff;
			cs->readbuff = nullptr;
		}

		if (cs->writebuff)
		{
			delete[]cs->writebuff;
			cs->writebuff = nullptr;
		}

		cs->session_stat = SESSION_STATUS::SS_REQUEST;
		cs->req_stat = MESSAGE_STATUS::MS_READ_HREAD;

		cs->message_len = 0;
		cs->read_header_len = 0;
		cs->read_header_len = 0;
	}
	else
	{

	}
}

NetworkInterface::NetworkInterface()
{
	_base = nullptr;
	_listener = nullptr;
}

NetworkInterface::~NetworkInterface()
{
	close();
}

 void NetworkInterface::listenerCb(struct evconnlistener* listener, evutil_socket_t fd,
	struct sockaddr* sock, int socklen, void* arg)
{
	 printf("accept a client %d\n", fd);

	 struct event_base* base = (struct event_base*)arg;

	 struct bufferevent* bev = bufferevent_socket_new(base, fd,
		 BEV_OPT_CLOSE_ON_FREE);
	 ConnectSession* cs = seesion_init(fd, bev);
	 strcpy(cs->remoute_ip,inet_ntoa(((sockaddr_in*)(sock))->sin_addr));
	 cs->session_stat = SESSION_STATUS::SS_REQUEST;
	 cs->req_stat = MESSAGE_STATUS::MS_READ_HREAD;
	 LOG_DEBUG("remoute_ip:%s\n", cs->remoute_ip);
	 bufferevent_setcb(cs->bev, handleRequest, handleResponse, hadnleError, cs);
	 bufferevent_enable(bev, EV_READ | EV_PERSIST );
	 bufferevent_settimeout(bev,10,10);
}

 void NetworkInterface::handleRequest(struct bufferevent* bev, void* arg)
{
	 ConnectSession* cs = static_cast<ConnectSession*>(arg);
	 if (cs->session_stat != SESSION_STATUS::SS_REQUEST)
	 {
		 printf("NetworkInterface handleRequest Sessionstat Error\n");
		 return;
	 }

	 if (cs->req_stat == MESSAGE_STATUS::MS_READ_HREAD)
	 {
		 while (cs->read_header_len != MESSAGE_LEN)
		 {
			 i32 len = bufferevent_read(cs->bev, cs->header + cs->read_header_len, MESSAGE_LEN - cs->read_header_len);
			 cs->read_header_len += len;
		 }
		 cs->header[cs->read_header_len + 1] = '\0';
		 
		 if (strncmp(cs->header, MESSAGE_HEADER_ID, strlen(MESSAGE_HEADER_ID)) == 0)
		 {
			 cs->eid = *(u16*)(cs->header + 4);
			 cs->message_len = *(u32*)(cs->header + 6);

			 if (cs->message_len < 1 || cs->message_len > MAX_MESSAGE_LEN)
			 {
				 printf("NetworkInterface handleRequest MessageLen unExpection\n");
				 return;
			 }
			 LOG_DEBUG("Head read success,eid:%d,length:%d\n",cs->eid,cs->message_len);
			 cs->readbuff = new char[cs->message_len];
			 cs->req_stat = MESSAGE_STATUS::MS_READ_MESSGAE;
			 cs->read_massege_len = 0;
		 }
		 else
		 {
			 printf("NetworkInterface handleRequest HeaderID unExpection\n");
			 bufferevent_free(bev);
			 session_free(cs);
			 return;
		 }

	 }

	 if (cs->req_stat == MESSAGE_STATUS::MS_READ_MESSGAE && evbuffer_get_length(bufferevent_get_input(bev)) > 0)
	 {
		 while (cs->read_massege_len != cs->message_len)
		 {
			 i32 len = bufferevent_read(cs->bev, cs->readbuff + cs->read_massege_len, cs->message_len - cs->read_massege_len);
			 cs->read_massege_len += len;
		 }
		 
		 cs->session_stat = SESSION_STATUS::SS_RESPONSE;
		 iEvent * ev = DispathMsgService::GetInstance()->parseEvent(cs->readbuff,cs->message_len,cs->eid);
		 delete[]cs->readbuff;
		 cs->readbuff = nullptr;
		 cs->read_massege_len = 0;

		 if (ev)
		 {
			 cs->ev_request = ev;
			 ev->SetArg(cs);
			 DispathMsgService::GetInstance()->enqueue(ev);
		 }
		 else
		 {
			 printf("NetworkInterface handleRequest Response ev is nullptr\n");
			 bufferevent_free(cs->bev);
			 session_free(cs);
			 return;
		 }
	 }
}

 void NetworkInterface::handleResponse(struct bufferevent* bev, void* arg)
{

}

 void NetworkInterface::hadnleError(struct bufferevent* bev, short event, void* arg)
{
	 ConnectSession* cs = static_cast<ConnectSession*>(arg);
	 if ((event & BEV_EVENT_TIMEOUT) && (event & BEV_EVENT_READING))
	 {
		 LOG_WARN("NetworkInterface hadnleError Timeout read\n");
	 }
	 else if ((event & BEV_EVENT_TIMEOUT) && (event & BEV_EVENT_WRITING))
	 {
		 LOG_WARN("NetworkInterface hadnleError Timeout write\n");
	 }
	 else if(event & BEV_EVENT_EOF)
	 {
		 LOG_DEBUG("NetworkInterface hadnleError Connect lost\n");
	 }
	 bufferevent_free(bev);
	 session_free(cs);
}



bool NetworkInterface::start(int port)
{
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	_base = event_base_new();

	_listener = evconnlistener_new_bind(_base, NetworkInterface::listenerCb, _base, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
		512, (struct sockaddr*)&sin, sizeof(struct sockaddr_in));
	
	return _listener ? true : false;
}

void NetworkInterface::close()
{
	if (_listener)
	{
		evconnlistener_free(_listener);
		_listener = nullptr;
	}

	if (_base)
	{
		event_base_free(_base);
		_base = nullptr;
	}
}

void NetworkInterface::NewworkEventDispath()
{
	event_base_loop(_base,EVLOOP_NONBLOCK);

	DispathMsgService::GetInstance()->handleAllRspEvent(this);
}

void NetworkInterface::sendResponseMessage(ConnectSession* cs)
{
	if (cs->ev_response == nullptr)
	{
		if (cs->ev_request)
		{
			delete cs->ev_request;
			cs->ev_request = nullptr;
		}
		bufferevent_free(cs->bev);
		session_free(cs);
	}
	else
	{
		bufferevent_write(cs->bev,cs->writebuff,cs->message_len + MESSAGE_LEN);
		session_reset(cs);
	}
}