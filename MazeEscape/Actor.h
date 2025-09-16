#pragma once
#include <string>

class Actor
{
public:

	inline const std::string& GetName() const { return Name; }

public:
	Actor() = default;
	Actor(std::string InName)
		:Name(InName)
	{

	}
protected:
	std::string Name = "¿Ã∏ß";
};

