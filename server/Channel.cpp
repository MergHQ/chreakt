#include "Channel.h"

#include <chrono>

using namespace std::chrono;

CChannel::CChannel()
{
	GenerateSnowflake();
}	

CChannel::~CChannel()
{
}

void CChannel::Init(CWebSocketServer& server)
{
  m_group = server.GetHub().createGroup<true>(100);
  m_group->onMessage([&](uWS::WebSocket<true> ws, char* message, size_t length, uWS::OpCode code) {
    m_group->broadcast(message, length, code);
  });
}

void CChannel::AddClient(CClient* client, uv_poll_t* poll)
{
  m_clients.push_back(client);
  m_group->addWebSocket(poll);
}

void CChannel::GenerateSnowflake()
{
	milliseconds ms = duration_cast< milliseconds>(
		system_clock::now().time_since_epoch()
	);

	int rand = ::rand() % 10000;
	m_snowflake = std::to_string(ms.count()) + std::to_string(rand) + "1"; // 1 for channels
}

