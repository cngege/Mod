#pragma once
#include <cstdint>

class Game
{
public:
	static bool ModState;
	static class LocalPlayer* localplayer;
	static class BitmapFont* mcfont;
	static class ClientInstance* Cinstance;
	static uintptr_t KeyMap;
private:
	static class ModuleManager* modmag;
public:
	static auto init()->void;
	static auto exit()->void;
	static auto GetModuleManager()->class ModuleManager*;

public:
	static auto IsKeyDown(int key)->bool;
	static auto IsKeyPressed(int key)->bool;

};