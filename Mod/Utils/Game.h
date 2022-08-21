#pragma once

class Game
{
public:
	static float* ArmsLength;
	static bool ModState;
	static class LocalPlayer* localplayer;
private:
	static class ModuleManager* modmag;
public:
	static auto init()->void;
	static auto exit()->void;
	static auto GetModuleManager()->ModuleManager*;

public:
	static auto IsKeyDown(int key)->bool;
	static auto IsKeyPressed(int key)->bool;

public:
	//瞬间破坏
	static auto Cheat_InstantDestroy(bool)->void;
};