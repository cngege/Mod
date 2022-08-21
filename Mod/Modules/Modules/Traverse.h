#pragma once
#include "../Module.h"

class Traverse : public Module {
public:
	Traverse();

public:
	virtual auto onTrigger()->void override;

};