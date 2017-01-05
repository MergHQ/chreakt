#pragma once
#include <vector>

#include "Client.h"

#include "WebSocketServer.h"

class CChannel : public IEntity
{
public:
	CChannel();
	~CChannel();

	std::string GetName() { return m_name; }
	std::string GetDescription() { return m_description; }
	void SetName(std::string name) { m_name = name; }
	void SetDescription(std::string desc) { m_description = desc; }
  std::vector<CClient*>& GetClients() { return m_clients; }
  void Init(CWebSocketServer& server);
  void AddClient(CClient* client, uv_poll_t* poll);

	// IEntity
	virtual std::string GetSnowflake() { return m_snowflake; }
	virtual void GenerateSnowflake();
	// ~IEntity

  private:
  uWS::Group<true>* m_group;
	std::vector<CClient*> m_clients;
	std::string m_name;
	std::string m_description;
	std::string m_snowflake;
};

