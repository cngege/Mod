#pragma once
#include "../Module.h"

class TPPoint : public Module {
	struct TpPointList
	{
		std::string name;
		float point[3];
		int dim;
	};
public:
	TPPoint();

private:
	std::optional<vec3_t> point;
	std::vector<TpPointList> tpPoints = {};

public:
	virtual auto onTrigger()->void override;
	virtual auto getBindKeyName()->std::string override;
	virtual auto onInternalImGUIRender()->void override;
	virtual auto onloadConfigFile(json& data)->void override;
	virtual auto onsaveConfigFile(json& data)->void override;
};