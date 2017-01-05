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
				CClient* client = new CClient;
				client->SetNickname(j["data"]["name"].getAsString());
				entities[client->GetSnowflake()] = client;
        j["data"]["client_id"] = client->GetSnowflake();
        CreateResponse("Ok", "", 10, j);
        ws.send(j.dump().c_str(), code);
				break;
			}
			case 2: 
      { // Join channel
				CChannel* channel = (CChannel*)entities[j["data"]["channel_id"].getAsString()];
				CClient* client = (CClient*)entities[j["data"]["client_id"].getAsString()];
        if (channel && client)
        {
          channel->AddClient(client, ws.getPollHandle());
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
        CChannel* channel = new CChannel;
        channel->SetDescription(j["data"]["description"].getAsString());
        channel->SetName(j["data"]["name"].getAsString());
        channel->Init(server);
        entities[channel->GetSnowflake()] = channel;
        j["data"]["channel_id"] = channel->GetSnowflake();
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