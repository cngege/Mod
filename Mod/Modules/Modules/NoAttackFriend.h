#pragma once
#pragma execution_character_set("utf-8")
#include "../Module.h"


class NoAttackFriend : public Module
{
public:
	NoAttackFriend();

public:
	std::string colorbyte[17] = { "§","0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"};

public:
	virtual auto onAttackBefore(class GameMode* gm,class Actor*)->bool override;

	auto IsFriend(class Player*)->bool;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};