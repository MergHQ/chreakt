#pragma once
#include <string>

struct IEntity
{
	virtual void GenerateSnowflake() = 0;
	virtual std::string GetSnowflake() = 0;
};
