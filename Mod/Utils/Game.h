#pragma once

class Game
{
public:
	static float* ArmsLength;
	static bool ModState;
public:
	static auto init()->void;
	static auto exit()->void;
};