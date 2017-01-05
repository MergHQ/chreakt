#include <stdio.h>
#include <map>

#include "WebSocketServer.h"
#include "Channel.h";
#include "json.hpp"

std::map<std::string, IEntity*> entities;
void CreateResponse(std::string status, std::string message, int op, nlohmann::json& appendTo)
{
  appendTo["status"] = status;
  appendTo["message"] = message;
  appendTo["op"] = op;
}

int main()
{
	CWebSocketServer server;
	nlohmann::json j;

	server.OnConnection([](uWS::WebSocket<true> ws, uWS::UpgradeInfo handler) {
		printf("NEW CLIENT \n");
	});

	server.OnMessage([&](uWS::WebSocket<uWS::SERVER> ws, std::string message, uWS::OpCode code) {
		nlohmann::json j = nlohmann::json::parse(message.c_str());
		int op = j["op"].get<int>();
		switch(op)
		{
      case 1: 
      { // Create client
				CClient* pClient = new CClient;
				pClient->SetNickname(j["data"]["name"].getAsString());
				entities[pClient->GetSnowflake()] = pClient;
        j["data"]["client_id"] = pClient->GetSnowflake();
        CreateResponse("Ok", "", 10, j);
        ws.send(j.dump().c_str(), code);
				break;
      }
      case 2: 
      { // Join channel
				CChannel* pChannel = (CChannel*)entities[j["data"]["channel_id"].getAsString()];
				CClient* pClient = (CClient*)entities[j["data"]["client_id"].getAsString()];
        if (pChannel && pClient)
        {
          pChannel->AddClient(pClient, ws.getPollHandle());
          CreateResponse("Ok", "", 20, j);
          ws.send(j.dump().c_str(), code);
          break;
        }
        nlohmann::json error;
        CreateResponse("Error", "Invalid client or channel", 20, error);
        ws.send(error.dump().c_str(), code);
        break;
      }
      case 3: 
      { // Create channel
        CChannel* pChannel = new CChannel;
        pChannel->SetDescription(j["data"]["description"].getAsString());
        pChannel->SetName(j["data"]["name"].getAsString());
        pChannel->Init(server);
        entities[pChannel->GetSnowflake()] = pChannel;
        j["data"]["channel_id"] = pChannel->GetSnowflake();
        CreateResponse("Ok", "", 30, j);
        ws.send(j.dump().c_str(), code);
        break;
      }
    }
  });

	server.Start(3000);

  // Clean up
  for (std::pair<std::string, IEntity*> p : entities)
    delete p.second;
}