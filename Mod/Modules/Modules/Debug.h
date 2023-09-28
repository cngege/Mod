#pragma once
#include "../Module.h"

class Debug : public Module {
public:
	Debug();
private:

public:
	bool SynchronousD3D12Render_ImGui = false;
	bool GameMode_attack_Print = false;
	bool GameMode_attack_UseItem = false;

public:
	virtual auto onImGUIRender() -> void override;
	virtual auto onKeyUpdate(int, bool) -> void override;
	virtual auto onInternalImGUIRender() -> void override;
	virtual auto onloadConfigFile(json& data) -> void override;
	virtual auto onsaveConfigFile(json& data) -> void override;
};