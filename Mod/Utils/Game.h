#pragma once
#include <cstdint>
#include <string>

class Game
{
public:
	static bool ModState;
	static class ServerPlayer* localServerPlayer;
	static class LocalPlayer* localplayer;
	static class BitmapFont* mcfont;
	static class ClientInstance* Cinstance;
	static void* WindowsHandle;
	static void* ChildWindowsHandle;
	static uintptr_t KeyMap;
	static bool MouseKeyDown[];
	static std::string ImConfigIni;
private:
	static class ModuleManager* modmag;
public:
	static auto init()->void;
	static auto exit()->void;
	static auto GetModuleManager()->class ModuleManager*;

public:
	static auto IsKeyDown(int key)->bool;
	static auto IsKeyPressed(int key)->bool;
	static auto IsMouseDown(int key) -> bool;

	static auto GetLocalServerPlayer() -> class ServerPlayer*;

};