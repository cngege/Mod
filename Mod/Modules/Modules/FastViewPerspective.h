#pragma once
#include "../Module.h"

class FastViewPerspective : public Module {
public:
	FastViewPerspective();

private:
	int ViewPerspective = 2;

public:
	auto isToggle()->bool;
	auto getViewPerspective(int source) -> int;
	
public:
	virtual auto getBindKeyName()->std::string override;
	virtual auto onInternalImGUIRender() -> void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};