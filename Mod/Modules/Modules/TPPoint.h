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
};