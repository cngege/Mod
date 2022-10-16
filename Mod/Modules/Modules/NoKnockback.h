#pragma once
#include "../Module.h"

class NoKnockback : public Module {
public:
	NoKnockback();

public:
	virtual auto getBindKeyName()->std::string override;
	virtual auto onKnockback(class LocalPlayer*, struct vec3_t*)->bool override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};