#pragma once
#include <string>
#include <uWS/uWS.h>

#include "IEntity.hpp"

class CClient : public IEntity
{
public:
	CClient();
	~CClient();

	std::string GetNickname() { return m_nickname; }
	void SetNickname(std::string name) { m_nickname = name; }
  void SetSocket(uWS::WebSocket<uWS::SERVER>* socket) { m_socket = socket; }
  uWS::WebSocket<uWS::SERVER>* GetSocket() { return m_socket; }
  void SetAddress(std::string address) { m_address = address; }
  std::string& GetAddress() { return m_address; }

	// IEntity
	virtual std::string GetSnowflake() { return m_snowflake; }
	virtual void GenerateSnowflake();
	// ~IEntity


private:
  std::string m_address;
	std::string m_nickname;
	std::string m_snowflake;
  uWS::WebSocket<uWS::SERVER>* m_socket;

};

