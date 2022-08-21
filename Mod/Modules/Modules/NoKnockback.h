#pragma once
#include "../Module.h"

class NoKnockback : public Module {
public:
	NoKnockback();

public:
	virtual auto onKnockback(class LocalPlayer*, struct vec3_t*)->bool override;
};