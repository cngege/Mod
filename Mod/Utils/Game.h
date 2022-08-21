#pragma once

class Game
{
public:
	static float* ArmsLength;
	static bool ModState;
public:
	static auto init()->void;
	static auto exit()->void;

public:
	static auto IsKeyDown(int key)->bool;
	static auto IsKeyPressed(int key)->bool;

	static auto KeyUpdate(int, bool)->void;

public:
	//瞬间破坏
	static auto Cheat_InstantDestroy(bool)->void;
};