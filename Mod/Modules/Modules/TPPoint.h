#pragma once
#include "../Module.h"

class TPPoint : public Module {
public:
	TPPoint();

private:
	std::optional<vec3_t> point;

	float btnTpPoint[3] = {0};

public:
	virtual auto onTrigger()->void override;
	virtual auto getBindKeyName()->std::string override;
	virtual auto onInternalImGUIRender()->void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};