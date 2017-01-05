#include "WebSocketServer.h"

CWebSocketServer::CWebSocketServer()
{
}

CWebSocketServer::~CWebSocketServer()
{
}

void CWebSocketServer::Start(int port)
{
	if (m_onConnectionCb != nullptr)
	  m_hub.onConnection(m_onConnectionCb);
	if (m_onMessageCb != nullptr) 
	{
		m_hub.onMessage([&](uWS::WebSocket<uWS::SERVER> ws, char* message, size_t length, uWS::OpCode code) {
			this->ParseMessage(ws, message, length, code);
		});
	}

  m_hub.listen(port);
	m_hub.run();
}

void CWebSocketServer::OnConnection(onConCb cb)
{
	m_onConnectionCb = cb;
}

void CWebSocketServer::OnMessage(parsedMessageCb cb)
{
	m_onMessageCb = cb;
}

void CWebSocketServer::ParseMessage(uWS::WebSocket<uWS::SERVER> ws, char* message, size_t length, uWS::OpCode code)
{
	m_onMessageCb(ws, std::string(message).substr(0, length), code);
}





