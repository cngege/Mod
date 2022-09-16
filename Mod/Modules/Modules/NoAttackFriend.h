#pragma once
#include "../Module.h"


class NoAttackFriend : public Module
{
public:
	NoAttackFriend();

public:
	std::string colorbyte[17] = { "§","0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"};

public:
	virtual auto onAttack(class Actor*)->bool override;

	auto IsFriend(class Player*)->bool;
};