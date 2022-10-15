#pragma once
#include "../Module.h"


class InstantDestroy : public Module {
public:
	InstantDestroy();
	~InstantDestroy();
public:
	auto onStartDestroyBlock(GameMode* gm, vec3_ti* Bpos, uint8_t* Face)->void;

	virtual auto getBindKeyName()->std::string override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};