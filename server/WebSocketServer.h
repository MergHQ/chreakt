#pragma once
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <uWS/uWS.h>

typedef std::function<void(uWS::WebSocket<uWS::SERVER>, uWS::UpgradeInfo handler)> onConCb;
typedef std::function<void(uWS::WebSocket<uWS::SERVER>, char*, size_t, uWS::OpCode)> onMessageCb;
typedef std::function<void(uWS::WebSocket<uWS::SERVER>, std::string, uWS::OpCode)> parsedMessageCb;

class CWebSocketServer
{
public:
	CWebSocketServer();
	~CWebSocketServer();

	void Start(int port);
	void OnConnection(onConCb callback);
	void OnMessage(parsedMessageCb callback);
  uWS::Hub& GetHub() { return m_hub;  };

private:
	void ParseMessage(uWS::WebSocket<uWS::SERVER>, char*, size_t, uWS::OpCode);

	onConCb m_onConnectionCb;
	parsedMessageCb m_onMessageCb;
	uWS::Hub m_hub;
};

