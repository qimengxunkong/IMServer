#pragma once
#include <event.h>
#include <event2/event.h>
#include<event2/listener.h>
#include"iEvent.h"
#include"glob.h"
#include"DispathMsgService.h"

#define MESSAGE_LEN 10
#define MESSAGE_HEADER_ID "FBEB"
#define MAX_MESSAGE_LEN 367280
enum class SESSION_STATUS
{
	SS_REQUEST,
	SS_RESPONSE
};

enum class MESSAGE_STATUS
{
	MS_READ_HREAD,
	MS_READ_MESSGAE,
	MS_READ_DONE,
	MS_SENDING
};

struct ConnectSession
{
	char remoute_ip[32];

	SESSION_STATUS session_stat;

	iEvent* ev_request;
	MESSAGE_STATUS req_stat;

	iEvent* ev_response;
	MESSAGE_STATUS res_stat;

	u16 eid;
	i32 fd;

	struct bufferevent *bev;
	u32 message_len;
	u32 read_massege_len;
	u32 send_len;
	char* readbuff;
	char header[MESSAGE_LEN + 1];
	char* writebuff;
	u32 read_header_len;
};

class NetworkInterface
{
public:
	NetworkInterface();
	~NetworkInterface();
	bool start(int port);
	void close();
	static void listenerCb(struct evconnlistener* listener, evutil_socket_t fd,
		struct sockaddr* sock, int socklen, void* arg);
	static void handleRequest(struct bufferevent* bev, void* arg);
	static void handleResponse(struct bufferevent* bev, void* arg);
	static void hadnleError(struct bufferevent* bev, short event, void* arg);

	void NewworkEventDispath();
	static void sendResponseMessage(ConnectSession* cs);
private:
	event_base* _base;
	struct evconnlistener* _listener;
};

