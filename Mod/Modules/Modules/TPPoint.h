#pragma once
#include "../Module.h"

class TPPoint : public Module {
public:
	TPPoint();
	~TPPoint();

private:
	vec3_t* point;

public:
	virtual auto onTrigger()->void override;
	virtual auto getBindKeyName()->std::string override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};