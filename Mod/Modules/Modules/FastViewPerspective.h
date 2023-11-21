#pragma once
#include "../Module.h"

class FastViewPerspective : public Module {
public:
	FastViewPerspective();

private:
	int ViewPerspective = 2;
	bool hide = false;

public:
	auto isToggle()->bool;
	auto getViewPerspective(int source) -> int;
	auto Hide() -> bool;

public:
	virtual auto getBindKeyName()->std::string override;
	virtual auto onInternalImGUIRender() -> void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};