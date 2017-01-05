#include "Client.h"

#include <chrono>

using namespace std::chrono;

CClient::CClient()
{
	GenerateSnowflake();
}

CClient::~CClient()
{
}

void CClient::GenerateSnowflake()
{
	milliseconds ms = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
	);

	int rand = ::rand() % 10000;
	m_snowflake = std::to_string(ms.count()) + std::to_string(rand) + "0"; // 0 for clients
}

