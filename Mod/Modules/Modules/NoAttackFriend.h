#pragma once
#include "../Module.h"


class NoAttackFriend : public Module
{
public:
	NoAttackFriend();

public:
	char colorbyte[17] = {(char)'§',(char)'0',(char)'1',(char)'2',(char)'3',(char)'4',(char)'5',(char)'6',(char)'7',(char)'8',(char)'9',(char)'a',(char)'b',(char)'c',(char)'d',(char)'e',(char)'f'};

public:
	virtual auto onAttack(class Actor*)->bool override;

	auto IsFriend(class Player*)->bool;
};